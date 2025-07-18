#include "Base.h"

#ifndef LINUX
#endif
ll get_current_timestamp_ms()
{
  #ifndef LINUX
  int ticks = tickGet();
  // int newRate = 1000000;
  // if (sysClkRateSet(newRate) == ERROR) {
  // printf("Error in setting clock rate!\n");
  // return;
  // }
  int clk_rate = sysClkRateGet();
  return ((int64_t)ticks * 1000) / clk_rate;
  #else
  struct timeval tv;
  gettimeofday(&tv, NULL);
  return (ll)tv.tv_sec * 1000 + tv.tv_usec / 1000;
  #endif
}

ll get_current_timestamp_us()
{
  #ifndef LINUX
  int ticks = tickGet();
  int clk_rate = sysClkRateGet();
  return ((int64_t)ticks * 1000000) / clk_rate;
  #else
  struct timeval tv;
  gettimeofday(&tv, NULL);
  return (ll)tv.tv_sec * 1000000 + tv.tv_usec;
  #endif
}

void timestamp_to_string(ll timestamp_ms, char *buffer, int buffer_size)
{
  time_t seconds = (time_t)(timestamp_ms / 1000);
  int millis = (int)(timestamp_ms % 1000);

  struct tm *tm_info = localtime(&seconds);

  snprintf(buffer, buffer_size, "%d/%d/%d %02d:%02d:%02d.%03d",
            tm_info->tm_year + 1900,
            tm_info->tm_mon + 1,
            tm_info->tm_mday,
            tm_info->tm_hour,
            tm_info->tm_min,
            tm_info->tm_sec,
            millis);
}

// Add RTI-compatible sqrt implementation using Newton's method
double rti_sqrt(double x)
{
    if (x < 0.0) {
        return 0.0; // Handle negative input
    }
    
    if (x == 0.0 || x == 1.0) {
        return x;
    }
    
    double guess = x / 2.0;
    double epsilon = 1e-10; // Precision threshold
    
    // Newton's method: x_new = (x_old + n/x_old) / 2
    while (1) {
        double new_guess = (guess + x / guess) / 2.0;
        
        // Check for convergence
        double diff = new_guess - guess;
        if (diff < 0.0) diff = -diff; // Absolute value
        
        if (diff < epsilon) {
            return new_guess;
        }
        
        guess = new_guess;
    }
}

// Absolute value
double rti_abs(double x)
{
    return (x < 0.0) ? -x : x;
}

// Simple power function for integer exponents
double rti_pow(double base, double exp)
{
    if (exp == 0.0) return 1.0;
    if (exp == 1.0) return base;
    if (exp == 2.0) return base * base;
    
    // For other cases, use repeated multiplication
    double result = 1.0;
    RTI_INT32 int_exp = (RTI_INT32)exp;
    RTI_BOOL negative = (int_exp < 0);
    
    if (negative) int_exp = -int_exp;
    
    while (int_exp > 0) {
        if (int_exp & 1) {
            result *= base;
        }
        base *= base;
        int_exp >>= 1;
    }
    
    return negative ? (1.0 / result) : result;
}

// Fast square root using bit manipulation (Quake algorithm variant)
double rti_sqrt_fast(double x)
{
    if (x <= 0.0) {
        return 0.0;
    }
    
    // Convert to 32-bit representation for bit manipulation
    float xf = (float)x;
    RTI_UINT32 i;
    float x2, y;
    
    // Copy bits to integer
    i = *(RTI_UINT32*)&xf;
    
    x2 = xf * 0.5f;
    y = xf;
    
    // Magic number for fast inverse square root
    i = 0x5f3759df - (i >> 1);
    y = *(float*)&i;
    
    // Newton-Raphson iteration (can be repeated for more precision)
    y = y * (1.5f - (x2 * y * y));
    y = y * (1.5f - (x2 * y * y)); // Second iteration for better precision
    
    return (double)(xf * y);
}

// Simple integer square root for small values
RTI_UINT32 rti_sqrt_int(RTI_UINT32 x)
{
    if (x == 0) return 0;
    
    RTI_UINT32 result = 0;
    RTI_UINT32 bit = 1;
    
    // Find the highest bit
    while (bit <= x) {
        bit <<= 2;
    }
    bit >>= 2;
    
    // Calculate square root bit by bit
    while (bit != 0) {
        if (x >= result + bit) {
            x -= result + bit;
            result = (result >> 1) + bit;
        } else {
            result >>= 1;
        }
        bit >>= 2;
    }
    
    return result;
}