#include "stream_utils.h"
#include "retain_vars.hpp"
#include "EncodingHelper.h"
#include "HeartBeatServer.hpp"
#include <fs/FSUtils.h>
#include <malloc.h>

#include <gx2/event.h>
#include <gx2/surface.h>
#include <gx2/mem.h>

#ifdef __cplusplus
extern "C" {
#endif
void
GX2ResolveAAColorBuffer(const GX2ColorBuffer * srcColorBuffer,
                        GX2Surface * dstSurface,
                        uint32_t dstMip,
                        uint32_t dstSlice);

#ifdef __cplusplus
}
#endif

uint8_t* img_heap = NULL;
static bool img_used[P120_IMGS];
static int alloc_fails = 0;
int switching_res_from = 0;

void* alloc_img() {
    if(switching_res_from) {
        for(int i = 0; i < P120_IMGS; i++) {
            if(img_used[i]) return NULL;
        }
        switching_res_from = 0;
    }
    switch(gResolution) {
        case 120:
            for(int i = 0; i < P120_IMGS; i++) {
                if(!img_used[i]) {
                    img_used[i] = true;
                    return img_heap + (4 * 214 * 120 + 192) * i;
                }
            }
            break;
        case 144:
            for(int i = 0; i < P144_IMGS; i++) {
                if(!img_used[i]) {
                    img_used[i] = true;
                    return img_heap + (4 * 256 * 144 + 192) * i;
                }
            }
            break;
        case 200:
            for(int i = 0; i < P200_IMGS; i++) {
                if(!img_used[i]) {
                    img_used[i] = true;
                    return img_heap + (4 * 356 * 200 + 192) * i;
                }
            }
            break;
        case 240:
            for(int i = 0; i < P240_IMGS; i++) {
                if(!img_used[i]) {
                    img_used[i] = true;
                    return img_heap + (4 * 427 * 240 + 192) * i;
                }
            }
            break;
        case 300:
            for(int i = 0; i < P300_IMGS; i++) {
                if(!img_used[i]) {
                    img_used[i] = true;
                    return img_heap + (4 * 534 * 300 + 224) * i;
                }
            }
            break;
        case 360:
            for(int i = 0; i < P360_IMGS; i++) {
                if(!img_used[i]) {
                    img_used[i] = true;
                    return img_heap + (4 * 640 * 360) * i;
                }
            }
            break;
        case 420:
            for(int i = 0; i < P420_IMGS; i++) {
                if(!img_used[i]) {
                    img_used[i] = true;
                    return img_heap + (4 * 747 * 420 + 208) * i;
                }
            }
            break;
        case 480:
            for(int i = 0; i < IMGS; i++) {
                if(!img_used[i]) {
                    img_used[i] = true;
                    return img_heap + 4 * 854 * 480 * i;
                }
            }
            break;
    }
    return NULL;
}

void free_img(void* img) {
    switch(switching_res_from? switching_res_from: gResolution) {
        case 120:
            for(int i = 0; i < P120_IMGS; i++) {
                if(img == img_heap + (4 * 214 * 120 + 192) * i) {
                    img_used[i] = false;
                }
            }
            break;
        case 144:
            for(int i = 0; i < P144_IMGS; i++) {
                if(img == img_heap + (4 * 256 * 144 + 192) * i) {
                    img_used[i] = false;
                }
            }
            break;
        case 200:
            for(int i = 0; i < P200_IMGS; i++) {
                if(img == img_heap + (4 * 356 * 200 + 192) * i) {
                    img_used[i] = false;
                }
            }
            break;
        case 240:
            for(int i = 0; i < P240_IMGS; i++) {
                if(img == img_heap + (4 * 427 * 240 + 192) * i) {
                    img_used[i] = false;
                }
            }
            break;
        case 300:
            for(int i = 0; i < P300_IMGS; i++) {
                if(img == img_heap + (4 * 534 * 300 + 224) * i) {
                    img_used[i] = false;
                }
            }
            break;
        case 360:
            for(int i = 0; i < P360_IMGS; i++) {
                if(img == img_heap + 4 * 640 * 360 * i) {
                    img_used[i] = false;
                }
            }
            break;
        case 420:
            for(int i = 0; i < P420_IMGS; i++) {
                if(img == img_heap + (4 * 747 * 420 + 208) * i) {
                    img_used[i] = false;
                }
            }
            break;
        case 480:
            for(int i = 0; i < IMGS; i++) {
                if(img == img_heap + 4 * 854 * 480 * i) {
                    img_used[i] = false;
                }
            }
            break;
    }
}

