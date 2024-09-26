#ifndef VVO_TIME_H
#define VVO_TIME_H

#ifdef __cplusplus
extern "C" {
#endif//__cplusplus


#ifdef _WIN32
#include <Windows.h>
#else
#include <unistd.h>
#endif//_WIN32

#include <inttypes.h>



void vvoSleepMs(
    uint32_t milliseconds
);


#ifdef __cplusplus
}
#endif//_cplusplus

#endif//VVO_H