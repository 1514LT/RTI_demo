#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "rti_me_c.h"
#include "wh_sm/wh_sm_history.h"
#include "rh_sm/rh_sm_history.h"

#include "osapi/osapi_thread.h"
#include "osapi/osapi_mutex.h"
#include "osapi/osapi_semaphore.h"

#include "DataType.h"
#include "DataTypeSupport.h"
#include "DataTypeApplication.h"
#include "Base.h"

// Function declarations
RTI_BOOL init_sync_primitives();
void cleanup_sync_primitives();
int get_stop_flag();

DDS_Long throughput_flag = 0;
DDS_Long delay_flag = 0;
DDS_Long small_packet_flag = 0;
DDS_Long large_packet_flag = 0;
DDS_Long test_time = 0;
volatile int stop_flag = 0;
DDS_Boolean jitter_flag = DDS_BOOLEAN_FALSE;

// Add a flag to track if timer has started
static volatile int timer_signal_sent = 0;

// Replace pthread synchronization primitives with RTI OSAPI
OSAPI_Semaphore_T *cond_semaphore = NULL;
OSAPI_Mutex_T *mutex = NULL;
OSAPI_Mutex_T *stop_flag_mutex = NULL;  // Mutex to protect stop_flag

// Function to safely read stop_flag
int get_stop_flag() {
    int value;
    
    // Check if mutex is initialized (only when test_time is used)
    if (stop_flag_mutex == NULL) {
        // If mutex is not initialized, just return the stop_flag value directly
        return stop_flag;
    }
    
    if (!OSAPI_Mutex_take(stop_flag_mutex)) {
        printf("Failed to take stop_flag_mutex in get_stop_flag\n");
        return 0; // Return 0 (not stopped) if mutex fails
    }
    value = stop_flag;
    if (!OSAPI_Mutex_give(stop_flag_mutex)) {
        printf("Failed to give stop_flag_mutex in get_stop_flag\n");
    }
    return value;
}

typedef struct {
    int minutes;
} TimerArgs;

// Convert pthread thread function to RTI thread function
RTI_BOOL timer_thread(struct OSAPI_ThreadInfo *thread_info)
{
    TimerArgs* args = (TimerArgs*)thread_info->user_data;
    int minutes = args->minutes;

    printf("Timer thread started, waiting %d minutes...\n", minutes);

    // Use RTI thread sleep instead of sleep()
    OSAPI_Thread_sleep(minutes * 60 * 1000); // Convert to milliseconds

    // Set flag before sending signal
    timer_signal_sent = 1;
    
    // Signal completion using RTI primitives
    if (!OSAPI_Mutex_take(mutex)) {
        printf("Failed to take mutex in timer thread\n");
        return RTI_FALSE;
    }
    
    if (!OSAPI_Semaphore_give(cond_semaphore)) {
        printf("Failed to signal semaphore in timer thread\n");
        OSAPI_Mutex_give(mutex);
        return RTI_FALSE;
    }
    
    
    if (!OSAPI_Mutex_give(mutex)) {
        printf("Failed to give mutex in timer thread\n");
        return RTI_FALSE;
    }

    return RTI_TRUE;
}

// Convert handle_stop to RTI thread function
RTI_BOOL handle_stop(struct OSAPI_ThreadInfo *thread_info)
{
    RTI_INT32 fail_reason;
    
    
    // Wait for the timer signal in a loop
    while (timer_signal_sent == 0) {
        OSAPI_Thread_sleep(100); // Sleep 100ms and check again
    }
    
    
    if (!OSAPI_Mutex_take(mutex)) {
        printf("Failed to take mutex in handle_stop\n");
        return RTI_FALSE;
    }
    
    // Now take the semaphore (should be available since timer_thread gave it)
    if (!OSAPI_Semaphore_take(cond_semaphore, 1000, &fail_reason)) {
        printf("Failed to wait on semaphore in handle_stop, fail_reason: %d\n", fail_reason);
        OSAPI_Mutex_give(mutex);
        return RTI_FALSE;
    }

    // Set stop_flag with mutex protection
    if (!OSAPI_Mutex_take(stop_flag_mutex)) {
        printf("Failed to take stop_flag_mutex in handle_stop\n");
        OSAPI_Mutex_give(mutex);
        return RTI_FALSE;
    }
    
    stop_flag = 1;
    
    if (!OSAPI_Mutex_give(stop_flag_mutex)) {
        printf("Failed to give stop_flag_mutex in handle_stop\n");
    }
    
    printf("handle_stop\n");

    if (!OSAPI_Mutex_give(mutex)) {
        printf("Failed to give mutex in handle_stop\n");
        return RTI_FALSE;
    }
    
    return RTI_TRUE;
}

