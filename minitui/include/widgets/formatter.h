#ifndef FORMATTER_H__
#define FORMATTER_H__

#include <ansi.h>

struct tui_color {
  uint8_t fr, fg, fb;
  uint8_t br, bg, bb;
  char ansi_code[128];
  tui_color(uint8_t fr, uint8_t fg, uint8_t fb, uint8_t br, uint8_t bg, uint8_t bb) : fr(fr), fg(fg), fb(fb), br(br), bg(bg), bb(bb) {
    sprintf(ansi_code, ANSI_CSI "38;2;%d;%d;%dm" ANSI_CSI "48;2;%d;%d;%dm", fr, fg, fb, br, bg, bb);
  }
  tui_color(uint32_t frgb=0x00ffffffu, uint32_t brgb=0u) {
    fr = (frgb >> 16) & 0xff;
    fg = (frgb >> 8) & 0xff;
    fb = frgb & 0xff;
    br = (brgb >> 16) & 0xff;
    bg = (brgb >> 8) & 0xff;
    bb = brgb & 0xff;
    sprintf(ansi_code, ANSI_CSI "38;2;%d;%d;%dm" ANSI_CSI "48;2;%d;%d;%dm", fr, fg, fb, br, bg, bb);
  }
  void set() const {
    ANSI_CMD(ansi_code);
    return ;
  }
};

typedef tui_color tui_formatter;

#endif