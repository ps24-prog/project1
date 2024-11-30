#ifndef CONVAS_H__
#define CONVAS_H__

#include "../widget.h"
#include "../geometry.h"
#include "formatter.h"

struct tui_canvas : public tui_widget {
  
  char *content;
  tui_formatter *formatter;

  struct tui_canvas_unit {
    char *content;
    tui_formatter *formatter;
  };

  struct tui_canvas_line {
    int length;
    char *content;
    tui_formatter *formatter;
    tui_canvas_unit operator [] (int y) const;
  };

  tui_canvas_line operator [] (int x) const;
  tui_canvas(tui_rect area, tui_widget *parent=NULL);
  virtual ~tui_canvas();
  
  virtual tui_event *on_event(tui_event *event) = 0;
  virtual void draw(tui_point point) const;
};

typedef tui_canvas::tui_canvas_unit tui_canvas_unit;

#endif