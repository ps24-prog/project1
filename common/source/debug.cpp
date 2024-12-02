#include <basics.h>
#include <debug.h>

const char *log_level_names[] = {
  [ALL] = "All",
  [TRACE] = "Trace",
  [DEBUG] = "Debug",
  [INFO] = "Info",
  [WARN] = "Warn",
  [ERR] = "Error",
  [FATAL] = "Fatal",
};

int log_level = ALL;
char log_level_name[32] = "All";

void tui_debug_init() {

  freopen("build/game.log", "w", stderr);
  
#ifdef _WIN64
  int pid = GetCurrentProcessId();
#else
  int pid = getpid();
#endif
  FILE * file = fopen("build/pid.info", "w");
  fprintf(file, "%d\n", pid);
  fclose(file);

  for (; log_level < ARRLEN(log_level_names); log_level++) {
    if (strcmp(log_level_name, log_level_names[log_level]) == 0) {
      break;
    }
  }

  if (log_level == ARRLEN(log_level_names)) {
    log_level = ALL;
    Warn("log_level argument invalid, fallback to ALL");
  }

  Info("Set log level %s", log_level_names[log_level]);

  return ;
}