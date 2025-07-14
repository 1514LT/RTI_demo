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

void
DataTypeSubscriber_on_data_available(void* listener_data,
                                       DDS_DataReader* reader)
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

    // 尝试读取smallPacket数据
    if (small_reader != NULL) {
        retcode = smallPacketDataReader_take(small_reader, 
           &small_sample_seq, &info_seq, TAKE_MAX_SAMPLES,
           DDS_ANY_SAMPLE_STATE, DDS_ANY_VIEW_STATE, DDS_ANY_INSTANCE_STATE);

        if (retcode != DDS_RETCODE_OK)
        {
            printf("failed to take small packet data: %d\n", retcode);
        } else {
            /* 打印每个有效的小包样本 */
            for (i = 0; i < smallPacketSeq_get_length(&small_sample_seq); ++i)
            {
                sample_info = DDS_SampleInfoSeq_get_reference(&info_seq, i);

                if (sample_info->valid_data)
                {
                    small_sample = smallPacketSeq_get_reference(&small_sample_seq, i);
                    printf("\n收到小包数据:\n\t序列号: %lld\n\t时间戳: %lld\n\t数据大小: %zu字节\n", 
                           small_sample->sequence_number, small_sample->timestamp_ns, sizeof(small_sample->payload));
                    
                    // 打印payload的前几个字符作为示例
                    printf("\tPayload前10字节: ");
                    for (int j = 0; j < 10 && j < sizeof(small_sample->payload); j++) {
                        printf("%c", small_sample->payload[j]);
                    }
                    printf("\n");
                }
                else
                {
                    printf("无效的小包数据\n");
                }
            }

            smallPacketDataReader_return_loan(small_reader, &small_sample_seq, &info_seq);
        }
        smallPacketSeq_finalize(&small_sample_seq);
        DDS_SampleInfoSeq_finalize(&info_seq);
    }

    // 尝试读取largePacket数据
    if (large_reader != NULL) {
        struct DDS_SampleInfoSeq large_info_seq = DDS_SEQUENCE_INITIALIZER;
        
        retcode = largePacketDataReader_take(large_reader, 
           &large_sample_seq, &large_info_seq, TAKE_MAX_SAMPLES,
           DDS_ANY_SAMPLE_STATE, DDS_ANY_VIEW_STATE, DDS_ANY_INSTANCE_STATE);

        if (retcode != DDS_RETCODE_OK)
        {
            printf("failed to take large packet data: %d\n", retcode);
        } else {
            /* 打印每个有效的大包样本 */
            for (i = 0; i < largePacketSeq_get_length(&large_sample_seq); ++i)
            {
                sample_info = DDS_SampleInfoSeq_get_reference(&large_info_seq, i);

                if (sample_info->valid_data)
                {
                    large_sample = largePacketSeq_get_reference(&large_sample_seq, i);
                    printf("\n收到大包数据:\n\t序列号: %lld\n\t时间戳: %lld\n\t数据大小: %zu字节\n", 
                           large_sample->sequence_number, large_sample->timestamp_ns, sizeof(large_sample->payload));
                    
                    // 打印payload的前几个字符作为示例
                    printf("\tPayload前10字节: ");
                    for (int j = 0; j < 10 && j < sizeof(large_sample->payload); j++) {
                        printf("%c", large_sample->payload[j]);
                    }
                    printf("\n");
                }
                else
                {
                    printf("无效的大包数据\n");
                }
            }

            largePacketDataReader_return_loan(large_reader, &large_sample_seq, &large_info_seq);
        }
        largePacketSeq_finalize(&large_sample_seq);
        DDS_SampleInfoSeq_finalize(&large_info_seq);
    }
}

void
DataTypeSubscriber_on_subscription_matched(void *listener_data,
                                             DDS_DataReader * reader,
                                             const struct
                                             DDS_SubscriptionMatchedStatus
                                             *status)
{
    if (status->current_count > 0)
    {
        printf("匹配到发布者\n");
    }
    else
    {
        printf("发布者断开连接\n");
    }
}

void 
smallPacketSubscriber_filter_sample(smallPacket *sample,
                                   DDS_Boolean *drop_sample)
{
    // 简单的过滤逻辑：根据序列号决定是否丢弃
    *drop_sample = (sample->sequence_number % 2 == 0) ? DDS_BOOLEAN_TRUE : DDS_BOOLEAN_FALSE;
}

void 
largePacketSubscriber_filter_sample(largePacket *sample,
                                   DDS_Boolean *drop_sample)
{
    // 简单的过滤逻辑：根据序列号决定是否丢弃
    *drop_sample = (sample->sequence_number % 3 == 0) ? DDS_BOOLEAN_TRUE : DDS_BOOLEAN_FALSE;
}

