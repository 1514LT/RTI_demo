#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "rti_me_c.h"
#include "disc_dpse/disc_dpse_dpsediscovery.h"
#include "wh_sm/wh_sm_history.h"
#include "rh_sm/rh_sm_history.h"
#include "DataType.h"
#include "DataTypeSupport.h"
#include "DataTypePlugin.h"
#include "DataTypeApplication.h"
#include "Base.h"

DDS_Long throughput_flag = 0;
DDS_Long delay_flag = 0;
DDS_Long small_packet_flag = 0;
DDS_Long large_packet_flag = 0;
DDS_Long test_time = 0;
DDS_Long start_time = 0;
DDS_Long end_time = 0;
DDS_Long recv_packets = 0;
DDS_LongLong max_delay = 0;
DDS_LongLong min_delay = 0;
DDS_LongLong total_delay = 0;
DDS_LongLong total_delay_squared = 0;
DDS_Boolean jitter_flag = DDS_BOOLEAN_FALSE;

void DataTypeSubscriber_on_data_available(void *listener_data,
                                          DDS_DataReader *reader)
{
  smallPacketDataReader *small_reader = smallPacketDataReader_narrow(reader);
  largePacketDataReader *large_reader = largePacketDataReader_narrow(reader);
  DDS_ReturnCode_t retcode;
  struct DDS_SampleInfo *sample_info = NULL;
  smallPacket *small_sample = NULL;
  largePacket *large_sample = NULL;

  struct DDS_SampleInfoSeq info_seq = DDS_SEQUENCE_INITIALIZER;
  struct smallPacketSeq small_sample_seq = DDS_SEQUENCE_INITIALIZER;
  struct largePacketSeq large_sample_seq = DDS_SEQUENCE_INITIALIZER;

  const DDS_Long TAKE_MAX_SAMPLES = 32;
  DDS_Long i;

  if (small_reader != NULL && small_packet_flag)
  {
    retcode = smallPacketDataReader_take(small_reader,
                                         &small_sample_seq, &info_seq, TAKE_MAX_SAMPLES,
                                         DDS_ANY_SAMPLE_STATE, DDS_ANY_VIEW_STATE, DDS_ANY_INSTANCE_STATE);

    if (retcode != DDS_RETCODE_OK)
    {
      printf("wait ...\n");
    }
    else
    {
      for (i = 0; i < smallPacketSeq_get_length(&small_sample_seq); ++i)
      {
        sample_info = DDS_SampleInfoSeq_get_reference(&info_seq, i);

        if (sample_info->valid_data)
        {
          small_sample = smallPacketSeq_get_reference(&small_sample_seq, i);
          recv_packets ++;
          if(small_sample->payload[0] == '#' && throughput_flag)
          {
            end_time = get_current_timestamp_ms();
            printf("end_time:%lld\n",end_time);
            printf("recv_packets:%lld\n",recv_packets);
            double duration_s = (end_time - start_time) / 1000.0;
            double throughput = (recv_packets * 64 * 8) / duration_s / (1024 * 1024);
            printf("throughput: %.4f Mbps\n", throughput);
            recv_packets = 0;
            exit(0);
          }

          if(recv_packets == 1)
          {
            start_time = get_current_timestamp_ms();
            printf("start_time %lld\n",start_time);
            min_delay = get_current_timestamp_ms() - small_sample->timestamp_ns;
          }
          if(!test_time && delay_flag)
          {
            ll delay = get_current_timestamp_ms() - small_sample->timestamp_ns;
            total_delay += delay;
            if (delay > max_delay){max_delay = delay;}
            if (delay < min_delay){min_delay = delay;}
          }
          else if(!test_time && jitter_flag)
          {
            ll delay = get_current_timestamp_us() - small_sample->timestamp_ns;
            total_delay += delay;
            total_delay_squared += delay * delay;
          }
          if(small_sample->payload[0] == '#' && delay_flag)
          {
            printf("recv end packet\n");
            double avg_delay = (double)total_delay / recv_packets;
            recv_packets = 0;
            avg_delay = 0;
            max_delay = 0;
            min_delay = 0;
            total_delay = 0;
            total_delay_squared = 0;
            exit(0);
          }
          else if(small_sample->payload[0] == '#' && jitter_flag)
          {
            printf("recv end packet\n");
            double avg_delay = (double)total_delay / recv_packets;
            double avg_delay_squared = (double)total_delay_squared / recv_packets;

            double variance = avg_delay_squared - (avg_delay * avg_delay);

            double jitter = rti_sqrt(variance);
            recv_packets = 0;
            avg_delay = 0;
            max_delay = 0;
            min_delay = 0;
            total_delay = 0;
            total_delay_squared = 0;
            exit(0);
          }
        }
      }
      smallPacketDataReader_return_loan(small_reader, &small_sample_seq, &info_seq);
    }
  }


  if (large_reader != NULL && large_packet_flag)
  {
    struct DDS_SampleInfoSeq large_info_seq = DDS_SEQUENCE_INITIALIZER;
    retcode = largePacketDataReader_take(large_reader,
                                         &large_sample_seq, &large_info_seq, TAKE_MAX_SAMPLES,
                                         DDS_ANY_SAMPLE_STATE, DDS_ANY_VIEW_STATE, DDS_ANY_INSTANCE_STATE);

    if (retcode != DDS_RETCODE_OK)
    {
      printf("wait for large packet...\n");
    }
    else
    {
      for (i = 0; i < largePacketSeq_get_length(&large_sample_seq); ++i)
      {
        sample_info = DDS_SampleInfoSeq_get_reference(&large_info_seq, i);

        if (sample_info->valid_data)
        {
          largePacket *large_sample = largePacketSeq_get_reference(&large_sample_seq, i);
          recv_packets ++;
          if(large_sample->payload[0] == '#' && throughput_flag)
          {
            end_time = get_current_timestamp_ms();
            printf("end_time:%lld\n",end_time);
            printf("recv_packets:%lld\n",recv_packets);
            double duration_s = (end_time - start_time) / 1000.0;
            double throughput = (recv_packets * 64 * 8) / duration_s / (1024 * 1024);
            printf("throughput: %.4f Mbps\n", throughput);
            recv_packets = 0;
            exit(0);
          }
          if(recv_packets == 1)
          {
            start_time = get_current_timestamp_ms();
            printf("start_time %lld\n",start_time);
            min_delay = get_current_timestamp_ms() - large_sample->timestamp_ns;
          }
          if(!test_time && delay_flag)
          {
            ll delay = get_current_timestamp_ms() - large_sample->timestamp_ns;
            total_delay += delay;
            if (delay > max_delay){max_delay = delay;}   
            if (min_delay == -1 || delay < min_delay){min_delay = delay;}
          }
          if(large_sample->payload[0] == '#' && delay_flag)
          {
            double avg_delay = (double)total_delay / recv_packets;
            recv_packets = 0;
            avg_delay = 0;
            max_delay = 0;
            min_delay = 0;
            total_delay = 0;
            total_delay_squared = 0;
            exit(0);
          }
        }
      }
      largePacketDataReader_return_loan(large_reader, &large_sample_seq, &large_info_seq);
    }
  }

  DDS_SampleInfoSeq_finalize(&info_seq);
  smallPacketSeq_finalize(&small_sample_seq);
  largePacketSeq_finalize(&large_sample_seq);
}

