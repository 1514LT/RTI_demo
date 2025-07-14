#ifndef BASE_H
#define BASE_H
#include <stdio.h>
#include <time.h>
#include <stdatomic.h>
#include "rti_me_c.h"

typedef long long ll;

// 获取当前时间戳（毫秒）
extern ll get_current_timestamp_ms();

// 将毫秒时间戳转换为 "yyyy/m/d HH:MM:SS.mmm" 格式字符串
extern void timestamp_to_string(ll timestamp_ms, char *buffer, int buffer_size);

#endif