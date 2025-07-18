/*
WARNING: THIS FILE IS AUTO-GENERATED. DO NOT MODIFY.

This file was generated from DataType.idl using "rtiddsgen".
The rtiddsgen tool is part of the RTI Data Distribution Service distribution.
For more information, type 'rtiddsgen -help' at a command shell
or consult the RTI Data Distribution Service manual.
*/

#ifndef DataTypePlugin_814104885_h
#define DataTypePlugin_814104885_h

#include "DataType.h"

#ifdef NDDS_USER_DLL_EXPORT
#if (defined(RTI_WIN32) || defined(RTI_WINCE))
/* If the code is building on Windows, start exporting symbols. */
#undef NDDSUSERDllExport
#define NDDSUSERDllExport __declspec(dllexport)
#endif
#else
#undef NDDSUSERDllExport
#define NDDSUSERDllExport
#endif

NDDSUSERDllExport extern struct NDDS_Type_Plugin*
smallPacketTypePlugin_get(void);
NDDSUSERDllExport extern const char*
smallPacketTypePlugin_get_default_type_name(void);
NDDSUSERDllExport extern NDDS_TypePluginKeyKind 
smallPacket_get_key_kind(
    struct NDDS_Type_Plugin *plugin,
    void *param);
/* --------------------------------------------------------------------------
Untyped interfaces to the typed sample management functions
* -------------------------------------------------------------------------- */
NDDSUSERDllExport extern RTI_BOOL
smallPacketPlugin_create_sample(
    struct NDDS_Type_Plugin *plugin, void **sample,void *param);

NDDSUSERDllExport extern RTI_BOOL 
smallPacketPlugin_delete_sample(
    struct NDDS_Type_Plugin *plugin, void *sample,void *param);

NDDSUSERDllExport extern RTI_BOOL 
smallPacketPlugin_copy_sample(
    struct NDDS_Type_Plugin *plugin, void *dst, const void *src, void *param);

/* --------------------------------------------------------------------------
(De)Serialize functions:
* -------------------------------------------------------------------------- */
NDDSUSERDllExport extern RTI_BOOL 
smallPacket_cdr_serialize(
    struct CDR_Stream_t *stream, const void *void_sample, void *param);

NDDSUSERDllExport extern RTI_BOOL 
smallPacket_cdr_deserialize(
    struct CDR_Stream_t *stream, void *void_sample, void *param);

NDDSUSERDllExport extern RTI_UINT32
smallPacket_get_serialized_sample_max_size(
    struct NDDS_Type_Plugin *plugin,
    RTI_UINT32 current_alignment,
    void *param);
/* Unkeyed type key serialization equivalent to sample serialization */
#define smallPacket_cdr_serialize_key smallPacket_cdr_serialize
#define smallPacket_cdr_deserialize_key smallPacket_cdr_deserialize
#define smallPacket_get_serialized_key_max_size smallPacket_get_serialized_sample_max_size

NDDSUSERDllExport extern struct NDDS_Type_Plugin*
largePacketTypePlugin_get(void);
NDDSUSERDllExport extern const char*
largePacketTypePlugin_get_default_type_name(void);
NDDSUSERDllExport extern NDDS_TypePluginKeyKind 
largePacket_get_key_kind(
    struct NDDS_Type_Plugin *plugin,
    void *param);
/* --------------------------------------------------------------------------
Untyped interfaces to the typed sample management functions
* -------------------------------------------------------------------------- */
NDDSUSERDllExport extern RTI_BOOL
largePacketPlugin_create_sample(
    struct NDDS_Type_Plugin *plugin, void **sample,void *param);

NDDSUSERDllExport extern RTI_BOOL 
largePacketPlugin_delete_sample(
    struct NDDS_Type_Plugin *plugin, void *sample,void *param);

NDDSUSERDllExport extern RTI_BOOL 
largePacketPlugin_copy_sample(
    struct NDDS_Type_Plugin *plugin, void *dst, const void *src, void *param);

/* --------------------------------------------------------------------------
(De)Serialize functions:
* -------------------------------------------------------------------------- */
NDDSUSERDllExport extern RTI_BOOL 
largePacket_cdr_serialize(
    struct CDR_Stream_t *stream, const void *void_sample, void *param);

NDDSUSERDllExport extern RTI_BOOL 
largePacket_cdr_deserialize(
    struct CDR_Stream_t *stream, void *void_sample, void *param);

NDDSUSERDllExport extern RTI_UINT32
largePacket_get_serialized_sample_max_size(
    struct NDDS_Type_Plugin *plugin,
    RTI_UINT32 current_alignment,
    void *param);
/* Unkeyed type key serialization equivalent to sample serialization */
#define largePacket_cdr_serialize_key largePacket_cdr_serialize
#define largePacket_cdr_deserialize_key largePacket_cdr_deserialize
#define largePacket_get_serialized_key_max_size largePacket_get_serialized_sample_max_size

#ifdef NDDS_USER_DLL_EXPORT
#if (defined(RTI_WIN32) || defined(RTI_WINCE))
/* If the code is building on Windows, stop exporting symbols. */
#undef NDDSUSERDllExport
#define NDDSUSERDllExport
#endif
#endif

#endif /* DataTypePlugin_814104885_h */

