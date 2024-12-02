#ifndef MOUSE_WIDGET_H__
#define MOUSE_WIDGET_H__

#include "../widget.h"

struct tui_mouse_widget : public tui_widget {
  bool pressed;
  
  tui_mouse_widget(tui_rect area, tui_widget *parent=NULL);

  tui_event *mouse_event;

  virtual tui_event *on_event(tui_event *event) = 0;
  virtual void draw(tui_point point) const = 0;
  virtual ~tui_mouse_widget() {}
};

#endif