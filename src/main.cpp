#include <map>
#include <string>
#include <wups.h>
#include <wups/config.h>
#include <wups/config/WUPSConfig.h>
#include <wups/config/WUPSConfigItemMultipleValues.h>
#include <utils/logger.h>
#include "retain_vars.hpp"
#include "EncodingHelper.h"
#include "MJPEGStreamServerUDP.hpp"
#include "HeartBeatServer.hpp"
#include "stream_utils.h"

// Mandatory plugin information.
WUPS_PLUGIN_NAME("Screen streaming");
WUPS_PLUGIN_DESCRIPTION("Streams the screen via UDP");
WUPS_PLUGIN_VERSION("v0.2");
WUPS_PLUGIN_AUTHOR("Maschell");
WUPS_PLUGIN_LICENSE("GPL");

// Something is using "write"...
WUPS_FS_ACCESS()

void resolutionChanged(WUPSConfigItemMultipleValues* configItem, int32_t newResolution) {
    DEBUG_FUNCTION_LINE("Resolution changed %d \n",newResolution);
    switching_res_from = gResolution;
    gResolution = newResolution;
}

void screenChanged(WUPSConfigItemMultipleValues* configItem, int32_t newScreen) {
    DEBUG_FUNCTION_LINE("Screen changed %d \n",newScreen);
    gScreen = newScreen;
}

void fpsChanged(WUPSConfigItemMultipleValues* configItem, int32_t newFps) {
    DEBUG_FUNCTION_LINE("fps changed %d \n",newFps);
    gFps = newFps;
}

void maxQualChanged(WUPSConfigItemMultipleValues* configItem, int32_t v) {
    DEBUG_FUNCTION_LINE("max quality changed %d \n",v);
    gMaxQuality = v;
}

void minQualChanged(WUPSConfigItemMultipleValues* configItem, int32_t v) {
    DEBUG_FUNCTION_LINE("min quality changed %d \n",v);
    gMinQuality = v;
}

WUPS_GET_CONFIG() {
    WUPSConfig* config = new WUPSConfig("Streaming Plugin");
    WUPSConfigCategory* catOther = config->addCategory("Main");

    std::map<int32_t,std::string> resolutionValues;
    resolutionValues[WUPS_STREAMING_RESOLUTION_120P] = "120p";
    resolutionValues[WUPS_STREAMING_RESOLUTION_144P] = "144p";
    resolutionValues[WUPS_STREAMING_RESOLUTION_200P] = "200p";
    resolutionValues[WUPS_STREAMING_RESOLUTION_240P] = "240p";
    resolutionValues[WUPS_STREAMING_RESOLUTION_300P] = "300p";
    resolutionValues[WUPS_STREAMING_RESOLUTION_360P] = "360p";
    resolutionValues[WUPS_STREAMING_RESOLUTION_420P] = "420p";
    resolutionValues[WUPS_STREAMING_RESOLUTION_480P] = "480p";

    std::map<int32_t,std::string> screenValues;
    screenValues[WUPS_STREAMING_SCREEN_DRC] = "Gamepad";
    screenValues[WUPS_STREAMING_SCREEN_TV] = "TV";

    std::map<int32_t,std::string> fpsValues;
    fpsValues[10] = "10 fps";
    fpsValues[12] = "12 fps";
    fpsValues[15] = "15 fps";
    fpsValues[20] = "20 fps";
    fpsValues[30] = "30 fps";
    fpsValues[60] = "60 fps";

    std::map<int32_t,std::string> boolValues;
    boolValues[0] = "No";
    boolValues[1] = "Yes";

    std::map<int32_t,std::string> qualValues;
    for(int i = 0; i <= 100; i += 5) {
        qualValues[i] = std::to_string(i) + "%";
    }

    //                    item Type             config id           displayed name              default value  onChangeCallback.
    catOther->addItem(new WUPSConfigItemMultipleValues("screen", "Screen", gScreen, screenValues, screenChanged));
    catOther->addItem(new WUPSConfigItemMultipleValues("resolution", "Resolution", gResolution, resolutionValues, resolutionChanged));
    catOther->addItem(new WUPSConfigItemMultipleValues("fps", "Target framerate", gFps, fpsValues, fpsChanged));
    catOther->addItem(new WUPSConfigItemMultipleValues("minqual", "Minimum quality", gMinQuality, qualValues, minQualChanged));
    catOther->addItem(new WUPSConfigItemMultipleValues("maxqual", "Maximum quality", gMaxQuality, qualValues, maxQualChanged));

    return config;
}

// Gets called once the loader exists.
INITIALIZE_PLUGIN() {
    socket_lib_init();

    log_init();
}


// Called whenever an application was started.
ON_APPLICATION_START(my_args) {
    socket_lib_init();
    log_init();

    gAppStatus = WUPS_APP_STATUS_FOREGROUND;

    img_heap = (uint8_t*) memalign(256, 4 * 427 * 240 * P240_IMGS);

    EncodingHelper::destroyInstance();
    EncodingHelper::getInstance()->StartAsyncThread();
    EncodingHelper::getInstance()->setMJPEGStreamServer(HeartBeatServer::getInstance()->getMJPEGServer());

    log_init();
}

ON_APP_STATUS_CHANGED(status) {
    gAppStatus = status;

    if(status == WUPS_APP_STATUS_CLOSED) {
        EncodingHelper::destroyInstance();
        HeartBeatServer::destroyInstance();
    }
}
