#ifndef DEBUG_H__
#define DEBUG_H__

#include <ansi.h>

enum tui_log_level {
  ALL,
  Trace,
  Debug,
  Info,
  Warn,
  Error,
  Fatal
};

extern const char *log_level_names[];
extern char log_level_name[];
extern int log_level;

#define _Log(...) fprintf(stderr, __VA_ARGS__)
// #define _Log_stderr(...) fprintf(stderr, __VA_ARGS__)

#define Log(level, ansi_fmt, fmt, ...) \
((level) >= log_level ? _Log(ANSI_FMT("%s [%s:%d %s] " fmt "\n", ansi_fmt), log_level_names[level], __FILE__, __LINE__, __func__, ## __VA_ARGS__) : 0)

#define Plain(fmt, ...) _Log(fmt "\n", ## __VA_ARGS__)


#define Info(fmt, ...) Log(Info, ANSI_BLUE, fmt, ## __VA_ARGS__)
#define Debug(fmt, ...) Log(Debug, ANSI_BLUE, fmt, ## __VA_ARGS__)
#define Trace(fmt, ...) Log(Trace, ANSI_DEFAULT, fmt, ## __VA_ARGS__)
#define Warn(fmt, ...) Log(Warn, ANSI_YELLOW, fmt, ## __VA_ARGS__)
#define Error(fmt, ...) do {\
  Log(5, ANSI_RED, fmt, ## __VA_ARGS__); \
  exit(-1); \
} while(0)

#define tui_assert(cond) do { \
  if (!(cond)) { \
    Error("Assertion failed: %s", #cond); \
  } \
} while(0)

void tui_debug_init();

#endif