#ifndef MAINSCR_H__
#define MAINSCR_H__

#include <minitui.h>

struct mainscr : public tui_canvas {
  int time_counter = 0;

  mainscr(tui_rect area);
  virtual ~mainscr() {}
  
  virtual tui_event *on_event(tui_event *event);
  
  void set_global_formatter(tui_formatter formatter);
  void set_central_message(const char *message, tui_formatter formatter = tui_formatter());
  void set_time_counter(tui_formatter formatter);
};

#endif