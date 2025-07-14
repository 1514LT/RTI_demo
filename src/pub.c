#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "rti_me_c.h"
#include "wh_sm/wh_sm_history.h"
#include "rh_sm/rh_sm_history.h"

#include "DataType.h"
#include "DataTypeSupport.h"
#include "DataTypeApplication.h"

DDS_Long throughput_flag = 0;
DDS_Long delay_flag = 0;
DDS_Long small_packet_flag = 0;
DDS_Long large_packet_flag = 0;

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
  DDS_DataWriter *small_datawriter;
  DDS_DataWriter *large_datawriter;
  smallPacketDataWriter *small_hw_datawriter;
  largePacketDataWriter *large_hw_datawriter;
  struct DDS_DataWriterQos dw_qos = DDS_DataWriterQos_INITIALIZER;
  DDS_ReturnCode_t retcode;
  smallPacket *small_sample = NULL;
  largePacket *large_sample = NULL;
  struct Application *application = NULL;
  DDS_Long i;
  struct DDS_DataWriterListener small_dw_listener = DDS_DataWriterListener_INITIALIZER;
  struct DDS_DataWriterListener large_dw_listener = DDS_DataWriterListener_INITIALIZER;

  // 为largePacket创建单独的topic
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

  // publisher已经在Application_create中创建了，直接使用全局变量
  if (publisher == NULL)
  {
    printf("publisher == NULL\n");
    goto done;
  }

  if (small_packet_flag)
  {
    // 创建smallPacket的topic
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
    // 创建largePacket的topic
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
    // 创建smallPacket的DataWriter
    small_dw_listener.on_publication_matched = smallPacketPublisher_on_publication_matched;
    small_datawriter = DDS_Publisher_create_datawriter(
        publisher, small_topic, &dw_qos, &small_dw_listener, DDS_PUBLICATION_MATCHED_STATUS);

    if (small_datawriter == NULL)
    {
      printf("small_datawriter == NULL\n");
      goto done;
    }
    sleep(1);
    small_hw_datawriter = smallPacketDataWriter_narrow(small_datawriter);
  }

  if (large_packet_flag)
  {
    // 创建largePacket的DataWriter
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

  // 确保Application被启用（无论使用哪种包类型）
  retcode = Application_enable(application);
  if (retcode != DDS_RETCODE_OK)
  {
    printf("failed to enable application\n");
    goto done;
  }

  for (i = 0; (application->count > 0 && i < application->count) || (application->count == 0); ++i)
  {
    if (small_packet_flag)
    {
      // 发送smallPacket
      small_sample->sequence_number = i;
      // small_sample->timestamp_ns = OSAPI_NtpTime_get_time();
      small_sample->timestamp_ns = 11;
      // 填充payload数据
      for (int index = 0; index < sizeof(small_sample->payload); index++)
      {
        small_sample->payload[index] = 'A' + (index % 26); // 循环填充A-Z
      }
      retcode = smallPacketDataWriter_write(small_hw_datawriter, small_sample, &DDS_HANDLE_NIL);
      if (retcode != DDS_RETCODE_OK)
      {
        printf("Failed to write small packet\n");
      }
      else
      {
        printf("Sent small packet: seq=%lld, timestamp=%lld\n",
               small_sample->sequence_number, small_sample->timestamp_ns);
      }
    }

    if (large_packet_flag)
    {
      // 发送largePacket
      large_sample->sequence_number = i;
      large_sample->timestamp_ns = 11;
      // 填充payload数据
      for (int index = 0; index < sizeof(large_sample->payload); index++)
      {
        large_sample->payload[index] = 'a' + (index % 26); // 循环填充a-z
      }
      retcode = largePacketDataWriter_write(large_hw_datawriter, large_sample, &DDS_HANDLE_NIL);
      if (retcode != DDS_RETCODE_OK)
      {
        printf("Failed to write large packet\n");
      }
      else
      {
        printf("Sent large packet: seq=%lld, timestamp=%lld\n",
               large_sample->sequence_number, large_sample->timestamp_ns);
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
  DDS_Long sleep_time = 1000;
  DDS_Long count = 0;

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
