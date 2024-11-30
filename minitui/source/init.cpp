#include <minitui.h>

void 
tui_init(int argc, char **argv) {
  tui_process_args(argc, argv);
  tui_debug_init();
  tui_terminal_init();
  tui_ui_init();
}

void tui_process_args(int argc, char **argv) {
  char ch;
  while ((ch = getopt(argc, argv, "l:w:h:")) != -1) {
    switch (ch) {
      case 'l':
        strcpy(log_level_name, optarg);
        break;
      case 'h':
        scr_size.x = atoi(optarg);
        break;
      case 'w':
        scr_size.y = atoi(optarg);
        break;
      case '?':
        Error("Unknown option: %c\n", optopt);
        break;
      default:
        break;
    }
  }
}

void tui_terminal_init() {
  static char buf[32768];
  setvbuf(stdin, NULL, _IONBF, 0);
  setvbuf(stdout, buf, _IOFBF, 32768);
  setvbuf(stderr, NULL, _IONBF, 0);

#ifdef _WIN64
  Info("We are in windows, enabling virtual terminal input.");
  HANDLE stdin_handle = GetStdHandle(STD_INPUT_HANDLE);
  if (stdin_handle == INVALID_HANDLE_VALUE) {
    Error("GetStdHandle() failed");
  }
  DWORD stdin_mode;
  if (!GetConsoleMode(stdin_handle, &stdin_mode)) {
    Error("GetConsoleMode() failed");
  }
  
  stdin_mode &= ~ENABLE_ECHO_INPUT;
  stdin_mode &= ~ENABLE_LINE_INPUT;
  stdin_mode &= ~ENABLE_INSERT_MODE;
  stdin_mode |= ENABLE_MOUSE_INPUT;
  stdin_mode &= ~ENABLE_PROCESSED_INPUT;
  stdin_mode &= ~ENABLE_QUICK_EDIT_MODE;
  stdin_mode |= ENABLE_EXTENDED_FLAGS;  /* Needed when changing _QUICK_EDIT_MODE */
  stdin_mode |= ENABLE_WINDOW_INPUT;
  stdin_mode |= ENABLE_VIRTUAL_TERMINAL_INPUT;

  if (!SetConsoleMode(stdin_handle, stdin_mode)) {
    Error("SetConsoleMode() failed");
  }
  // printf("\033=");
  Info("Enable virtual terminal input success.");
#else
  Info("We are in Linux, doing some necessary initiation");

  termios termios_0;
  memset(&termios_0, 0, sizeof(termios));
  tcgetattr(1, &termios_0);
  
  termios_0.c_iflag &= ~(IGNBRK | BRKINT | PARMRK | ISTRIP | INLCR | IGNCR | ICRNL | IXON);
  // termios_0.c_oflag &= ~OPOST;
  termios_0.c_lflag &= ~(ECHO | ECHONL | ICANON | ISIG | IEXTEN);
  termios_0.c_cflag &= ~(CSIZE | PARENB);
  termios_0.c_cflag |= CS8;

  if (tcsetattr(1, TCSANOW, &termios_0) == 0) 
    Info("Initiation Finished!");
  else
    Error("Termios set failed!");

#endif

  ANSI_ENABLE(ANSI_MOUSE);
  // ANSI_ENABLE(ANSI_ANY_EVENT);
  ANSI_ENABLE(ANSI_SGR);

  // enable paste mode
  ANSI_ENABLE(ANSI_PASTE);
  // hide the cursor
  ANSI_DISABLE(ANSI_CURSOR);
  
  // ANSI_CMD(ANSI_ENABLE(ANSI_ALTER_BUF));
}