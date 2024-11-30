#ifndef NCONVAS_H__
#define NCONVAS_H__

#include "../widget.h"
#include "../geometry.h"
#include "formatter.h"

struct tui_ncanvas : public tui_widget {
  uint8_t boff;

  tui_ncanvas(tui_rect area, tui_widget *parent=NULL);
  virtual ~tui_ncanvas();
  
  virtual tui_event *on_event(tui_event *event);
  virtual void draw(tui_point point) const;
};

#endif