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
  tui_canvas_unit operator [] (tui_point point) const;

  void update_content(tui_point point, char content, tui_formatter formatter);

  tui_point print_content(tui_point point, bool linelimit, tui_formatter formatter, const char *content);

  template <typename... Args>
  tui_point print_content(tui_point point, bool linelimit, tui_formatter formatter, const char *content, Args... args) {
    tui_point res = print_content(point, linelimit, formatter, content);
    return print_content(res, linelimit, args...);
  }

  tui_canvas(tui_rect area, tui_formatter default_formatter=tui_formatter());
  virtual ~tui_canvas();
  
  virtual void draw(tui_point point) const;
};

typedef tui_canvas::tui_canvas_unit tui_canvas_unit;

#endif