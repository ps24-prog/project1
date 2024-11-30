#ifndef ANSI_H__
#define ANSI_H__

#include <basics.h>

#define ESC_CODE '\033'
#define ANSI_ESC "\033"
#define ANSI_CSI ANSI_ESC "["
#define ANSI_OSC ANSI_ESC "]"

#define ANSI_RST ANSI_CSI "0m"

#define ANSI_RED_NF ANSI_CSI "31m"
#define ANSI_RED_NG ANSI_CSI "41m"
#define ANSI_RED_HF ANSI_CSI "91m"
#define ANSI_RED_HG ANSI_CSI "101m"
#define ANSI_RED    ANSI_RED_NF

#define ANSI_BLUE_NF ANSI_CSI "34m"
#define ANSI_BLUE ANSI_BLUE_NF

#define ANSI_DEFAULT ANSI_CSI "39m"
#define ANSI_YELLOW ANSI_CSI "33m"
#define ANSI_GREEN ANSI_CSI "34m"

#define ANSI_WHITE_HG ANSI_CSI "107m"

#define ANSI_PACK(...) __VA_ARGS__
#define ANSI_FMT(original, fmt) fmt original ANSI_RST

#define ANSI_CMD(...) printf(__VA_ARGS__)

#define ANSI_MODE_ENABLE "h"
#define ANSI_MODE_DISABLE "l"

#define ANSI_CURSOR "?25"
#define ANSI_ALTER_BUF "?1049"
#define ANSI_PASTE "?2004"
#define ANSI_MOUSE "?1000"
#define ANSI_SGR "?1006"
#define ANSI_SGR_PIXEL "?1016"
#define ANSI_ANY_EVENT "?1003"

#define ANSI_ENABLE(ANSI_MODE) ANSI_CMD(ANSI_CSI ANSI_MODE ANSI_MODE_ENABLE)
#define ANSI_DISABLE(ANSI_MODE) ANSI_CMD(ANSI_CSI ANSI_MODE ANSI_MODE_DISABLE)

inline static void ansi_cursor_up(int x) { // CUU
  ANSI_CMD(ANSI_CSI "%dA", x);
}
inline static void ansi_cursor_dw(int x) { // CUD
  ANSI_CMD(ANSI_CSI "%dB", x);
}
inline static void ansi_cursor_fw(int x) { // CUF
  ANSI_CMD(ANSI_CSI "%dC", x);
}
inline static void ansi_cursor_bw(int x) { // CUB
  ANSI_CMD(ANSI_CSI "%dD", x);
}

inline static void ansi_cursor_set(int row, int col) {
  ANSI_CMD(ANSI_CSI "%d;%dH", row, col);
}

inline static void ansi_title(const char *title) {
  ANSI_CMD(ANSI_OSC "2;%s\x7", title);
}

inline static void ansi_icon(const char *icon) {
  ANSI_CMD(ANSI_OSC "1;%s\x7", icon);
}

inline static void ansi_get_size() {
  ANSI_CMD(ANSI_CSI "18t");
}

#endif