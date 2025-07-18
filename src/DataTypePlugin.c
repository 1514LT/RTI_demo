/*
WARNING: THIS FILE IS AUTO-GENERATED. DO NOT MODIFY.

This file was generated from DataType.idl using "rtiddsgen".
The rtiddsgen tool is part of the RTI Data Distribution Service distribution.
For more information, type 'rtiddsgen -help' at a command shell
or consult the RTI Data Distribution Service manual.
*/

#include "DataType.h"
#include "DataTypePlugin.h"

/*** SOURCE_BEGIN ***/
#define UNUSED_ARG(x) (void)(x)

/* --------------------------------------------------------------------------
(De)Serialize functions:
* -------------------------------------------------------------------------- */
RTI_BOOL 
smallPacket_cdr_serialize(
    struct CDR_Stream_t *stream, const void *void_sample, void *param)
{
    smallPacket *sample = (smallPacket *)void_sample;

    if ((stream == NULL) || (void_sample == NULL))
    {
        return RTI_FALSE;
    }

    UNUSED_ARG(param);

    if (!CDR_Stream_serialize_long_long(
        stream, &sample->sequence_number)) {
        return RTI_FALSE;
    }  
    if (!CDR_Stream_serialize_long_long(
        stream, &sample->timestamp_ns)) {
        return RTI_FALSE;
    }  
    if (!CDR_Stream_serialize_primitive_array(
        stream,
        (void*)sample->payload,
        (48),
        CDR_CHAR_TYPE)) {
        return RTI_FALSE;
    }

    return RTI_TRUE;
}

RTI_BOOL 
smallPacket_cdr_deserialize(
    struct CDR_Stream_t *stream, void *void_sample, void *param)
{
    smallPacket *sample = (smallPacket *)void_sample;

    if ((stream == NULL) || (void_sample == NULL))
    {
        return RTI_FALSE;
    }

    UNUSED_ARG(param);

    if (!CDR_Stream_deserialize_long_long(
        stream, &sample->sequence_number)) {
        return RTI_FALSE;
    }  
    if (!CDR_Stream_deserialize_long_long(
        stream, &sample->timestamp_ns)) {
        return RTI_FALSE;
    }  
    if (!CDR_Stream_deserialize_primitive_array(
        stream,
        (void*)sample->payload,
        (48),
        CDR_CHAR_TYPE)) {
        return RTI_FALSE;
    }

    return RTI_TRUE;

}

RTI_UINT32
smallPacket_get_serialized_sample_max_size(
    struct NDDS_Type_Plugin *plugin,
    RTI_UINT32 current_alignment,
    void *param)
{
    RTI_UINT32 initial_alignment = current_alignment;

    UNUSED_ARG(plugin);
    UNUSED_ARG(param);
    current_alignment += CDR_get_max_size_serialized_long_long(
        current_alignment);

    current_alignment += CDR_get_max_size_serialized_long_long(
        current_alignment);

    current_alignment += CDR_get_max_size_serialized_primitive_array(
        current_alignment, (48), CDR_CHAR_TYPE);

    return  current_alignment - initial_alignment;
}

/* --------------------------------------------------------------------------
*  Sample Support functions:
* -------------------------------------------------------------------------- */
RTI_BOOL
smallPacketPlugin_create_sample(
    struct NDDS_Type_Plugin *plugin, void **sample, void *param)
{
    UNUSED_ARG(plugin);
    UNUSED_ARG(param);
    *sample = (void *) smallPacket_create();
    return (sample != NULL);
}

RTI_BOOL
smallPacketPlugin_delete_sample(
    struct NDDS_Type_Plugin *plugin, void *sample, void *param)
{
    UNUSED_ARG(plugin);
    UNUSED_ARG(param);
    /* smallPacket_delete() is a void function
    which expects (sample != NULL). Since smallPacketPlugin_delete_sample
    is an internal function, sample is assumed to be a valid pointer */ 
    smallPacket_delete((smallPacket *) sample);
    return RTI_TRUE;
}

RTI_BOOL 
smallPacketPlugin_copy_sample(
    struct NDDS_Type_Plugin *plugin, void *dst, const void *src, void *param)
{
    UNUSED_ARG(plugin);
    UNUSED_ARG(param);
    return smallPacket_copy(
        (smallPacket *)dst,
        (const smallPacket *)src);
}
/* --------------------------------------------------------------------------
*  Type smallPacket Plugin Instantiation
* -------------------------------------------------------------------------- */

NDDSCDREncapsulation smallPacketEncapsulationKind[] =
{ {0,0} };

struct NDDS_Type_Plugin smallPacketTypePlugin =
{
    {0, 0},                     /* NDDS_Type_PluginVersion */
    NULL,                       /* DDS_TypeCode_t* */
    smallPacketEncapsulationKind,
    NDDS_TYPEPLUGIN_NO_KEY,     /* NDDS_TypePluginKeyKind */
    smallPacket_cdr_serialize,
    smallPacket_cdr_deserialize,
    smallPacket_get_serialized_sample_max_size,
    NULL, NULL, NULL,           /* key support functions*/
    smallPacketPlugin_create_sample,
    smallPacketPlugin_delete_sample,
    smallPacketPlugin_copy_sample,
    PluginHelper_get_key_kind,
    PluginHelper_instance_to_keyhash,
    NULL, NULL, NULL, NULL  /* endpoint wrappers not used in C */
};

/* --------------------------------------------------------------------------
*  Type smallPacket Plugin Methods
* -------------------------------------------------------------------------- */

struct NDDS_Type_Plugin *
smallPacketTypePlugin_get(void) 
{ 
    return &smallPacketTypePlugin;
} 

const char*
smallPacketTypePlugin_get_default_type_name(void) 
{ 
    return smallPacketTYPENAME;
} 

