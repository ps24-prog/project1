#ifndef TINYFMT_H__
#define TINYFMT_H__

#include "basics.h"

#define TUI_FMT_MAXLEN 65536

struct tui_fmt {
  char *body;
  tui_fmt(const char *fmt, ...) {
    char *tmp = new char[TUI_FMT_MAXLEN];
    va_list args;
    va_start(args, fmt);
    vsnprintf(tmp, TUI_FMT_MAXLEN, fmt, args);
    va_end(args);
    body = new char[strlen(tmp) + 1];
    strcpy(body, tmp);
  }
  tui_fmt(const tui_fmt &other) {
    body = new char[strlen(other.body) + 1];
    strcpy(body, other.body);
  }
  bool operator == (const tui_fmt &other) const {
    return strcmp(body, other.body) == 0;
  }
  bool operator == (const char *other) const {
    return strcmp(body, other) == 0;
  }
  char *operator() () const {
    return (char *) body;
  }
  ~tui_fmt() {
    delete[] body;
  }
};

#endif