DDS_Boolean
DataTypeSubscriber_on_before_sample_deserialize(
        void *listener_data,
        DDS_DataReader *reader,
        struct NDDS_Type_Plugin *plugin,
        struct CDR_Stream_t *stream,
        DDS_Boolean *dropped)
{
    (void)listener_data;  // 避免未使用参数警告
    (void)reader;         // 避免未使用参数警告
    
    DDS_Boolean result = DDS_BOOLEAN_FALSE;
    smallPacket *small_sample = NULL;
    largePacket *large_sample = NULL;

    // 尝试创建smallPacket
    small_sample = smallPacket_create();
    if (small_sample != NULL)
    {
        /* CDR封装头 */
        if (!CDR_Stream_deserialize_header(stream))
        {
            printf("failed deserialize header for small packet\n");
            goto done;
        }

        /* 用户数据 */
        if (!plugin->deserialize_data(stream, small_sample, NULL))
        {
            printf("failed deserialize small packet data\n");
            goto done;
        }

        /* 过滤样本 */
        smallPacketSubscriber_filter_sample(small_sample, dropped);

        if (*dropped)
        {
            printf("\n小包样本在反序列化前被过滤\n\t丢弃 - 序列号: %lld\n",
                   small_sample->sequence_number);
        }

        result = DDS_BOOLEAN_TRUE;
    }
    else
    {
        // 尝试创建largePacket
        large_sample = largePacket_create();
        if (large_sample != NULL)
        {
            /* CDR封装头 */
            if (!CDR_Stream_deserialize_header(stream))
            {
                printf("failed deserialize header for large packet\n");
                goto done;
            }

            /* 用户数据 */
            if (!plugin->deserialize_data(stream, large_sample, NULL))
            {
                printf("failed deserialize large packet data\n");
                goto done;
            }

            /* 过滤样本 */
            largePacketSubscriber_filter_sample(large_sample, dropped);

            if (*dropped)
            {
                printf("\n大包样本在反序列化前被过滤\n\t丢弃 - 序列号: %lld\n",
                       large_sample->sequence_number);
            }

            result = DDS_BOOLEAN_TRUE;
        }
  }

done:
    if (small_sample != NULL)
    {
        smallPacket_delete(small_sample);
    }
    if (large_sample != NULL)
    {
        largePacket_delete(large_sample);
    }
    return result;
}

DDS_Boolean
DataTypeSubscriber_on_before_sample_commit(
        void *listener_data,
        DDS_DataReader *reader,
        const void *const sample,
        const struct DDS_SampleInfo *const sample_info,
        DDS_Boolean *dropped)
{
    (void)listener_data;  // 避免未使用参数警告
    (void)reader;         // 避免未使用参数警告
    (void)sample_info;    // 避免未使用参数警告
    
    // 尝试转换为smallPacket
    smallPacket *small_hw_sample = (smallPacket *)sample; 
    largePacket *large_hw_sample = (largePacket *)sample;

    // 根据数据类型进行过滤
    if (small_hw_sample != NULL) {
        smallPacketSubscriber_filter_sample(small_hw_sample, dropped);
        if (*dropped)
        {
            printf("\n小包样本在提交前被过滤\n\t丢弃 - 序列号: %lld\n",
                   small_hw_sample->sequence_number);
        }
    } else if (large_hw_sample != NULL) {
        largePacketSubscriber_filter_sample(large_hw_sample, dropped);
        if (*dropped)
        {
            printf("\n大包样本在提交前被过滤\n\t丢弃 - 序列号: %lld\n",
                   large_hw_sample->sequence_number);
        }
    }

    return DDS_BOOLEAN_TRUE;
}


