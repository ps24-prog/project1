#include <ansi.h>
#include <stdbool.h>
#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include <getopt.h>

#ifdef _WIN64
#include <windows.h>
#endif

#ifndef SCR_WIDTH
#define SCR_WIDTH 100
#endif
#ifndef SCR_HEIGHT
#define SCR_HEIGHT 40
#endif

static int cmd_help(char** argv, int argc);

static int cmd_log(char** argv, int argc) {
#ifdef _WIN64
  system("cat build\\game.log");
#else
  system("cat build/game.log");
#endif
  ANSI_CMD(ANSI_RST);
  return 0;
}

static int cmd_debug(char** argv, int argc) {

  if (argc > 1 && strcmp(argv[1], "-r") == 0) {
  #ifdef _WIN64
    system("gdb -ex \"target remote localhost:8117\" -ex \"file build/nju_universalis\" -ex \"b main\" -ex \"c\"");
  #else
    system("gdb -ex \"target remote localhost:8117\" -ex \"b main\" -ex \"c\"");
  #endif
  } else {
  #ifdef _WIN64
    FILE *file = fopen("build\\pid.info", "r");
  #else
    FILE *file = fopen("build/pid.info", "r");
  #endif

    if (!file) {
      puts("The program seems not to be running...");
    } else {
      int pid;
      fscanf(file, "%d", &pid);
      fclose(file);
      char cmd[2048];
      sprintf(cmd, "gdb -p %d", pid);
      system(cmd);
    }
  }

}

static int cmd_game(char** argv, int argc) {

#ifdef _WIN64

  bool debug_server = 0;
  char log_level[25] = "Debug";

  optind = 0;
  char ch;
  while ((ch = getopt(argc, argv, "gl:")) != -1) {
    switch (ch) {
      case 'g':
        debug_server = 1;
        break;
      case 'l':
        strcpy(log_level, optarg);
        break;
      case '?':
        printf("Unknown option: %s\n", optopt);
        break;
    }
  }

  char cmd[1024], pwd[1024];
  FILE *pwd_f = _popen("cygpath -aw .", "r");
  fscanf(pwd_f, "%s", pwd);

  if (debug_server) {
    sprintf(cmd, "wt --size %d,%d --pos 100,100 gdbserver :8117 %s\\build\\nju_universalis -l %s", SCR_WIDTH, SCR_HEIGHT + 1, pwd, log_level);
    printf("%s\n", cmd);
    system(cmd);
  }
  else {
    sprintf(cmd, "wt --size %d,%d --pos 100,100 %s\\build\\nju_universalis -l %s", SCR_WIDTH, SCR_HEIGHT + 1, pwd, log_level);
    printf("%s\n", cmd);
    system(cmd);
  }

#else

  if (debug_server) {
    system("konsole -e gdbserver :8117 `pwd`/build/nju_universalis -p TerminalColumns=100 -p TerminalRows=40 -p ICON=`pwd`/resources/dbcq.ico 2> /dev/null &");
  }
  else system("konsole -e `pwd`/build/nju_universalis -p TerminalColumns=100 -p TerminalRows=40 -p ICON=`pwd`/resources/dbcq.ico 2> /dev/null &");

#endif

  return 0;

}

static int cmd_compile(char **argv, int argc) {
  bool clean_before_compile = 0;
  bool test_mode = 0;

  optind = 0;
  char ch;
  while ((ch = getopt(argc, argv, "ct")) != -1) {
    switch (ch) {
      case 'c':
        clean_before_compile = 1;
        break;
      case 't':
        test_mode = 1;
        break;
      case '?':
        printf("Unknown option: %c\n", optopt);
        break;
      default:
        break;
    }
  }

  if (clean_before_compile) {
    system("make clean");
  } 
  if (test_mode) {
    printf("%s\n", "make -j8 test_compile");
    return system("make -j8 test_compile");
  } else {
    printf("%s\n", "make -j8 compile");
    return system("make -j8 compile");
  }
}

static struct {
  const char* name;
  const char* description;
  int (*handler) (char**, int);
} cmd_table[] = {
  { "help", "Display information about all supported commands", cmd_help },
  { "game", "Launch the game", cmd_game},
  { "log",  "Display log information", cmd_log},
  { "debug", "Debug the program", cmd_debug},
  { "compile", "Compile the program, use [-c] to clean before compilation", cmd_compile},
};
// #define ARRLEN(arr) (int)(sizeof(arr) / sizeof(arr[0]))
#define CNT_CMD ARRLEN(cmd_table)

void cmd_mainloop() {
  char str[1024];
  printf("Welcome to the game launcher, use `help` for help.\n");
  while (1) {
    printf(">> ");
    fgets(str, 1024, stdin);
    *strchr(str, '\n') = '\0';

    int argc = 0;
    char* argv[32];
    char* cmd = strtok(str, " ");
    argv[argc++] = cmd;
    while (cmd = strtok(NULL, " ")) {
      argv[argc++] = cmd;
    }
    argv[argc] = NULL;

    if (argv[0] == NULL) { continue; }
    if (strcmp(argv[0], "exit") == 0 || strcmp(argv[0], "quit") == 0) break;

    int cmd_num;
    for (cmd_num = 0; cmd_num < CNT_CMD; cmd_num++) {
      if (strcmp(argv[0], cmd_table[cmd_num].name) == 0) {
        cmd_table[cmd_num].handler(argv, argc);
        break;
      }
    }

    if (cmd_num == CNT_CMD) { printf("Unknown command '%s'\n", argv[0]); }
  }
}



int main() {

  cmd_mainloop();

  return 0;
}

static int cmd_help(char** argv, int argc) {
  if (argc == 1) {
    /* no argument given */
    for (int i = 0; i < CNT_CMD; i++) {
      printf("%s\t - %s\n", cmd_table[i].name, cmd_table[i].description);
    }
  }
  else {
    int i;
    bool flag[CNT_CMD];
    memset(flag, 0, sizeof(flag));
    for (int cur = 1; cur < argc; cur++) {
      for (i = 0; i < CNT_CMD; i++) {
        if (strcmp(argv[cur], cmd_table[i].name) == 0) {
          if (!flag[i]) {
            printf("%s\t - %s\n", cmd_table[i].name, cmd_table[i].description);
          }
          flag[i] = 1;
          break;
        }
      }
      if (i == CNT_CMD) printf("Unknown command '%s'\n", argv[cur]);
    }
  }
  return 0;
}
