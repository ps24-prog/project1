#ifndef TEXTBOX_H__
#define TEXTBOX_H__

#include <minitui.h>

struct textbox : public tui_canvas {
  textbox(tui_rect area, const char *path, tui_formatter formatter);
  tui_event *on_event(tui_event *event) override {
    if (event->event_type == TUI_KEYBD_EVENT) {
      auto kbd_event = (tui_kbd_event *) event->event_body;
      if (kbd_event->check_key('q')) {
        delete event;
        return new tui_event(
          TUI_EXIT_EVENT, 
          new tui_exit_event(0)
        );
      }
    }
    delete event;
    return NULL;
  }
  virtual ~textbox();
};

#endif