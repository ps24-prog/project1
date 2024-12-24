#ifndef FORMATTER_H__
#define FORMATTER_H__

#include <ansi.h>

struct tui_color {
  bool valid;
  uint8_t fr, fg, fb;
  uint8_t br, bg, bb;
  char ansi_code[128];
  tui_color() : valid(false) {}
  tui_color(uint8_t fr, uint8_t fg, uint8_t fb, uint8_t br, uint8_t bg, uint8_t bb) : valid(true), fr(fr), fg(fg), fb(fb), br(br), bg(bg), bb(bb) {
    sprintf(ansi_code, ANSI_CSI "38;2;%d;%d;%dm" ANSI_CSI "48;2;%d;%d;%dm", fr, fg, fb, br, bg, bb);
  }
  tui_color(uint32_t frgb, uint32_t brgb) {
    fr = (frgb >> 16) & 0xff;
    fg = (frgb >> 8) & 0xff;
    fb = frgb & 0xff;
    br = (brgb >> 16) & 0xff;
    bg = (brgb >> 8) & 0xff;
    bb = brgb & 0xff;
    valid = true;
    sprintf(ansi_code, ANSI_CSI "38;2;%d;%d;%dm" ANSI_CSI "48;2;%d;%d;%dm", fr, fg, fb, br, bg, bb);
  }
  void set() const {
    if (valid)
      ANSI_CMD(ansi_code);
    return ;
  }
};

typedef tui_color tui_formatter;

#endif