int
subscriber_main_w_args(DDS_Long domain_id, char *udp_intf, char *peer, DDS_Long sleep_time,
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
    
    // 为smallPacket和largePacket创建单独的topic
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

    // 创建largePacket的topic
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
    dr_qos.resource_limits.max_samples = 32;
    dr_qos.resource_limits.max_samples_per_instance = 32;
    dr_qos.resource_limits.max_instances = 1;
    dr_qos.history.depth = 32;

    // 创建smallPacket的DataReader
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

    // 为largePacket创建单独的QoS
    struct DDS_DataReaderQos large_dr_qos = DDS_DataReaderQos_INITIALIZER;
    large_dr_qos.reliability.kind = DDS_RELIABLE_RELIABILITY_QOS;
    large_dr_qos.protocol.rtps_object_id = 201;  // 使用不同的object_id
    large_dr_qos.resource_limits.max_samples = 32;
    large_dr_qos.resource_limits.max_samples_per_instance = 32;
    large_dr_qos.resource_limits.max_instances = 1;
    large_dr_qos.history.depth = 32;

    // 创建largePacket的DataReader
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

    // 暂时注释掉远程发布断言，因为发布者可能还没有运行
    printf("Skipping remote publication assertions for now...\n");
    /*
    rem_publication_data.key.value[DDS_BUILTIN_TOPIC_KEY_OBJECT_ID] = 100;
    rem_publication_data.topic_name = DDS_String_dup("smallPacketTopic");
    rem_publication_data.type_name = DDS_String_dup("smallPacket");

    rem_publication_data.reliability.kind = DDS_RELIABLE_RELIABILITY_QOS;

    retcode = DPSE_RemotePublication_assert(
       application->participant,
       "publisher",
       &rem_publication_data,
       smallPacket_get_key_kind(smallPacketTypePlugin_get(), NULL));

    if (retcode != DDS_RETCODE_OK) 
    {
        printf("failed to assert remote publication for small packet\n");
        goto done;
    }

    // 为largePacket也创建远程发布断言
    DDS_PublicationBuiltinTopicData_finalize(&rem_publication_data);
    // 重新初始化结构体
    memset(&rem_publication_data, 0, sizeof(rem_publication_data));
    rem_publication_data.key.value[DDS_BUILTIN_TOPIC_KEY_OBJECT_ID] = 101;
    rem_publication_data.topic_name = DDS_String_dup("largePacketTopic");
    rem_publication_data.type_name = DDS_String_dup("largePacket");
    rem_publication_data.reliability.kind = DDS_RELIABLE_RELIABILITY_QOS;

    retcode = DPSE_RemotePublication_assert(
       application->participant,
       "publisher",
       &rem_publication_data,
       largePacket_get_key_kind(largePacketTypePlugin_get(), NULL));

    if (retcode != DDS_RETCODE_OK) 
    {
        printf("failed to assert remote publication for large packet\n");
        goto done;
    }
    */

    retcode = Application_enable(application);
    if (retcode != DDS_RETCODE_OK) 
    {
        printf("failed to enable application\n");
        goto done;
    }

    if (application->count != 0) 
    {
        printf("Running for %d seconds, press Ctrl-C to exit\n", application->count);
        OSAPI_Thread_sleep(application->count * 1000);
    } 
    else 
    {
        int sleep_loop_count =  (24 * 60 * 60) / 2000;
        int sleep_loop_left = (24 * 60 * 60) % 2000;

        printf("Running for 24 hours, press Ctrl-C to exit\n");

        while (sleep_loop_count)
        {
            OSAPI_Thread_sleep(2000  * 1000);
            --sleep_loop_count;
        }

        OSAPI_Thread_sleep(sleep_loop_left * 1000);
    }

    Application_delete(application);

    DDS_DataReaderQos_finalize(&dr_qos);
    DDS_DataReaderQos_finalize(&large_dr_qos);

done:
    return 0;
}

#if !(defined(RTI_VXWORKS) && !defined(__RTP__))
int
main(int argc,char **argv)
{
    DDS_Long i = 0;
    DDS_Long domain_id = 0;
    char *peer = NULL;
    char *udp_intf = NULL;
    DDS_Long throughput = 0; 
    DDS_Long delay = 0;
    DDS_Long small_packet = 0;
    DDS_Long large_packet = 0;
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
        else if (!strcmp(argv[i],"-throughput"))
        {
          ++i;
          if (i == argc)
          {
              printf("-throughput <throughput>\n");
              return -1;
          }
          throughput = strtol(argv[i], NULL, 0);
        }
        else if (!strcmp(argv[i], "-delay"))
        {
            ++i;
            if (i == argc)
            {
                printf("-delay <delay>\n");
                return -1;
            }
            delay = strtol(argv[i], NULL, 0);
        }
        else if (!strcmp(argv[i], "-small_packet"))
        {
            ++i;
            if (i == argc)
            {
                printf("-small_packet <size>\n");
                return -1;
            }
            small_packet = strtol(argv[i], NULL, 0);
        }
        else if (!strcmp(argv[i], "-large_packet"))
        {
            ++i;
            if (i == argc)
            {
                printf("-large_packet <size>\n");
                return -1;
            }
            large_packet = strtol(argv[i], NULL, 0);
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
int
subscriber_main(void)
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