void smallPacketPublisher_on_publication_matched(void *listener_data,
                                                 DDS_DataWriter *writer,
                                                 const struct DDS_PublicationMatchedStatus *status)
{
  if (status->current_count_change > 0)
  {
    printf("Matched a subscriber\n");
  }
  else if (status->current_count_change < 0)
  {
    printf("Unmatched a subscriber\n");
  }
}

void largePacketPublisher_on_publication_matched(void *listener_data,
                                                 DDS_DataWriter *writer,
                                                 const struct DDS_PublicationMatchedStatus *status)
{
  if (status->current_count_change > 0)
  {
    printf("Matched a subscriber for large packet\n");
  }
  else if (status->current_count_change < 0)
  {
    printf("Unmatched a subscriber for large packet\n");
  }
}

extern DDS_Publisher *publisher;
int publisher_main_w_args(DDS_Long domain_id, char *udp_intf, char *peer, DDS_Long sleep_time, DDS_Long count)
{
  DDS_DataWriter *small_datawriter = NULL;
  DDS_DataWriter *large_datawriter = NULL;
  smallPacketDataWriter *small_hw_datawriter = NULL;
  largePacketDataWriter *large_hw_datawriter = NULL;
  struct DDS_DataWriterQos dw_qos = DDS_DataWriterQos_INITIALIZER;
  DDS_ReturnCode_t retcode;
  smallPacket *small_sample = NULL;
  largePacket *large_sample = NULL;
  struct Application *application = NULL;
  DDS_Long i;
  struct DDS_DataWriterListener small_dw_listener = DDS_DataWriterListener_INITIALIZER;
  struct DDS_DataWriterListener large_dw_listener = DDS_DataWriterListener_INITIALIZER;

  DDS_Topic *small_topic = NULL;
  DDS_Topic *large_topic = NULL;

  if (small_packet_flag)
  {
    small_sample = smallPacket_create();
    if (small_sample == NULL)
    {
      printf("failed smallPacket_create\n");
      return 0;
    }
  }

  if (large_packet_flag)
  {
    large_sample = largePacket_create();
    if (large_sample == NULL)
    {
      printf("failed largePacket_create\n");
      return 0;
    }
  }

  application = Application_create("publisher", "subscriber", domain_id, udp_intf, peer, sleep_time, count);

  if (application == NULL)
  {
    printf("failed Application create\n");
    goto done;
  }

  if (publisher == NULL)
  {
    printf("publisher == NULL\n");
    goto done;
  }

  if (small_packet_flag)
  {
    retcode = DDS_DomainParticipant_register_type(application->participant,
                                                  "smallPacket",
                                                  smallPacketTypePlugin_get());
    if (retcode != DDS_RETCODE_OK)
    {
      printf("failed to register smallPacket type\n");
      goto done;
    }

    small_topic = DDS_DomainParticipant_create_topic(application->participant,
                                                     "smallPacketTopic",
                                                     "smallPacket",
                                                     &DDS_TOPIC_QOS_DEFAULT, NULL,
                                                     DDS_STATUS_MASK_NONE);

    if (small_topic == NULL)
    {
      printf("small_topic == NULL\n");
      goto done;
    }
  }

  if (large_packet_flag)
  {
    retcode = DDS_DomainParticipant_register_type(application->participant,
                                                  "largePacket",
                                                  largePacketTypePlugin_get());
    if (retcode != DDS_RETCODE_OK)
    {
      printf("failed to register largePacket type\n");
      goto done;
    }

    large_topic = DDS_DomainParticipant_create_topic(application->participant,
                                                     "largePacketTopic",
                                                     "largePacket",
                                                     &DDS_TOPIC_QOS_DEFAULT, NULL,
                                                     DDS_STATUS_MASK_NONE);

    if (large_topic == NULL)
    {
      printf("large_topic == NULL\n");
      goto done;
    }
  }

  dw_qos.reliability.kind = DDS_RELIABLE_RELIABILITY_QOS;
  dw_qos.resource_limits.max_samples_per_instance = 32;
  dw_qos.resource_limits.max_instances = 2;
  dw_qos.resource_limits.max_samples =
      dw_qos.resource_limits.max_instances * dw_qos.resource_limits.max_samples_per_instance;
  dw_qos.history.depth = 32;
  dw_qos.protocol.rtps_reliable_writer.heartbeat_period.sec = 0;
  dw_qos.protocol.rtps_reliable_writer.heartbeat_period.nanosec = 250000000;

  if (small_packet_flag)
  {
    small_dw_listener.on_publication_matched = smallPacketPublisher_on_publication_matched;
    small_datawriter = DDS_Publisher_create_datawriter(
        publisher, small_topic, &dw_qos, &small_dw_listener, DDS_PUBLICATION_MATCHED_STATUS);

    if (small_datawriter == NULL)
    {
      printf("small_datawriter == NULL\n");
      goto done;
    }
    small_hw_datawriter = smallPacketDataWriter_narrow(small_datawriter);
  }

  if (large_packet_flag)
  {
    large_dw_listener.on_publication_matched = largePacketPublisher_on_publication_matched;
    large_datawriter = DDS_Publisher_create_datawriter(
        publisher, large_topic, &dw_qos, &large_dw_listener, DDS_PUBLICATION_MATCHED_STATUS);

    if (large_datawriter == NULL)
    {
      printf("large_datawriter == NULL\n");
      goto done;
    }
    large_hw_datawriter = largePacketDataWriter_narrow(large_datawriter);
  }

  retcode = Application_enable(application);
  if (retcode != DDS_RETCODE_OK)
  {
    printf("failed to enable application\n");
    goto done;
  }
  
  printf("Waiting for subscriber to match...\n");
  DDS_Boolean matched = DDS_BOOLEAN_FALSE;
  int wait_count = 0;
  const int max_wait_seconds = 30; 
  while (!matched && wait_count < max_wait_seconds * 10)
  {
    struct DDS_PublicationMatchedStatus status;
    if(small_packet_flag)
    {
      retcode = DDS_DataWriter_get_publication_matched_status(small_datawriter, &status);
    }
    else
    {
      retcode = DDS_DataWriter_get_publication_matched_status(large_datawriter, &status);
    }
    if (retcode == DDS_RETCODE_OK && status.current_count > 0)
    {
      matched = DDS_BOOLEAN_TRUE;
      printf("subscriber matched! Current count: %d\n", status.current_count);
    }
    OSAPI_Thread_sleep(100); 
  }

  for (i = 0; (application->count > 0 && i < application->count) || (application->count == 0); ++i)
  {
    if (small_packet_flag)
    {
      small_sample->sequence_number = i;
      if(jitter_flag){small_sample->timestamp_ns = get_current_timestamp_us();}
      else{small_sample->timestamp_ns = get_current_timestamp_ms();}
      for (int index = 0; index < sizeof(small_sample->payload); index++)
      {
        small_sample->payload[index] = 'A' + (index % 26); 
      }
      if(i == application->count - 1)
      {
        small_sample->payload[0] = '#';
      }
      if(get_stop_flag())
      {
        small_sample->payload[0] = '#';
        retcode = smallPacketDataWriter_write(small_hw_datawriter, small_sample, &DDS_HANDLE_NIL);
        printf("send last packeg\n");
        if (retcode != DDS_RETCODE_OK)
        {
          printf("Failed to write end small packet\n");
        }
        goto done;
      }
      retcode = smallPacketDataWriter_write(small_hw_datawriter, small_sample, &DDS_HANDLE_NIL);
      if (retcode != DDS_RETCODE_OK)
      {
        printf("Failed to write small packet\n");
      }
      else
      {
        if(!test_time)
        {
          printf("Sent small packet: seq=%lld, timestamp=%lld\n",small_sample->sequence_number, small_sample->timestamp_ns);
        }
      }
    }

    else if (large_packet_flag)
    {
      large_sample->sequence_number = i;
      large_sample->timestamp_ns = get_current_timestamp_ms();
      for (int index = 0; index < sizeof(large_sample->payload); index++)
      {
        large_sample->payload[index] = 'a' + (index % 26); 
      }
      if(i == application->count - 1)
      {
        large_sample->payload[0] = '#';
      }
      if(get_stop_flag())
      {
        large_sample->payload[0] = '#';
        retcode = largePacketDataWriter_write(large_hw_datawriter, large_sample, &DDS_HANDLE_NIL);
        printf("send last packeg\n");
        if (retcode != DDS_RETCODE_OK)
        {
          printf("Failed to write end large packet\n");
        }
        goto done;
      }
      retcode = largePacketDataWriter_write(large_hw_datawriter, large_sample, &DDS_HANDLE_NIL);
      if (retcode != DDS_RETCODE_OK)
      {
        printf("Failed to write large packet\n");
      }
      else
      {
        if(!test_time)
        {
          printf("Sent large packet: seq=%lld, timestamp=%lld\n",large_sample->sequence_number, large_sample->timestamp_ns);
        }
      }
    }
      OSAPI_Thread_sleep(application->sleep_time);
  }

done:

  Application_delete(application);

  DDS_DataWriterQos_finalize(&dw_qos);

  if (small_sample != NULL)
  {
    smallPacket_delete(small_sample);
  }
  if (large_sample != NULL)
  {
    largePacket_delete(large_sample);
  }

  return 0;
}