void DataTypeSubscriber_on_subscription_matched(void *listener_data,
                                                DDS_DataReader *reader,
                                                const struct
                                                DDS_SubscriptionMatchedStatus
                                                    *status)
{
  if (status->current_count_change > 0)
  {
    printf("Matched a publisher\n");
  }
  else if (status->current_count_change < 0)
  {
    printf("Unmatched a publisher\n");
  }
}

void smallPacketSubscriber_filter_sample(smallPacket *sample,
                                         DDS_Boolean *drop_sample)
{
  /* Example filter: drop samples with even-numbered count in data */
  if ((sample->sequence_number % 2) == 0)
  {
    *drop_sample = DDS_BOOLEAN_TRUE;
  }
}

void largePacketSubscriber_filter_sample(largePacket *sample,
                                         DDS_Boolean *drop_sample)
{
  /* Example filter: drop samples with even-numbered count in data */
  if ((sample->sequence_number % 2) == 0)
  {
    *drop_sample = DDS_BOOLEAN_TRUE;
  }
}

DDS_Boolean DataTypeSubscriber_on_before_sample_deserialize(
    void *listener_data,
    DDS_DataReader *reader,
    struct NDDS_Type_Plugin *plugin,
    struct CDR_Stream_t *stream,
    DDS_Boolean *dropped)
{
  *dropped = DDS_BOOLEAN_FALSE;

  /* deserialize the sample */
  return DDS_BOOLEAN_TRUE;
}

