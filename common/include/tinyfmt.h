#ifndef TINYFMT_H__
#define TINYFMT_H__

#include "basics.h"

#define TUI_FMT_MAXLEN 65536

struct tui_fmt {
  char *body;
  tui_fmt(const char *fmt, ...) {
    body = new char[TUI_FMT_MAXLEN];
    va_list args;
    va_start(args, fmt);
    vsnprintf(body, TUI_FMT_MAXLEN, fmt, args);
    va_end(args);
  }
  const char *operator() () const {
    return body;
  }
  ~tui_fmt() {
    delete[] body;
  }
};

#endif