#if !(defined(RTI_VXWORKS) && !defined(__RTP__))
int main(int argc, char **argv)
{
  DDS_Long i = 0;
  DDS_Long domain_id = 0;
  char *peer = "239.255.0.1";
  char *udp_intf = NULL;
  DDS_Long sleep_time = 10;
  DDS_Long count = 0;
  char date[64];
  timestamp_to_string(get_current_timestamp_ms(), date, sizeof(date));
  printf("date: %s\n", date);
  for (i = 1; i < argc; ++i)
  {
    if (!strcmp(argv[i], "-domain"))
    {
      ++i;
      if (i == argc)
      {
        printf("-domain <domain_id>\n");
        return -1;
      }
      domain_id = strtol(argv[i], NULL, 0);
    }
    else if (!strcmp(argv[i], "-udp_intf"))
    {
      ++i;
      if (i == argc)
      {
        printf("-udp_intf <interface>\n");
        return -1;
      }
      udp_intf = argv[i];
    }
    else if (!strcmp(argv[i], "-peer"))
    {
      ++i;
      if (i == argc)
      {
        printf("-peer <address>\n");
        return -1;
      }
      peer = argv[i];
    }
    else if (!strcmp(argv[i], "-sleep"))
    {
      ++i;
      if (i == argc)
      {
        printf("-sleep_time <sleep_time>\n");
        return -1;
      }
      sleep_time = strtol(argv[i], NULL, 0);
    }
    else if (!strcmp(argv[i], "-count"))
    {
      ++i;
      if (i == argc)
      {
        printf("-count <count>\n");
        return -1;
      }
      count = strtol(argv[i], NULL, 0);
    }
    else if (!strcmp(argv[i], "-throughput_flag"))
    {
      ++i;
      if (i == argc)
      {
        printf("-throughput_flag <throughput>\n");
        return -1;
      }
      throughput_flag = strtol(argv[i], NULL, 0);
    }
    else if (!strcmp(argv[i], "-delay_flag"))
    {
      ++i;
      if (i == argc)
      {
        printf("-delay_flag <delay>\n");
        return -1;
      }
      delay_flag = strtol(argv[i], NULL, 0);
    }
    else if (!strcmp(argv[i], "-small_packet_flag"))
    {
      ++i;
      if (i == argc)
      {
        printf("-small_packet_flag <size>\n");
        return -1;
      }
      small_packet_flag = strtol(argv[i], NULL, 0);
    }
    else if (!strcmp(argv[i], "-large_packet_flag"))
    {
      ++i;
      if (i == argc)
      {
        printf("-large_packet_flag <size>\n");
        return -1;
      }
      large_packet_flag = strtol(argv[i], NULL, 0);
    }
    else if (!strcmp(argv[i],"-test_time"))
    {
      ++i;
      if (i == argc)
      {
        printf("-test_time <size>\n");
        return -1;
      }
      test_time = strtol(argv[i], NULL, 0);
    }
    else if (!strcmp(argv[i],"-jitter_flag"))
    {
      ++i;
      if (i == argc)
      {
        printf("-jitter_flag <value>\n");
        return -1;
      }
      jitter_flag = strtol(argv[i], NULL, 0);
    }
    else if (!strcmp(argv[i], "-h"))
    {
      Application_help(argv[0]);
      return 0;
    }
    else
    {
      printf("unknown option: %s\n", argv[i]);
      return -1;
    }
  }
  if(test_time)
  {
    // Initialize synchronization primitives
    if (!init_sync_primitives()) {
        printf("Failed to initialize sync primitives\n");
        return 1;
    }
    
    // Create timer thread using RTI OSAPI
    struct OSAPI_ThreadProperty timer_thread_prop = OSAPI_THREAD_PROPERTY_DEFAULT;
    TimerArgs args;
    args.minutes = test_time;
    
    struct OSAPI_Thread *timer_thread_handle = OSAPI_Thread_create(
        "timer_thread",
        &timer_thread_prop,
        timer_thread,
        &args,
        NULL  // No wakeup routine needed
    );
    
    if (timer_thread_handle == NULL) {
        printf("Failed to create timer thread\n");
        cleanup_sync_primitives();
        return 1;
    }
    
    // Start timer thread
    if (!OSAPI_Thread_start(timer_thread_handle)) {
        printf("Failed to start timer thread\n");
        OSAPI_Thread_destroy(timer_thread_handle);
        cleanup_sync_primitives();
        return 1;
    }
    
    // Create stop handler thread
    struct OSAPI_ThreadProperty stop_thread_prop = OSAPI_THREAD_PROPERTY_DEFAULT;
    
    struct OSAPI_Thread *stop_thread_handle = OSAPI_Thread_create(
        "stop_thread",
        &stop_thread_prop,
        handle_stop,
        NULL,
        NULL  // No wakeup routine needed
    );
    
    if (stop_thread_handle == NULL) {
        printf("Failed to create stop thread\n");
        OSAPI_Thread_destroy(timer_thread_handle);
        cleanup_sync_primitives();
        return 1;
    }
    
    // Start stop thread
    if (!OSAPI_Thread_start(stop_thread_handle)) {
        printf("Failed to start stop thread\n");
        OSAPI_Thread_destroy(timer_thread_handle);
        OSAPI_Thread_destroy(stop_thread_handle);
        cleanup_sync_primitives();
        return 1;
    }
    
    // Store thread handles for cleanup (you might want to make these global)
    // For now, we'll let them run and clean up at program exit
  }

  return publisher_main_w_args(domain_id, udp_intf, peer, sleep_time, count);
}
#elif defined(RTI_VXWORKS)
int subscriber_main(void)
{
  /* Explicitly configure args below */
  DDS_Long domain_id = 0;
  char *peer = "127.0.0.1";
  char *udp_intf = NULL;
  DDS_Long sleep_time = 1000;
  DDS_Long count = 0;

  return publisher_main_w_args(domain_id, udp_intf, peer, sleep_time, count);
}
#endif

