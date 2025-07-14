#ifndef Application_h
#define Application_h

#include "rti_me_c.h"

struct Application {
    DDS_DomainParticipant *participant;
    char topic_name[255];
    char type_name[255];
    DDS_Long sleep_time;
    DDS_Long count;
    DDS_Topic *topic;
};

extern void 
Application_help(char *appname);

extern struct Application*
Application_create(const char *local_participant_name,
                  const char *remote_participant_name,
                  DDS_Long domain_id, char *udp_intf, char *peer, 
                  DDS_Long sleep_time, DDS_Long count);

extern DDS_ReturnCode_t
Application_enable(struct Application *application);

extern void
Application_delete(struct Application *application);

#endif
