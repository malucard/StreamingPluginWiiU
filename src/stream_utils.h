#ifndef _STREAM_UTILS_H_
#define _STREAM_UTILS_H_

#include <gx2/surface.h>
#include <utils/logger.h>

constexpr int P240_IMGS = 3;
constexpr int P120_IMGS = (427 * 240 * 4 * P240_IMGS + 192) / (214 * 120 * 4 + 192);
constexpr int P144_IMGS = (427 * 240 * 4 * P240_IMGS + 192) / (256 * 144 * 4 + 192);
constexpr int P200_IMGS = (427 * 240 * 4 * P240_IMGS + 192) / (356 * 200 * 4 + 192);
constexpr int P300_IMGS = (427 * 240 * 4 * P240_IMGS + 224) / (534 * 300 * 4 + 224);
constexpr int P360_IMGS = (427 * 240 * 4 * P240_IMGS) / (640 * 360 * 4);
constexpr int P420_IMGS = (427 * 240 * 4 * P240_IMGS + 208) / (747 * 420 * 4 + 208);
constexpr int IMGS = (427 * 240 * 4 * P240_IMGS) / (854 * 480 * 4);

extern int switching_res_from;
extern uint8_t* img_heap;
void* alloc_img();
void free_img(void* img);

void free_buf(GX2ColorBuffer* buf);

bool copyBuffer(GX2ColorBuffer * sourceBuffer, GX2ColorBuffer * targetBuffer, uint32_t targetWidth, uint32_t targetHeight);

bool streamVideo(GX2ColorBuffer *srcBuffer);

#endif
