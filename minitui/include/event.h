#ifndef EVENT_H__
#define EVENT_H__

#include <basics.h>

// SGR mouse event codes
enum tui_mouse_event_type {
  MOUSE_LEFT_CLICK=0,
  MOUSE_MID_CLICK=1,
  MOUSE_RIGHT_CLICK=2,
  MOUSE_MOVE=35,
  MOUSE_SCROLL=64,
};

#define TUI_KEYBD_EVENT 1
#define TUI_MOUSE_EVENT 2
#define TUI_EXIT_EVENT 127
#define TUI_PASTE_EVENT 22



struct tui_mouse_event {
  int type;
  int x, y;
  bool ispress;
  tui_mouse_event(int type, int x, int y, bool ispress) 
    : type(type)
    , x(x)
    , y(y)
    , ispress(ispress) 
  {}
  tui_point get_point() const {
    return (tui_point) {x, y};
  }
  void log_mouse_event(tui_mouse_event *mouse_event, bool trace=false);
};

struct tui_paste_event {
  char *str;
};

enum tui_kbd_code_type {
  TUI_KBD_CHAR,
  TUI_KBD_FUNCTION,
  TUI_KBD_CTRL
};

struct tui_kbd_event {
  // tui_kbd_code_type code_type;
  char code;
};

struct tui_exit_event {
  int retcode;
  void *invoker;
};

struct tui_event {
  int event_type;
  void *event_body;
  tui_event(int event_type, void *event_body) 
    : event_type(event_type)
    , event_body(event_body)
  {}
  ~tui_event() {
    // only free memory allocated, no destruction function called
    free(event_body);
  }
};

int tui_exec();

#endif