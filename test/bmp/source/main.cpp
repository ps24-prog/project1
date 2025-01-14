#include <minitui.h>

int main() {

  SetConsoleOutputCP(CP_UTF8);

  tui_pixelmap *pixel_map = tui_pixelmap::from_bmp("storage/capoo.bmp");
  
  for (int i = 0; i < pixel_map->height; i += 2) {
    for (int j = 0; j < pixel_map->width; j++) {
      auto pixel = pixel_map->get_formatter<tui_bmp_pixel_24>(tui_point(i / 2, j));
      pixel.set();
      printf("\u2580");
      tui_formatter().set();
    }
    tui_formatter().set();
    printf("\n");
  }

  return 0;
}