#ifndef LINE_EDITER_H__
#define LINE_EDITER_H__

#include <minitui.h>

struct line_editer : public tui_canvas {
  char buf[1024];
  line_editer(tui_rect area) 
  : tui_canvas(area) {
    tui_assert(area.height() == 1);

  }
  virtual ~line_editer() {}
};

#endif