// Add initialization function
RTI_BOOL init_sync_primitives()
{
    mutex = OSAPI_Mutex_new();
    if (mutex == NULL) {
        printf("Failed to create mutex\n");
        return RTI_FALSE;
    }
    
    stop_flag_mutex = OSAPI_Mutex_new();
    if (stop_flag_mutex == NULL) {
        printf("Failed to create stop_flag_mutex\n");
        OSAPI_Mutex_delete(mutex);
        return RTI_FALSE;
    }
    
    cond_semaphore = OSAPI_Semaphore_new();
    if (cond_semaphore == NULL) {
        printf("Failed to create semaphore\n");
        OSAPI_Mutex_delete(stop_flag_mutex);
        OSAPI_Mutex_delete(mutex);
        return RTI_FALSE;
    }
    
    printf("Semaphore initialized (initial value = 1)\n");
    
    return RTI_TRUE;
}

// Add cleanup function
void cleanup_sync_primitives()
{
    if (cond_semaphore != NULL) {
        OSAPI_Semaphore_delete(cond_semaphore);
        cond_semaphore = NULL;
    }
    
    if (stop_flag_mutex != NULL) {
        OSAPI_Mutex_delete(stop_flag_mutex);
        stop_flag_mutex = NULL;
    }
    
    if (mutex != NULL) {
        OSAPI_Mutex_delete(mutex);
        mutex = NULL;
    }
}
