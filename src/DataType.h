/*
WARNING: THIS FILE IS AUTO-GENERATED. DO NOT MODIFY.

This file was generated from DataType.idl using "rtiddsgen".
The rtiddsgen tool is part of the RTI Data Distribution Service distribution.
For more information, type 'rtiddsgen -help' at a command shell
or consult the RTI Data Distribution Service manual.
*/

#ifndef DataType_814104882_h
#define DataType_814104882_h

#ifndef rti_me_c_h
#include "rti_me_c.h"
#endif

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

typedef struct smallPacket

{

    CDR_LongLong sequence_number;
    CDR_LongLong timestamp_ns;
    CDR_Char payload[48];

} smallPacket ;

extern const char *smallPacketTYPENAME;

#define REDA_SEQUENCE_USER_API
#define T smallPacket
#define TSeq smallPacketSeq
#define REDA_SEQUENCE_EXCLUDE_C_METHODS
#include <reda/reda_sequence_decl.h>

#define REDA_SEQUENCE_USER_API
#define T smallPacket
#define TSeq smallPacketSeq
#define REDA_SEQUENCE_EXCLUDE_STRUCT
#include <reda/reda_sequence_decl.h>

NDDSUSERDllExport extern RTI_BOOL
smallPacket_initialize(smallPacket* sample);

NDDSUSERDllExport extern RTI_BOOL
smallPacket_finalize(smallPacket* sample);

NDDSUSERDllExport extern smallPacket*
smallPacket_create();

NDDSUSERDllExport extern void
smallPacket_delete(smallPacket* sample);

NDDSUSERDllExport extern RTI_BOOL
smallPacket_copy(smallPacket* dst, const smallPacket* src);

typedef struct largePacket

{

    CDR_LongLong sequence_number;
    CDR_LongLong timestamp_ns;
    CDR_Char payload[1484];

} largePacket ;

extern const char *largePacketTYPENAME;

#define REDA_SEQUENCE_USER_API
#define T largePacket
#define TSeq largePacketSeq
#define REDA_SEQUENCE_EXCLUDE_C_METHODS
#include <reda/reda_sequence_decl.h>

#define REDA_SEQUENCE_USER_API
#define T largePacket
#define TSeq largePacketSeq
#define REDA_SEQUENCE_EXCLUDE_STRUCT
#include <reda/reda_sequence_decl.h>

NDDSUSERDllExport extern RTI_BOOL
largePacket_initialize(largePacket* sample);

NDDSUSERDllExport extern RTI_BOOL
largePacket_finalize(largePacket* sample);

NDDSUSERDllExport extern largePacket*
largePacket_create();

NDDSUSERDllExport extern void
largePacket_delete(largePacket* sample);

NDDSUSERDllExport extern RTI_BOOL
largePacket_copy(largePacket* dst, const largePacket* src);

#ifdef NDDS_USER_DLL_EXPORT
#if (defined(RTI_WIN32) || defined(RTI_WINCE))
/* If the code is building on Windows, stop exporting symbols. */
#undef NDDSUSERDllExport
#define NDDSUSERDllExport
#endif
#endif

#endif /* DataType */