DDS_Boolean DataTypeSubscriber_on_before_sample_commit(
    void *listener_data,
    DDS_DataReader *reader,
    const void *const sample,
    const struct DDS_SampleInfo *const sample_info,
    DDS_Boolean *dropped)
{
  smallPacket *hw_sample = (smallPacket *)sample;

  smallPacketSubscriber_filter_sample(hw_sample, dropped);

  if (*dropped)
  {
    printf("\nSample filtered, before commit...\n\tDROPPED - id: %lld\n",
           hw_sample->sequence_number);
  }

  return DDS_BOOLEAN_TRUE;
}

int subscriber_main_w_args(DDS_Long domain_id, char *udp_intf, char *peer, DDS_Long sleep_time,
                           DDS_Long count)
{
  DDS_Subscriber *subscriber;
  DDS_DataReader *small_datareader;
  DDS_DataReader *large_datareader;
  struct DDS_DataReaderQos dr_qos = DDS_DataReaderQos_INITIALIZER;
  DDS_ReturnCode_t retcode;
  struct DDS_PublicationBuiltinTopicData rem_publication_data =
      DDS_PublicationBuiltinTopicData_INITIALIZER;
  struct DDS_DataReaderListener dr_listener = DDS_DataReaderListener_INITIALIZER;
  struct Application *application;


  DDS_Topic *small_topic = NULL;
  DDS_Topic *large_topic = NULL;

  application = Application_create("subscriber", "publisher", domain_id,
                                   udp_intf, peer, sleep_time, count);

  if (application == NULL)
  {
    return 0;
  }

  subscriber = DDS_DomainParticipant_create_subscriber(application->participant,
                                                       &DDS_SUBSCRIBER_QOS_DEFAULT,
                                                       NULL,
                                                       DDS_STATUS_MASK_NONE);
  if (subscriber == NULL)
  {
    printf("subscriber == NULL\n");
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

  else if (large_packet_flag)
  {

    printf("Registering largePacket type...\n");
    retcode = DDS_DomainParticipant_register_type(application->participant,
                                                  "largePacket",
                                                  largePacketTypePlugin_get());
    if (retcode != DDS_RETCODE_OK)
    {
      printf("failed to register largePacket type\n");
      goto done;
    }
    printf("largePacket type registered successfully\n");

    printf("Creating largePacket topic...\n");
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
    printf("largePacket topic created successfully\n");
  }

  /* Uncomment to filter samples with callbacks */
/* #define HW_USE_SAMPLE_FILTER */
#ifdef HW_USE_SAMPLE_FILTER
  /* choose one callback to enable */
#if 0
    dr_listener.on_before_sample_deserialize = 
        DataTypeSubscriber_on_before_sample_deserialize;
#else
  dr_listener.on_before_sample_commit =
      DataTypeSubscriber_on_before_sample_commit;
#endif

#endif
  dr_listener.on_data_available = DataTypeSubscriber_on_data_available;
  dr_listener.on_subscription_matched =
      DataTypeSubscriber_on_subscription_matched;

#if 1 /* DDS_RELIABLE_RELIABILITY_QOS */
  dr_qos.reliability.kind = DDS_RELIABLE_RELIABILITY_QOS;
#else /* DDS_BEST_EFFORT_RELIABILITY_QOS */
  dr_qos.reliability.kind = DDS_BEST_EFFORT_RELIABILITY_QOS;
#endif

  /* INTEROPERABILITY NOTE:
     Non-Connext Micro DDS writers and readers will have default LivelinessQoS
     kind set to DDS_AUTOMATIC_LIVELINESS_QOS.

     Because Connext Micro currently only supports
     DDS_MANUAL_BY_TOPIC_LIVELINESS_QOS for finite lease_duration,
     a non-Connext Micro writer must set its liveliness kind to
     DDS_MANUAL_BY_TOPIC_LIVELINESS_QOS as well in order for
     Requested-Offered (RxO) semantics to be compatible.
  */

  dr_qos.protocol.rtps_object_id = 200;
  dr_qos.resource_limits.max_samples = 1;
  dr_qos.resource_limits.max_samples_per_instance = 1;
  dr_qos.resource_limits.max_instances = 1;
  dr_qos.history.depth = 1;


  struct DDS_DataReaderQos large_dr_qos = DDS_DataReaderQos_INITIALIZER;
  large_dr_qos.reliability.kind = DDS_RELIABLE_RELIABILITY_QOS;
  large_dr_qos.protocol.rtps_object_id = 201; 
  large_dr_qos.resource_limits.max_samples = 1;
  large_dr_qos.resource_limits.max_samples_per_instance = 1;
  large_dr_qos.resource_limits.max_instances = 1;
  large_dr_qos.history.depth = 1;

  if (small_packet_flag)
  {

    small_datareader = DDS_Subscriber_create_datareader(subscriber,
                                                        DDS_Topic_as_topicdescription(small_topic),
                                                        &dr_qos,
                                                        &dr_listener,
                                                        DDS_DATA_AVAILABLE_STATUS |
                                                            DDS_SUBSCRIPTION_MATCHED_STATUS);

    if (small_datareader == NULL)
    {
      printf("small_datareader == NULL\n");
      goto done;
    }
  }

  if (large_packet_flag)
  {

    printf("Creating largePacket DataReader...\n");
    large_datareader = DDS_Subscriber_create_datareader(subscriber,
                                                        DDS_Topic_as_topicdescription(large_topic),
                                                        &large_dr_qos,
                                                        &dr_listener,
                                                        DDS_DATA_AVAILABLE_STATUS |
                                                            DDS_SUBSCRIPTION_MATCHED_STATUS);

    if (large_datareader == NULL)
    {
      printf("large_datareader == NULL\n");
      printf("Error creating largePacket DataReader. Check if topic and QoS are correct.\n");
      goto done;
    }
    printf("largePacket DataReader created successfully\n");
  }


  printf("Skipping remote publication assertions for now...\n");

  retcode = Application_enable(application);
  if (retcode != DDS_RETCODE_OK)
  {
    printf("failed to enable application\n");
    goto done;
  }

  if (application->count != 0)
  {
    printf("Running for %d counts, press Ctrl-C to exit\n", application->count);
  }
  else
  {
    printf("Running for 24 hours, press Ctrl-C to exit\n");
  }

  int sleep_loop_count = (24 * 60 * 60) / 2000;
  int sleep_loop_left = (24 * 60 * 60) % 2000;

  while (sleep_loop_count)
  {
    OSAPI_Thread_sleep(2000 * 1000);
    --sleep_loop_count;
  }

  OSAPI_Thread_sleep(sleep_loop_left * 1000);
  

  Application_delete(application);

  DDS_DataReaderQos_finalize(&dr_qos);
  DDS_DataReaderQos_finalize(&large_dr_qos);

done:
  return 0;
}

#if !(defined(RTI_VXWORKS) && !defined(__RTP__))
int main(int argc, char **argv)
{
  DDS_Long i = 0;
  DDS_Long domain_id = 0;
  char *peer = NULL;
  char *udp_intf = NULL;
  DDS_Long sleep_time = 1000;
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
        printf("-sleep <sleep_time>\n");
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

  return subscriber_main_w_args(domain_id, udp_intf, peer, sleep_time, count);
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

  return subscriber_main_w_args(domain_id, udp_intf, peer, sleep_time, count);
}
#endif
