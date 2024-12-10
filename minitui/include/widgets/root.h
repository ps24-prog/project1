#ifndef ROOT_H__
#define ROOT_H__

#include "../widget.h"

struct tui_root : public tui_widget {
  int child_count;

  tui_root(tui_rect area, tui_widget *parent=NULL);
  virtual tui_event *on_event(tui_event *event);
  virtual void draw(tui_point point) const;
};

#endif