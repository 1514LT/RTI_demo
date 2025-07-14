/*
WARNING: THIS FILE IS AUTO-GENERATED. DO NOT MODIFY.

This file was generated from DataType.idl using "rtiddsgen".
The rtiddsgen tool is part of the RTI Data Distribution Service distribution.
For more information, type 'rtiddsgen -help' at a command shell
or consult the RTI Data Distribution Service manual.
*/

#include "DataTypeSupport.h"

/*** SOURCE_BEGIN ***/
/* =========================================================================== */

/* Requires */
#define TTYPENAME   smallPacketTYPENAME

/* 
smallPacketDataWriter (DDS_DataWriter)   
*/

/* Defines */
#define TDataWriter smallPacketDataWriter
#define TData       smallPacket

#include "dds_c/dds_c_tdatawriter_gen.h"

#undef TDataWriter
#undef TData

/* =========================================================================== */
/* 
smallPacketDataReader (DDS_DataReader)   
*/

/* Defines */
#define TDataReader smallPacketDataReader
#define TDataSeq    smallPacketSeq
#define TData       smallPacket
#include "dds_c/dds_c_tdatareader_gen.h"
#undef TDataReader
#undef TDataSeq
#undef TData

DDS_ReturnCode_t
smallPacketTypeSupport_register_type(
    DDS_DomainParticipant* participant,
    const char* type_name)
{
    DDS_ReturnCode_t retcode = DDS_RETCODE_ERROR;

    if (participant == NULL) 
    {
        goto done;
    }

    if (type_name == NULL) 
    {
        type_name = smallPacketTypePlugin_get_default_type_name();
        if (type_name == NULL)
        {
            goto done;
        }
    }

    retcode = DDS_DomainParticipant_register_type(
        participant,
        type_name,
        smallPacketTypePlugin_get());

    if (retcode != DDS_RETCODE_OK)
    {
        goto done;
    }

    retcode = DDS_RETCODE_OK;

    done:

    return retcode;
}

DDS_ReturnCode_t
smallPacketTypeSupport_unregister_type(
    DDS_DomainParticipant* participant,
    const char* type_name)
{
    DDS_ReturnCode_t retcode = DDS_RETCODE_ERROR;

    if (participant == NULL) 
    {
        goto done;
    }

    if (type_name == NULL) 
    {
        type_name = smallPacketTypePlugin_get_default_type_name();
        if (type_name == NULL)
        {
            goto done;
        }
    }

    if (smallPacketTypePlugin_get() !=
    DDS_DomainParticipant_unregister_type(participant,type_name))
    {
        goto done;
    }

    retcode = DDS_RETCODE_OK;

    done:

    return retcode;
}

#undef TTYPENAME

/* =========================================================================== */

/* Requires */
#define TTYPENAME   largePacketTYPENAME

/* 
largePacketDataWriter (DDS_DataWriter)   
*/

/* Defines */
#define TDataWriter largePacketDataWriter
#define TData       largePacket

#include "dds_c/dds_c_tdatawriter_gen.h"

#undef TDataWriter
#undef TData

/* =========================================================================== */
/* 
largePacketDataReader (DDS_DataReader)   
*/

/* Defines */
#define TDataReader largePacketDataReader
#define TDataSeq    largePacketSeq
#define TData       largePacket
#include "dds_c/dds_c_tdatareader_gen.h"
#undef TDataReader
#undef TDataSeq
#undef TData

DDS_ReturnCode_t
largePacketTypeSupport_register_type(
    DDS_DomainParticipant* participant,
    const char* type_name)
{
    DDS_ReturnCode_t retcode = DDS_RETCODE_ERROR;

    if (participant == NULL) 
    {
        goto done;
    }

    if (type_name == NULL) 
    {
        type_name = largePacketTypePlugin_get_default_type_name();
        if (type_name == NULL)
        {
            goto done;
        }
    }

    retcode = DDS_DomainParticipant_register_type(
        participant,
        type_name,
        largePacketTypePlugin_get());

    if (retcode != DDS_RETCODE_OK)
    {
        goto done;
    }

    retcode = DDS_RETCODE_OK;

    done:

    return retcode;
}

DDS_ReturnCode_t
largePacketTypeSupport_unregister_type(
    DDS_DomainParticipant* participant,
    const char* type_name)
{
    DDS_ReturnCode_t retcode = DDS_RETCODE_ERROR;

    if (participant == NULL) 
    {
        goto done;
    }

    if (type_name == NULL) 
    {
        type_name = largePacketTypePlugin_get_default_type_name();
        if (type_name == NULL)
        {
            goto done;
        }
    }

    if (largePacketTypePlugin_get() !=
    DDS_DomainParticipant_unregister_type(participant,type_name))
    {
        goto done;
    }

    retcode = DDS_RETCODE_OK;

    done:

    return retcode;
}

#undef TTYPENAME

