﻿#ifndef TIME_H__
#define TIME_H__
#if !defined(_MSC_VER)
#include <sys/time.h>
#endif
#include <stdlib.h>
#include <iostream>
using namespace std;


#ifdef _MSC_VER
#include <winsock2.h>
int gettimeofday(struct timeval *tp, void *tzp);

#endif
int64_t yang_get_milli_time();//haomiao
int64_t yang_get_micro_time();//weimiao
int64_t yang_get_milli_tick();//haomiao
int64_t yang_get_micro_tick();//weimiao
int64_t yang_get_nano_tick();//namiao
int64_t yang_get_system_time();
int64_t yang_update_system_time();

class YangNtp
{
public:
    uint64_t system_ms_;
    uint64_t ntp_;
    uint32_t ntp_second_;
    uint32_t ntp_fractions_;
public:
    YangNtp();
    virtual ~YangNtp();
public:
    static YangNtp from_time_ms(uint64_t ms);
    static YangNtp to_time_ms(uint64_t ntp);
public:
    static uint64_t kMagicNtpFractionalUnit;
};

class YangWallClock
{
public:
    YangWallClock();
    virtual ~YangWallClock();
public:
    /**
     * Current time in srs_utime_t.
     */
    virtual int64_t now();
};


#endif