NDDS_TypePluginKeyKind 
smallPacket_get_key_kind(
    struct NDDS_Type_Plugin *plugin,
    void *param)
{
    UNUSED_ARG(param);
    UNUSED_ARG(plugin);
    return NDDS_TYPEPLUGIN_NO_KEY;
}

/* --------------------------------------------------------------------------
(De)Serialize functions:
* -------------------------------------------------------------------------- */
RTI_BOOL 
largePacket_cdr_serialize(
    struct CDR_Stream_t *stream, const void *void_sample, void *param)
{
    largePacket *sample = (largePacket *)void_sample;

    if ((stream == NULL) || (void_sample == NULL))
    {
        return RTI_FALSE;
    }

    UNUSED_ARG(param);

    if (!CDR_Stream_serialize_long_long(
        stream, &sample->sequence_number)) {
        return RTI_FALSE;
    }  
    if (!CDR_Stream_serialize_long_long(
        stream, &sample->timestamp_ns)) {
        return RTI_FALSE;
    }  
    if (!CDR_Stream_serialize_primitive_array(
        stream,
        (void*)sample->payload,
        (1048560),
        CDR_CHAR_TYPE)) {
        return RTI_FALSE;
    }

    return RTI_TRUE;
}

RTI_BOOL 
largePacket_cdr_deserialize(
    struct CDR_Stream_t *stream, void *void_sample, void *param)
{
    largePacket *sample = (largePacket *)void_sample;

    if ((stream == NULL) || (void_sample == NULL))
    {
        return RTI_FALSE;
    }

    UNUSED_ARG(param);

    if (!CDR_Stream_deserialize_long_long(
        stream, &sample->sequence_number)) {
        return RTI_FALSE;
    }  
    if (!CDR_Stream_deserialize_long_long(
        stream, &sample->timestamp_ns)) {
        return RTI_FALSE;
    }  
    if (!CDR_Stream_deserialize_primitive_array(
        stream,
        (void*)sample->payload,
        (1048560),
        CDR_CHAR_TYPE)) {
        return RTI_FALSE;
    }

    return RTI_TRUE;

}

RTI_UINT32
largePacket_get_serialized_sample_max_size(
    struct NDDS_Type_Plugin *plugin,
    RTI_UINT32 current_alignment,
    void *param)
{
    RTI_UINT32 initial_alignment = current_alignment;

    UNUSED_ARG(plugin);
    UNUSED_ARG(param);
    current_alignment += CDR_get_max_size_serialized_long_long(
        current_alignment);

    current_alignment += CDR_get_max_size_serialized_long_long(
        current_alignment);

    current_alignment += CDR_get_max_size_serialized_primitive_array(
        current_alignment, (1048560), CDR_CHAR_TYPE);

    return  current_alignment - initial_alignment;
}

/* --------------------------------------------------------------------------
*  Sample Support functions:
* -------------------------------------------------------------------------- */
RTI_BOOL
largePacketPlugin_create_sample(
    struct NDDS_Type_Plugin *plugin, void **sample, void *param)
{
    UNUSED_ARG(plugin);
    UNUSED_ARG(param);
    *sample = (void *) largePacket_create();
    return (sample != NULL);
}

RTI_BOOL
largePacketPlugin_delete_sample(
    struct NDDS_Type_Plugin *plugin, void *sample, void *param)
{
    UNUSED_ARG(plugin);
    UNUSED_ARG(param);
    /* largePacket_delete() is a void function
    which expects (sample != NULL). Since largePacketPlugin_delete_sample
    is an internal function, sample is assumed to be a valid pointer */ 
    largePacket_delete((largePacket *) sample);
    return RTI_TRUE;
}

RTI_BOOL 
largePacketPlugin_copy_sample(
    struct NDDS_Type_Plugin *plugin, void *dst, const void *src, void *param)
{
    UNUSED_ARG(plugin);
    UNUSED_ARG(param);
    return largePacket_copy(
        (largePacket *)dst,
        (const largePacket *)src);
}
/* --------------------------------------------------------------------------
*  Type largePacket Plugin Instantiation
* -------------------------------------------------------------------------- */

NDDSCDREncapsulation largePacketEncapsulationKind[] =
{ {0,0} };

struct NDDS_Type_Plugin largePacketTypePlugin =
{
    {0, 0},                     /* NDDS_Type_PluginVersion */
    NULL,                       /* DDS_TypeCode_t* */
    largePacketEncapsulationKind,
    NDDS_TYPEPLUGIN_NO_KEY,     /* NDDS_TypePluginKeyKind */
    largePacket_cdr_serialize,
    largePacket_cdr_deserialize,
    largePacket_get_serialized_sample_max_size,
    NULL, NULL, NULL,           /* key support functions*/
    largePacketPlugin_create_sample,
    largePacketPlugin_delete_sample,
    largePacketPlugin_copy_sample,
    PluginHelper_get_key_kind,
    PluginHelper_instance_to_keyhash,
    NULL, NULL, NULL, NULL  /* endpoint wrappers not used in C */
};

/* --------------------------------------------------------------------------
*  Type largePacket Plugin Methods
* -------------------------------------------------------------------------- */

struct NDDS_Type_Plugin *
largePacketTypePlugin_get(void) 
{ 
    return &largePacketTypePlugin;
} 

const char*
largePacketTypePlugin_get_default_type_name(void) 
{ 
    return largePacketTYPENAME;
} 

NDDS_TypePluginKeyKind 
largePacket_get_key_kind(
    struct NDDS_Type_Plugin *plugin,
    void *param)
{
    UNUSED_ARG(param);
    UNUSED_ARG(plugin);
    return NDDS_TYPEPLUGIN_NO_KEY;
}

