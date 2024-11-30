#include <minitui.h>

tui_rect global_rect;
tui_point scr_size = {SCR_HEIGHT, SCR_WIDTH};

bool 
tui_point::is_in(
  const tui_rect &rect
) const {
  return x >= rect.start.x
      && y >= rect.start.y
      && x <= rect.end.x
      && y <= rect.end.y;
}

bool 
tui_point::is_valid() const {
  return is_in(global_rect);
}

tui_point
tui_point::operator - (
  const tui_point &b
) const {
  return (tui_point) {
    .x = x - b.x,
    .y = y - b.y
  };
}

int
tui_rect::height() const {
  return end.x - start.x + 1;
}

int
tui_rect::width() const {
  return end.y - start.y + 1;
}