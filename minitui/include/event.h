#ifndef EVENT_H__
#define EVENT_H__

#include <basics.h>
#include <geometry.h>

#include "keycode.h"

void tui_set_gbl_mouse_point(tui_point point);
tui_point tui_get_gbl_mouse_point();

struct event;

// SGR mouse event codes
enum tui_mouse_event_type {
  MOUSE_LEFT_CLICK=0,
  MOUSE_MID_CLICK=1,
  MOUSE_RIGHT_CLICK=2,
  MOUSE_MOVE=35,
  MOUSE_SCROLL=64,
  MOUSE_FADE_OUT=128
};

enum tui_event_type {
  TUI_NO_EVENT,
  TUI_KEYBD_EVENT,
  TUI_MOUSE_EVENT,
  TUI_TIMER_INTERUPTER_EVENT, // no body
  TUI_EXIT_EVENT,
  TUI_PASTE_EVENT
};

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
    return tui_point(x, y);
  }
  void translate(tui_point head) {
    x -= head.x;
    y -= head.y;
  }
  static void log_mouse_event(const tui_mouse_event *mouse_event, bool trace=false);
};

struct tui_paste_event {
  char *str;
  ~tui_paste_event() {
    delete str;
  }
};

enum tui_kbd_code_type {
  TUI_KBD_CHAR,
  TUI_KBD_FUNCTION,
  TUI_KBD_CTRL
};

struct tui_kbd_event {
  // tui_kbd_code_type code_type;
  char code;
  tui_kbd_event(char code) : code(code) {}
  bool check_key(char key, bool strict=true);
};

struct tui_exit_event {
  int retcode;
  tui_exit_event(int retcode) : retcode(retcode) {}
};

struct tui_widget;

struct tui_event {
  int event_type;
  void *event_body;
  tui_widget *event_target;

  tui_event *translate_mouse_event(tui_point head);
  bool check_key(char key, bool strict=true);
  tui_point check_mouse_move();
  
  static void log_event(const tui_event *event, bool trace=false);
  static tui_event *exit_on_key(tui_event *event, char key, int retcode=0);

  tui_event(int event_type, void *event_body, tui_widget *event_target=NULL) 
    : event_type(event_type)
    , event_body(event_body)
    , event_target(event_target)
  {}
  ~tui_event();
};

int tui_exec();
tui_event *tui_get_event();
void tui_threads_init();

class tui_event_queue {
  std::queue<tui_event *> queue;
  std::mutex mutex;
  std::condition_variable cv;
public:
  tui_event_queue() : queue(), mutex(), cv() {}
  ~tui_event_queue();
  void push_event(tui_event *event);
  tui_event *pop_event();
  bool empty();
  void clear();
};

extern tui_event_queue gbl_event_queue;

// struct tui_chrono_event {

// }



#endif