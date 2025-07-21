/*
WARNING: THIS FILE IS AUTO-GENERATED. DO NOT MODIFY.

This file was generated from DataType.idl using "rtiddsgen".
The rtiddsgen tool is part of the RTI Data Distribution Service distribution.
For more information, type 'rtiddsgen -help' at a command shell
or consult the RTI Data Distribution Service manual.
*/

#include "DataType.h"

#define UNUSED_ARG(x) (void)(x)

/*** SOURCE_BEGIN ***/

/* ========================================================================= */

const char *smallPacketTYPENAME = "smallPacket";

RTI_BOOL
smallPacket_initialize(smallPacket* sample)
{
    if (sample == NULL)
    {
        return RTI_FALSE;
    }

    CDR_Primitive_init_long_long(&sample->sequence_number);
    CDR_Primitive_init_long_long(&sample->timestamp_ns);
    CDR_Primitive_init_array(
        sample->payload, ((48)*CDR_CHAR_SIZE));
    return RTI_TRUE;
}

RTI_BOOL
smallPacket_finalize(smallPacket* sample)
{
    if (sample == NULL)
    {
        return RTI_FALSE;
    }

    return RTI_TRUE;
}

smallPacket *
smallPacket_create(void)
{
    smallPacket* sample;
    OSAPI_Heap_allocate_struct(&sample, smallPacket);
    if (sample != NULL) {
        if (!smallPacket_initialize(sample)) {
            OSAPI_Heap_free_struct(sample);
            sample = NULL;
        }
    }
    return sample;
}

void
smallPacket_delete(smallPacket*sample)
{
    if (sample != NULL) {
        /* smallPacket_finalize() always 
        returns RTI_TRUE when called with sample != NULL */
        smallPacket_finalize(sample);
        OSAPI_Heap_free_struct(sample);
    }
}

RTI_BOOL
smallPacket_copy(smallPacket* dst,const smallPacket* src)
{        
    if ((dst == NULL) || (src == NULL))
    {
        return RTI_FALSE;
    }
    CDR_Primitive_copy_long_long(&dst->sequence_number, &src->sequence_number);
    CDR_Primitive_copy_long_long(&dst->timestamp_ns, &src->timestamp_ns);
    CDR_Primitive_copy_array(dst->payload,src->payload,((48)*CDR_CHAR_SIZE));
    return RTI_TRUE;
}

/**
* <<IMPLEMENTATION>>
*
* Defines:  TSeq, T
*
* Configure and implement 'smallPacket' sequence class.
*/
#define REDA_SEQUENCE_USER_API
#define T smallPacket
#define TSeq smallPacketSeq
#define T_initialize smallPacket_initialize
#define T_finalize   smallPacket_finalize
#define T_copy       smallPacket_copy
#include "reda/reda_sequence_defn.h"
#undef T_copy
#undef T_finalize
#undef T_initialize

/* ========================================================================= */

const char *largePacketTYPENAME = "largePacket";

RTI_BOOL
largePacket_initialize(largePacket* sample)
{
    if (sample == NULL)
    {
        return RTI_FALSE;
    }

    CDR_Primitive_init_long_long(&sample->sequence_number);
    CDR_Primitive_init_long_long(&sample->timestamp_ns);
    CDR_Primitive_init_array(
        sample->payload, ((1484)*CDR_CHAR_SIZE));
    return RTI_TRUE;
}

RTI_BOOL
largePacket_finalize(largePacket* sample)
{
    if (sample == NULL)
    {
        return RTI_FALSE;
    }

    return RTI_TRUE;
}

largePacket *
largePacket_create(void)
{
    largePacket* sample;
    OSAPI_Heap_allocate_struct(&sample, largePacket);
    if (sample != NULL) {
        if (!largePacket_initialize(sample)) {
            OSAPI_Heap_free_struct(sample);
            sample = NULL;
        }
    }
    return sample;
}

void
largePacket_delete(largePacket*sample)
{
    if (sample != NULL) {
        /* largePacket_finalize() always 
        returns RTI_TRUE when called with sample != NULL */
        largePacket_finalize(sample);
        OSAPI_Heap_free_struct(sample);
    }
}

RTI_BOOL
largePacket_copy(largePacket* dst,const largePacket* src)
{        
    if ((dst == NULL) || (src == NULL))
    {
        return RTI_FALSE;
    }
    CDR_Primitive_copy_long_long(&dst->sequence_number, &src->sequence_number);
    CDR_Primitive_copy_long_long(&dst->timestamp_ns, &src->timestamp_ns);
    CDR_Primitive_copy_array(dst->payload,src->payload,((1484)*CDR_CHAR_SIZE));
    return RTI_TRUE;
}

/**
* <<IMPLEMENTATION>>
*
* Defines:  TSeq, T
*
* Configure and implement 'largePacket' sequence class.
*/
#define REDA_SEQUENCE_USER_API
#define T largePacket
#define TSeq largePacketSeq
#define T_initialize largePacket_initialize
#define T_finalize   largePacket_finalize
#define T_copy       largePacket_copy
#include "reda/reda_sequence_defn.h"
#undef T_copy
#undef T_finalize
#undef T_initialize

