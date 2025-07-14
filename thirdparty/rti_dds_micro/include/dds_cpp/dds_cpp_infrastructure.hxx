/*

 (c) Copyright, Real-Time Innovations, 2013-2015.
 All rights reserved.

 No duplications, whole or partial, manual or electronic, may be made
 without express written permission.  Any such copies, or
 revisions thereof, must display this notice unaltered.
 This code contains trade secrets of Real-Time Innovations, Inc.


modification history
--------------------
19may2015,as  MICRO-1193 Refactoring of Sequence API levels
16may2014,as  MICRO-794 Remove C++ TODO and commented out code
08nov2013,as  MICRO-681 Complete implementation of WaitSets
              and support for StatusConditions
19jul2013,as  Major C++ update
11jan2013,eh  Created. 
===================================================================== */

#ifndef dds_cpp_infrastructure_hxx
#define dds_cpp_infrastructure_hxx

/*i @file
  @ingroup DDSInfrastructureModule

  @brief Defines the \dds infrastructure package
*/

#ifndef dds_cpp_dll_hxx
  #include "dds_cpp/dds_cpp_dll.hxx"
#endif

#ifndef dds_c_infrastructure_h
  #include "dds_c/dds_c_infrastructure.h"
#endif


/* ================================================================= */
/*                 Listeners                                         */
/* ================================================================= */

/*e \dref_Listener
 */
class DDSCPPDllExport DDSListener 
{

  protected:
    DDSListener() { }
    virtual ~DDSListener() { }
};

/* ================================================================= */
/*                         Conditions                                */
/* ================================================================= */
class DDSWaitSet;
class DDSCondition;

class DDSCPPDllExport DDSConditionSeq
{
  friend class DDSWaitSet;

  public:
    RTI_INT32 maximum() const;
    bool maximum(RTI_INT32 new_max);
    RTI_INT32 length() const;
    bool length(RTI_INT32 new_length);
    DDSCondition** get_reference(RTI_INT32 i);
    DDSConditionSeq();
    ~DDSConditionSeq();
#ifdef RTI_CERT
  private:
#else
  public:
#endif
    bool copy(const DDSConditionSeq& other);
    bool is_equal(const DDSConditionSeq& other) const;
    bool operator==(const DDSConditionSeq& other) const;
    bool operator!=(const DDSConditionSeq& other) const;
    DDSCondition *const & operator[](RTI_INT32 i) const;
    DDSCondition*& operator[](RTI_INT32 i);
    DDSConditionSeq& operator=(const DDSConditionSeq&);
    DDSConditionSeq(const DDSConditionSeq&);
    bool ensure_length(RTI_INT32 len, RTI_INT32 max);

  private:
    DDS_ConditionSeq _c_seq;
};

class DDSCPPDllExport DDSCondition
{
  friend class DDSWaitSet;

  public:

    virtual bool get_trigger_value();

  protected:
    DDSCondition(DDS_Condition *c_cond);

    virtual ~DDSCondition();

    DDS_Condition *_c_cond;

};


/* -----------------------------------------------------------------*/

class DDSCPPDllExport DDSGuardCondition : public DDSCondition {    
  public: 
    
    DDS_ReturnCode_t set_trigger_value(bool value);
    
  public:

    DDSGuardCondition();
    
    virtual ~DDSGuardCondition();
};


/* -----------------------------------------------------------------*/
class DDSEntity;
class DDSCPPDllExport DDSStatusCondition : public DDSCondition {
  friend class DDSEntity;

  public:
    
    DDS_StatusMask get_enabled_statuses();
    
    DDS_ReturnCode_t set_enabled_statuses(DDS_StatusMask mask);

    DDSEntity* get_entity();

  protected:
    DDSStatusCondition(DDS_StatusCondition *c_cond);
    virtual ~DDSStatusCondition();
};

/* ----------------------------------------------------------------- */

class DDSCPPDllExport DDSWaitSet {
  public:

    DDS_ReturnCode_t wait(DDSConditionSeq& active_conditions,
                          const DDS_Duration_t& timeout);

    DDS_ReturnCode_t attach_condition(DDSCondition* cond);

    DDS_ReturnCode_t detach_condition(DDSCondition* cond);

    DDS_ReturnCode_t get_conditions(DDSConditionSeq& attached_conditions);

  public:

    virtual ~DDSWaitSet();

    DDSWaitSet();

  private:

    /*i
      @brief The underlying implementation object owned by this class.
     */
    DDS_WaitSet* _c_impl;
};

/* ================================================================= */
/*                      DDSEntity                                       */
/* ================================================================= */ 

/* ----------------------------------------------------------------- */
/*e \dref_Entity
 */
class DDSCPPDllExport DDSEntity
{

  public: 
    
    /*e \dref_Entity_enable
     */
    DDS_ReturnCode_t enable();
    
    /*e \dref_Entity_get_statuscondition
     */
    DDSStatusCondition *get_statuscondition();
        
    /*e \dref_Entity_get_status_changes
     */
    DDS_StatusMask get_status_changes();
    
    /*i
     * Return a reference to the underlying C entity.
     */
    DDS_Entity* get_c_entity();

  protected:

    DDSEntity(DDS_Entity *c_entity);
    virtual ~DDSEntity();

    DDS_Entity *_c_entity;
};  


#endif
