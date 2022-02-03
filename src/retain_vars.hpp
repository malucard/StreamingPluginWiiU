#ifndef _RETAINS_VARS_H_
#define _RETAINS_VARS_H_

#include <wups.h>

#define WUPS_STREAMING_SCREEN_DRC 0
#define WUPS_STREAMING_SCREEN_TV 1

#define WUPS_STREAMING_RESOLUTION_120P 120
#define WUPS_STREAMING_RESOLUTION_144P 144
#define WUPS_STREAMING_RESOLUTION_200P 200
#define WUPS_STREAMING_RESOLUTION_240P 240
#define WUPS_STREAMING_RESOLUTION_300P 300
#define WUPS_STREAMING_RESOLUTION_360P 360
#define WUPS_STREAMING_RESOLUTION_420P 420
#define WUPS_STREAMING_RESOLUTION_480P 480

extern wups_loader_app_status_t gAppStatus;
extern int32_t gScreen;
extern int32_t gResolution;
extern int32_t gMaxQuality;
extern int32_t gMinQuality;
extern int32_t gSendPriority;
extern int32_t gEncodePriority;
extern int32_t gFps;

#endif // _RETAINS_VARS_H_
