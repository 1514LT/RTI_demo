/*
 * FILE: cdr_stream.h - CDR stream API
 *
 * (c) Copyright, Real-Time Innovations, 2012-2015.
 * All rights reserved.
 *
 * No duplications, whole or partial, manual or electronic, may be made
 * without express written permission.  Any such copies, or
 * revisions thereof, must display this notice unaltered.
 * This code contains trade secrets of Real-Time Innovations, Inc.
 *
 * Modification History
 * -------------------- 
 * 23feb2015,eh   MICRO-1075: remove and replace macros
 * 03jun2013,kaj  MICRO-193 (verification cdrs != NULL made resp. of caller)
 * 24mar2012,kaj  Written
 */
/*ci
 * \file 
 * \defgroup CDRStreamClass CDR Stream 
 * 
 * \brief CDR Stream  
 *  
 * \details 
 * An abstraction for serializing and deserializing CDR types to and from a 
 * buffer.  Enforces alignment and endianness. 
 *  
 */
/*ci \addtogroup CDRStreamClass
 *   @{
 */
#ifndef cdr_stream_h
#define cdr_stream_h

#ifndef osapi_types_h
#include "osapi/osapi_types.h"
#endif

#ifndef cdr_dll_h
#include "cdr/cdr_dll.h"
#endif

#ifdef __cplusplus
extern "C"
{
#endif

#define CDR_MAX_ALIGNMENT     8

/*i \ingroup RTICdrStreamModule
  Big endian in CDR.
*/
#define RTI_CDR_ENDIAN_BIG (0)

/*i \ingroup RTICdrStreamModule
  Little endian in CDR
*/
#define RTI_CDR_ENDIAN_LITTLE (1)


#define CDR_BYTESWAP_INVALID  -1

/* -------------------------------------------------------------------------- */
/*ci \brief Abstraction for serializing to and deserializing from a buffer */
struct CDR_Stream_t
{
    /*ci \brief Allocated buffer */
    char *real_buff;           

    /*ci \brief Buffer aligned on 8-byte boundary */
    char *buffer;            

    /*ci \brief Address used for alignment calculations */
    char *align_base;    
    
    /*ci \brief Current location in buffer */        
    char *buff_ptr;      
    
    /*ci \brief Length of the aligned buffer */        
    RTI_UINT32 length;   
    
    /*ci \brief Whether to byte swap during serialization/deserialization to 
     *   be consistent with stream's endianness
     */
    RTI_BOOL need_byte_swap;

    /*ci \brief Endianness of stream, either RTI_CDR_ENDIAN_LITTLE or 
     * RTI_CDR_ENDIAN_BIG
     */
    RTI_UINT16 endian;
};

/*ci \brief Set stream to specified alignment 
 * 
 * \param[inout] cdrs Stream to set, must be valid pointer 
 * \param[in] align Alignment to set 
 */
CDRDllExport void
CDR_Stream_align(struct CDR_Stream_t *stream, 
                 RTI_UINT8 alignment);


#ifndef RTI_CERT
/*ci 
 * \brief 
 * Free resources of a stream 
 *  
 * \param[in] cdrs Stream to free 
 *   
 */
CDRDllExport void
CDR_Stream_free(struct CDR_Stream_t *cdrs);
#endif

/*ci 
 * \brief 
 * Allocate and return a new stream 
 *  
 * \param[in] buffsize Size in bytes of stream buffer 
 *  
 * \return Pointer to allocated stream, is NULL if allocation failed 
 */
MUST_CHECK_RETURN CDRDllExport struct CDR_Stream_t*
CDR_Stream_alloc(RTI_UINT32 buffsize);

/*ci 
 * \brief 
 * Reset stream pointer to start of buffer 
 *  
 * \param[in] cdrs Stream to reset 
 *   
 */
CDRDllExport void
CDR_Stream_reset(struct CDR_Stream_t *cdrs);

/*ci
 * \brief 
 * Set byte swap state of stream 
 *  
 * \param[in] cdrs Stream to set 
 * \param[in] littleEndian Flag to set stream for little endian byte order 
 *  
 */
CDRDllExport void
CDR_Stream_byteswap_set(struct CDR_Stream_t *cdrs, RTI_BOOL littleEndian);

/*e \dref_CDR_Stream_get_current_position_offset
 */
MUST_CHECK_RETURN CDRDllExport RTI_UINT32
CDR_Stream_get_current_position_offset(struct CDR_Stream_t *cdrs);

/*e \dref_CDR_Stream_set_current_position_offset
 */
MUST_CHECK_RETURN CDRDllExport RTI_BOOL
CDR_Stream_set_current_position_offset(struct CDR_Stream_t *cdrs,RTI_UINT32 num);

/*e \dref_CDR_Stream_increment_current_position
 */
MUST_CHECK_RETURN CDRDllExport RTI_BOOL
CDR_Stream_increment_current_position(struct CDR_Stream_t *me,RTI_INT32 amount);

/*ci 
 * \brief 
 * Assign buffer to stream 
 *  
 * \param[in] me Stream 
 * \param[in] buf Pointer of new stream buffer 
 * \param[in] length Length in bytes of new stream buffer 
 *  
 * \return RTI_TRUE on success with new stream buffer assigned.  RTI_FALSE on 
 * failure.
 */
MUST_CHECK_RETURN CDRDllExport RTI_BOOL
CDR_Stream_set_buffer(struct CDR_Stream_t *me,char *buf, RTI_UINT32 length);


/*e \dref_CDR_Stream_check_size
 */
MUST_CHECK_RETURN CDRDllExport RTI_BOOL
CDR_Stream_check_size(struct CDR_Stream_t *me, RTI_UINT32 size); 

/*e \dref_CDR_Stream_get_current_position_ptr
 */
MUST_CHECK_RETURN CDRDllExport char*
CDR_Stream_get_current_position_ptr(struct CDR_Stream_t *me);

/*ci
 * \brief
 * Return the size of the encapsulation header based on the logical size of a
 * stream.
 *
 * \param[in] size Current size of a stream
 *
 * \return Return the size of the encapsulation header
 */
MUST_CHECK_RETURN CDRDllExport RTI_UINT32
CDR_Stream_get_encapsulation_size(RTI_UINT32 size);

/*e \dref_CDR_Stream_is_byte_swapped
 */
MUST_CHECK_RETURN CDRDllExport RTI_BOOL
CDR_Stream_is_byte_swapped(struct CDR_Stream_t *me);

#ifdef __cplusplus
} /* extern "C" */
#endif


#endif /* cdr_stream_h */

/*ci @} */
