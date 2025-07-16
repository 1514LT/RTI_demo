#ifndef BASE_H
#define BASE_H
#include <stdio.h>
#include <time.h>
#include <stdatomic.h>
// Remove math.h and use RTI alternatives
// #include <math.h>
#include "rti_me_c.h"
#include "osapi/osapi_system.h"
#include "osapi/osapi_time.h"
#include "osapi/osapi_log.h"

typedef long long ll;

extern ll get_current_timestamp_ms();
extern ll get_current_timestamp_us();
extern void timestamp_to_string(ll timestamp_ms, char *buffer, int buffer_size);

// Add RTI-compatible math functions
extern double rti_sqrt(double x);
extern double rti_abs(double x);
extern double rti_pow(double base, double exp);

#endif