#ifndef BMP_H__
#define BMP_H__

#include <minitui.h>

struct tui_bmp_header{
  uint16_t bfType;
  uint32_t bfSize;
  uint32_t bfReserved;
  uint32_t bfOffBits;
}__attribute__((packed));

struct tui_bitmap_info
{
  uint32_t biSize;
  uint32_t biWidth;
  int32_t  biHeight;
  uint16_t biPlanes;
  uint16_t biBitCount;
  uint32_t biCompression;
  uint32_t biSizeImages;
  int32_t  biXPelsPerMeter;
  int32_t  biYPelsPerMeter;
  uint32_t biClrUsed;
  uint32_t biClrImportant;
}__attribute((packed));

struct tui_bmp_pixel_24{
  uint8_t b;
  uint8_t g;
  uint8_t r;
}__attribute__((packed));

#endif  