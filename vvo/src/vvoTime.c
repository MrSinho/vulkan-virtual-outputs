#ifdef __cplusplus
extern "C" {
#endif//__cplusplus

#include "vvo/vvoTime.h"



void vvoSleepMs(
    uint32_t milliseconds
) {
#ifdef _WIN32
	Sleep((DWORD)milliseconds);
#else
	usleep(milliseconds * 1000);
#endif//_WIN32
}



#ifdef __cplusplus
}
#endif//__cplusplus