bool copyBuffer(GX2ColorBuffer * sourceBuffer, GX2ColorBuffer * targetBuffer, uint32_t targetWidth, uint32_t targetHeight) {
    // Making sure the buffers are not NULL
    if (sourceBuffer != NULL && targetBuffer != NULL) {
        void* img = alloc_img();
        if(img == NULL) {
            //DEBUG_FUNCTION_LINE("failed to allocate memory.\n");
            return false;
        }
        targetBuffer->surface.use =         (GX2SurfaceUse) (GX2_SURFACE_USE_COLOR_BUFFER | GX2_SURFACE_USE_TEXTURE);
        targetBuffer->surface.dim =         GX2_SURFACE_DIM_TEXTURE_2D;
        targetBuffer->surface.width =       targetWidth;
        targetBuffer->surface.height =      targetHeight;
        targetBuffer->surface.depth =       1;
        targetBuffer->surface.mipLevels =   1;
        targetBuffer->surface.format =      GX2_SURFACE_FORMAT_UNORM_R8_G8_B8_A8;
        //targetBuffer->surface.format =      GX2_SURFACE_FORMAT_SRGB_R8_G8_B8_A8;
        targetBuffer->surface.aa =          GX2_AA_MODE1X;
        targetBuffer->surface.tileMode =    GX2_TILE_MODE_LINEAR_ALIGNED;
        targetBuffer->viewMip =             0;
        targetBuffer->viewFirstSlice =      0;
        targetBuffer->viewNumSlices =       1;

        GX2CalcSurfaceSizeAndAlignment(&targetBuffer->surface);
        GX2InitColorBufferRegs(targetBuffer);

        // Let's allocate the memory.
        targetBuffer->surface.image = img;
        //DEBUG_FUNCTION_LINE("Allocated image data buffer. data %08X  size %08X \n",targetBuffer->surface.image,targetBuffer->surface.imageSize);

        GX2Invalidate(GX2_INVALIDATE_MODE_CPU, targetBuffer->surface.image, targetBuffer->surface.imageSize);
        if (sourceBuffer->surface.aa == GX2_AA_MODE1X) {
            // If AA is disabled, we can simply use GX2CopySurface.
            GX2CopySurface(&sourceBuffer->surface,
                           sourceBuffer->viewMip,
                           sourceBuffer->viewFirstSlice,
                           &targetBuffer->surface, 0, 0);
        } else {
            // If AA is enabled, we need to resolve the AA buffer.
            GX2ResolveAAColorBuffer(sourceBuffer,&targetBuffer->surface, 0, 0);
        }
        return true;
    } else {
        DEBUG_FUNCTION_LINE("Couldn't copy buffer, pointer was NULL\n");
        return false;
    }
}

uint32_t frame_counter = 0;
uint32_t frame_counter_skipped = 0;
int32_t curQuality = 50;
int32_t stepQuality = 2;
int32_t maxFrameDropsQuality = 5;
int32_t minFrameDropsQuality = 95;

void free_buf(GX2ColorBuffer* buf) {
    if(buf->surface.image != NULL) {
        free_img(buf->surface.image);
    }
    if(buf != NULL) {
        free(buf);
        buf = NULL;
    }
}

bool streamVideo(GX2ColorBuffer *srcBuffer) {
    if(srcBuffer == NULL) {
        return false;
    }

    if(!HeartBeatServer::isInstanceConnected()) {
        return false;
    }

    GX2ColorBuffer* colorBuffer  = (GX2ColorBuffer*) memalign(0x40,sizeof(GX2ColorBuffer));
    if(colorBuffer == NULL) {
        DEBUG_FUNCTION_LINE("Failed to allocate color buffer\n");
        return false;
    }
    //DEBUG_FUNCTION_LINE("allocated at %08X\n",colorBuffer);
    memset(colorBuffer,0,sizeof(GX2ColorBuffer));

    // keep dimensions

    uint32_t height = gResolution;
    uint32_t width = (height * 16 + 8) / 9;
    frame_counter++;

    bool valid = copyBuffer(srcBuffer,colorBuffer,width,height);
    if(!valid) {
        //DEBUG_FUNCTION_LINE("Copy buffer failed.\n");
        alloc_fails++;
        frame_counter_skipped++;
        free_buf(colorBuffer);
    } else {
        //DEBUG_FUNCTION_LINE("Copy buffer was successful.\n");

        // Flush out destinations caches
        GX2Invalidate(GX2_INVALIDATE_MODE_COLOR_BUFFER, colorBuffer->surface.image,colorBuffer->surface.imageSize);

        // Wait for GPU to finish
        //GX2DrawDone();

        DCFlushRange(colorBuffer,sizeof(GX2ColorBuffer));

        OSMessage message;
        message.message = (void *) 0x1337;
        message.args[1] = (uint32_t) colorBuffer;

        if(!EncodingHelper::addFSQueueMSG(message)) {
            frame_counter_skipped++;

            //DEBUG_FUNCTION_LINE("Adding to queue failed, free memory\n");
            free_buf(colorBuffer);
            valid = false;
        }
    }

    if(frame_counter == 60) { // Print this every second.

        DEBUG_FUNCTION_LINE("Streaming at %d fps, skipped %d, fails %d, res %dp, quality %d%%\n",frame_counter-frame_counter_skipped, frame_counter_skipped, alloc_fails, height, curQuality);

        int32_t curQualityOld = curQuality;
        if(frame_counter - frame_counter_skipped < gFps - 10) {  // Lower the quality if we drop more than [maxFrameDropsQuality]% of the frames.
            curQuality -= stepQuality * 4;
        } else if(frame_counter - frame_counter_skipped < gFps) {  // Lower the quality if we drop more than [maxFrameDropsQuality]% of the frames.
            curQuality -= stepQuality;
        } else if(frame_counter - frame_counter_skipped > gFps + 5) { // Increase the quality if we drop less than [minFrameDropsQuality]% of the frames.
            curQuality += stepQuality; // Increase the quality by [stepQuality]%
        }

        // Make sure to set the quality to at least [minQuality]%
        if(curQuality < gMinQuality) {
            curQuality = gMinQuality;
        }

        // Make sure to set the quality to at most [maxQuality]%
        if(curQuality >= gMaxQuality) {
            curQuality = gMaxQuality;
        }

        frame_counter = 0;
        frame_counter_skipped = 0;
        alloc_fails = 0;
    }

    return valid;
}
