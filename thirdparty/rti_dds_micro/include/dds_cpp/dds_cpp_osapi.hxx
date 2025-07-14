/* 

 (c) Copyright, Real-Time Innovations, 2006-2015.  All rights reserved.
 No duplications, whole or partial, manual or electronic, may be made
 without express written permission.  Any such copies, or
 revisions thereof, must display this notice unaltered.
 This code contains trade secrets of Real-Time Innovations, Inc.
*/
/*
modification history
--------------------
23jan2016,tk MICRO-1523 Use bool instead of RTI_BOOL
22as2015,as  Created
=========================================================================*/

#ifndef dds_cpp_osapi_hxx
#define dds_cpp_osapi_hxx

#ifndef dds_cpp_dll_hxx
#include "dds_cpp/dds_cpp_dll.hxx"
#endif
#ifndef dds_cpp_infrastructure_hxx
#include "dds_cpp/dds_cpp_infrastructure.hxx"
#endif
#ifndef osapi_log_h
#include "osapi/osapi_log.h"
#endif

#if OSAPI_ENABLE_LOG

class DDSCPPDllExport OSAPILog {
public:
    static bool initialize();
#ifndef RTI_CERT
    static bool finalize();
#endif /* RTI_CERT */
    static bool clear();
    static bool set_log_handler(OSAPI_LogHandler_T handler,void *param);
    static bool get_log_handler(OSAPI_LogHandler_T *handler,void **param);
#if OSAPI_ENABLE_TRACE
    static bool set_trace_handler(OSAPI_TraceHandler_T handler,void *param);
    static bool get_trace_handler(OSAPI_TraceHandler_T *handler,void **param);
#endif /* OSAPI_ENABLE_TRACE */
    static bool set_display_handler(OSAPI_LogDisplay_T handler,void *param);
    static bool get_display_handler(OSAPI_LogDisplay_T *handler,void **param);

};

#if OSAPI_ENABLE_TRACE
class DDSCPPDllExport OSAPITrace {
public:
    static void set_trace_mask(RTI_UINT32 mask);
};
#endif /* OSAPI_ENABLE_TRACE */

#endif /* OSAPI_ENABLE_LOG */

#endif /* dds_cpp_osapi_hxx */
