#ifndef WIDGET_H__
#define WIDGET_H__

#include "geometry.h"
#include "event.h"
#include <debug.h>

struct tui_widget {
  tui_rect area;
  bool updated;
  tui_widget *parent;
  
  tui_widget(tui_rect area, tui_widget *parent=NULL);
  tui_point position_interpreter(tui_point point) const;
  tui_point position_mapper(tui_point local_point) const;

  void set_updated() {updated = true;};
  void reset_updated() {updated = false;};
  bool get_updated() const {return updated;}

  virtual tui_event *on_event(tui_event *event) = 0;
  virtual void draw(tui_point point) const = 0;
  virtual ~tui_widget() {}
};

#endif