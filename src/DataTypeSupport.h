/*
WARNING: THIS FILE IS AUTO-GENERATED. DO NOT MODIFY.

This file was generated from DataType.idl using "rtiddsgen".
The rtiddsgen tool is part of the RTI Data Distribution Service distribution.
For more information, type 'rtiddsgen -help' at a command shell
or consult the RTI Data Distribution Service manual.
*/

#ifndef DataTypeSupport_814104882_h
#define DataTypeSupport_814104882_h

#include <stdlib.h>

/* Uses */
#include "DataType.h"
/* Requires */
#include "DataTypePlugin.h"

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

/* ========================================================================== */
/**
Uses:     T
Defines:  TTypeSupport, TDataWriter, TDataReader*/

NDDSUSERDllExport extern DDS_ReturnCode_t
smallPacketTypeSupport_register_type(
    DDS_DomainParticipant* participant,
    const char* type_name);

NDDSUSERDllExport extern DDS_ReturnCode_t
smallPacketTypeSupport_unregister_type(
    DDS_DomainParticipant* participant,
    const char* type_name);

DDS_DATAWRITER_C(smallPacketDataWriter, smallPacket);

DDS_DATAREADER_C(smallPacketDataReader, smallPacketSeq, smallPacket);

/* ========================================================================== */
/**
Uses:     T
Defines:  TTypeSupport, TDataWriter, TDataReader*/

NDDSUSERDllExport extern DDS_ReturnCode_t
largePacketTypeSupport_register_type(
    DDS_DomainParticipant* participant,
    const char* type_name);

NDDSUSERDllExport extern DDS_ReturnCode_t
largePacketTypeSupport_unregister_type(
    DDS_DomainParticipant* participant,
    const char* type_name);

DDS_DATAWRITER_C(largePacketDataWriter, largePacket);

DDS_DATAREADER_C(largePacketDataReader, largePacketSeq, largePacket);

#ifdef NDDS_USER_DLL_EXPORT
#if (defined(RTI_WIN32) || defined(RTI_WINCE))
/* If the code is building on Windows, stop exporting symbols. */
#undef NDDSUSERDllExport
#define NDDSUSERDllExport
#endif
#endif

#endif  /* DataTypeSupport_814104882_h */

