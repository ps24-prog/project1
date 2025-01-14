#include <minitui.h>

tui_pixelmap::tui_pixelmap(
  int width,
  int height,
  int byte_per_pixel
) : width(width), height(height), byte_per_pixel(byte_per_pixel) {
  bitmap = new uint8_t*[height];
  for (int i = 0; i < height; i++) {
    // align to 4 bytes
    bitmap[i] = new uint8_t[(width * byte_per_pixel + 3) & (~3)];
  }
}

tui_pixelmap::~tui_pixelmap() {
  for (int i = 0; i < height; i++) {
    delete bitmap[i];
  }
  delete bitmap;
}

void *
tui_pixelmap::at(
  int x,
  int y
) {
  return bitmap[x] + y * byte_per_pixel;
}

void
tui_pixelmap::set_pixel(
  int x,
  int y,
  uint8_t *pixel
) {
  memcpy(at(x, y), pixel, byte_per_pixel);
}

tui_pixelmap *
tui_pixelmap::from_bmp(
  const char *filename
) {
  FILE *fp = fopen(filename, "rb");
  if (!fp) {
    Warn("Cannot open file %s", filename);
    fclose(fp);
    return NULL;
  }
  tui_bmp_header header;
  fread(&header, sizeof(header), 1, fp);
  if (header.bfType != 0x4d42) {
    Warn("Not a bmp file!");
    fclose(fp);
    return NULL;
  }
  if (header.bfOffBits != 0x36) {
    Warn("Do not support bmp file with offset %d", header.bfOffBits);
    fclose(fp);
    return NULL;
  }
  tui_bitmap_info info;
  fread(&info, sizeof(info), 1, fp);
  if (info.biSize != 40) {
    Warn("Only support 40 bytes info header!");
    fclose(fp);
    return NULL;
  }
  if (info.biBitCount != 24) {
    Warn("Only support 24-bit bmp file!");
    fclose(fp);
    return NULL;
  }
  if (info.biPlanes != 1) {
    Warn("Only support 1 plane bmp file!");
    fclose(fp);
    return NULL;
  }
  if (info.biCompression != 0) {
    Warn("Only support uncompressed bmp file!");
    fclose(fp);
    return NULL;
  }
  if (info.biClrUsed != 0) {
    Warn("Do not support bmp file with color table!");
    // just ignore it
  }
  if (info.biClrImportant != 0) {
    Warn("bmp file with important color!");
    // just ignore it
  }
  
  // task phase3: read the pixel data according to the rest of the file

  fclose(fp);

  // task phase3: fill it
  return NULL;
}