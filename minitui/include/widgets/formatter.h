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
    else
      ANSI_CMD(ANSI_RST);
    return ;
  }
};

typedef tui_color tui_formatter;

#define TUI_WHITE_V 0xccccccu
#define TUI_GREEN_V 0x13a10eu
#define TUI_RED_V 0xc50f1fu
#define TUI_BLUE_V 0x003fdau
#define TUI_YELLOW_V 0xc19c00u
#define TUI_PURPLE_V 0x881798u
#define TUI_CYAN_V 0x3a96ddu
#define TUI_BLACK_V 0x0c0c0cu

#define TUI_LIGHT_WHITE_V 0xffffffu
#define TUI_LIGHT_GREEN_V 0x16c60cu
#define TUI_LIGHT_RED_V 0xff4a4au
#define TUI_LIGHT_BLUE_V 0x3b78ffu
#define TUI_LIGHT_YELLOW_V 0xf9f1a5u
#define TUI_LIGHT_PURPLE_V 0xb4009eu
#define TUI_LIGHT_CYAN_V 0x61d6d6u
#define TUI_LIGHT_BLACK_V 0x767676u

#define TUI_NJU_PURPLE 0x63065fu

enum tui_eight_colors {
  TUI_WHITE,
  TUI_GREEN,
  TUI_RED,
  TUI_BLUE,
  TUI_YELLOW,
  TUI_PURPLE,
  TUI_CYAN,
  TUI_BLACK
};

extern const uint32_t tui_eight_colors_v[];
extern const uint32_t tui_eight_colors_light_v[];

#endif
