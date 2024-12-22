#ifndef TEXTBOX_H__
#define TEXTBOX_H__

#include <minitui.h>

struct tui_textbox : public tui_canvas {
  tui_textbox(tui_rect area, const char *path, tui_formatter formatter);
  tui_event *on_event(tui_event *event) override {
    delete event;
    return NULL;
  }
  virtual ~tui_textbox();
};

#endif