#include "retain_vars.hpp"
wups_loader_app_status_t gAppStatus __attribute__((section(".data"))) = WUPS_APP_STATUS_UNKNOWN;
int32_t gResolution __attribute__((section(".data"))) = WUPS_STREAMING_RESOLUTION_360P;
int32_t gScreen __attribute__((section(".data"))) = WUPS_STREAMING_SCREEN_DRC;
int32_t gSendPriority __attribute__((section(".data"))) = 31;
int32_t gEncodePriority __attribute__((section(".data"))) = 31;
int32_t gFps __attribute__((section(".data"))) = 30;
int32_t gMaxQuality __attribute__((section(".data"))) = 100;
int32_t gMinQuality __attribute__((section(".data"))) = 20;
