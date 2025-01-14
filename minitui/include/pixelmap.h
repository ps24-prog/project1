#ifndef BITMAP_H__
#define BITMAP_H__

#include <minitui.h>

template <typename color_t>
concept rgb_color = requires(color_t color) {
  { color.r } -> std::convertible_to<uint8_t>;
  color.r = (uint8_t) 0xff;
  { color.g } -> std::convertible_to<uint8_t>;
  color.g = (uint8_t) 0xff;
  { color.b } -> std::convertible_to<uint8_t>;
  color.b = (uint8_t) 0xff;
};

struct tui_pixelmap {
  int width;
  int height;
  int byte_per_pixel;
  uint8_t **bitmap;

  tui_pixelmap(int width, int height, int byte_per_pixel=3);
  
  void *at(int x, int y);
  void set_pixel(int x, int y, uint8_t *pixel);

  template <rgb_color color_t>
  color_t *at(int x, int y) {
    tui_assert(sizeof(color_t) == byte_per_pixel);
    return (color_t *) at(x, y);
  }

  template <rgb_color color_t>
  color_t get_pixel(int x, int y) {
    tui_assert(sizeof(color_t) == byte_per_pixel);
    return *(color_t *) at(x, y);
  }

  void set_pixel(int x, int y, rgb_color auto color) {
    tui_assert(sizeof(color) == byte_per_pixel);
    uint8_t *pixel = (uint8_t *) &color;
    set_pixel(x, y, pixel);
  }

  template <rgb_color color_t>
  void set_pixel(int x, int y, uint8_t r, uint8_t g, uint8_t b) {
    tui_assert(sizeof(color_t) == byte_per_pixel);
    color_t color;
    color.r = r, color.g = g, color.b = b;
    set_pixel(x, y, color);
  }

  template <rgb_color color_t>
  tui_formatter get_formatter(tui_point point) {
    tui_assert(byte_per_pixel == sizeof(color_t));
    // task phase3: generate formatter in this position
  }

  static tui_pixelmap *from_bmp(const char *filename);

  ~tui_pixelmap();
};

#endif