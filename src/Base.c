#include "Base.h"

// 获取当前时间戳（毫秒）
ll get_current_timestamp_ms()
{
  struct timeval tv;
  gettimeofday(&tv, NULL);
  return (ll)tv.tv_sec * 1000 + tv.tv_usec / 1000;
}

// 获取当前时间戳（微秒）
ll get_current_timestamp_us()
{
  struct timeval tv;
  gettimeofday(&tv, NULL);
  return (ll)tv.tv_sec * 1000000 + tv.tv_usec;
}

// 将毫秒时间戳转换为 "yyyy/m/d HH:MM:SS.mmm" 格式字符串
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