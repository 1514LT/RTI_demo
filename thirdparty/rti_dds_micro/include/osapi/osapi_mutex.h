/*
 * FILE: osapi_mutex.h - Definition of mutex interface
 *
 * Copyright 2012-2015 Real-Time Innovations, Inc.
 *
 * No duplications, whole or partial, manual or electronic, may be made
 * without express written permission.  Any such copies, or
 * revisions thereof, must display this notice unaltered.
 * This code contains trade secrets of Real-Time Innovations, Inc.
 *
 * Modification History
 * --------------------
 * 12mar2012,tk Written
 */
/*ce
 * \file 
 * \brief Definition of mutex interface
 */
#ifndef osapi_mutex_h
#define osapi_mutex_h

#ifndef osapi_dll_h
#include "osapi/osapi_dll.h"
#endif
#ifndef osapi_types_h
#include "osapi/osapi_types.h"
#endif

#ifdef __cplusplus
extern "C"
{
#endif

/*e \defgroup OSAPI_MutexClass OSAPI Mutex
    \ingroup OSAPIModule

    \brief Mutex API
 */

/*e \ingroup OSAPI_MutexClass
 *
 *  Abstract Mutex object
 */
struct OSAPI_Mutex;

/*e \ingroup OSAPI_MutexClass
 *  Abstract Mutex type
 */
typedef struct OSAPI_Mutex OSAPI_Mutex_T;

#ifndef RTI_CERT
/*e \ingroup OSAPI_MutexClass
 *
 * \brief Delete a mutex.
 *
 * \param [in] mutex Delete a mutex created with \ref OSAPI_Mutex_new.
 *
 * \return RTI_TRUE on success, RTI_FALSE on failure
 */
SHOULD_CHECK_RETURN OSAPIDllExport RTI_BOOL
OSAPI_Mutex_delete(OSAPI_Mutex_T *mutex);
#endif /* !RTI_CERT */

/*e \ingroup OSAPI_MutexClass
 *
 * \brief Create a mutex.
 *
 * \return Pointer to a mutex in a not taken condition. NULL on failure.
 *
 * \sa \ref OSAPI_Mutex_delete
 */
OSAPIDllExport OSAPI_Mutex_T*
OSAPI_Mutex_new(void);

/*e \ingroup OSAPI_MutexClass
 *
 * \brief Take a mutex.
 *
 * \details
 *
 * A mutex can only be taken if it is not currently already taken 
 * by another thread; it CAN be taken if is it already taken by 
 * the same thread. In order to release a mutex, it must be
 * given as many times a it has been taken. Note that `take` will 
 * block indefinitely.
 *
 * Note: the mutex is not required to support priority inversion;
 *       there is no protection against deadlocks or starvation.
 *
 * \param [in] mutex Take a mutex previously created with \ref OSAPI_Mutex_new.
 *
 * \return RTI_TRUE on success, RTI_FALSE on failure
 *
 * \sa \ref OSAPI_Mutex_give, \ref OSAPI_Mutex_new
 *
 */
MUST_CHECK_RETURN OSAPIDllExport RTI_BOOL
OSAPI_Mutex_take(OSAPI_Mutex_T *mutex);

/*e \ingroup OSAPI_MutexClass
 *
 * \brief Give a mutex.
 *
 * \details
 *
 * A mutex can only be given if it is owned by the calling thread. Furthermore,
 * it must be given as many times as it has been taken.
 *
 * \param [in] mutex Take a mutex previously created with \ref OSAPI_Mutex_new.
 *
 * \return RTI_TRUE on success, RTI_FALSE on failure
 *
 * \sa \ref OSAPI_Mutex_take, \ref OSAPI_Mutex_new
 *
 */
MUST_CHECK_RETURN OSAPIDllExport RTI_BOOL
OSAPI_Mutex_give(OSAPI_Mutex_T *mutex);

#ifdef __cplusplus
}                               /* extern "C" */
#endif


#endif /* osapi_mutex_h */
