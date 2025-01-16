#include <minitui.h>

tui_point scr_size(SCR_HEIGHT, SCR_WIDTH);
tui_rect global_rect(tui_point(0, 0), tui_point(SCR_HEIGHT - 1, SCR_WIDTH - 1));

tui_point::tui_point(
  int x, 
  int y, 
  const tui_rect *rect
) : x(x), y(y), rect(rect) {
  if (!rect) {
    this->rect = &global_rect;
  }
}

tui_point::tui_point(
  const tui_point &p, 
  const tui_rect *rect
) : x(p.x), y(p.y), rect(rect) {
  if (!rect) {
    this->rect = p.rect;
  }
}

bool 
tui_point::is_in(
  const tui_rect &rect
) const {
  return x >= rect.head.x
      && y >= rect.head.y
      && x <= rect.tail.x
      && y <= rect.tail.y;
}

bool 
tui_point::is_in_local(
  const tui_rect &rect
) const {
  return is_in(rect.local_rect());
}

bool 
tui_point::is_valid() const {
  return is_in(*rect);
}

tui_point
tui_point::operator - (
  const tui_point &b
) const {
  return tui_point(x - b.x, y - b.y);
}

tui_point
tui_point::operator + (
  const tui_point &b
) const {
  return tui_point(x + b.x, y + b.y);
}

bool 
tui_point::operator == (
  const tui_point &b
) const {
  return x == b.x && y == b.y;
}

bool
tui_point::operator != (
  const tui_point &b
) const {
  return !(*this == b);
}

bool
tui_rect::operator == (
  const tui_rect &b
) const {
  return head == b.head && tail == b.tail;
}

bool
tui_rect::operator != (
  const tui_rect &b
) const {
  return !(*this == b);
}

int
tui_rect::height() const {
  return tail.x - head.x + 1;
}

int
tui_rect::width() const {
  return tail.y - head.y + 1;
}

bool
tui_rect::isvalid() const {
  return head.is_in(global_rect) && tail.is_in(global_rect);
}

tui_point&
tui_point::operator ++ () {
  if (!rect) {
    Error("No rect");
  }
  if (!is_valid()) {
    Error("Point not valid");
  }
  if (y == rect->tail.y) {
    y = rect->head.y;
    if (x == rect->tail.x) {
      x = -1;
      y = -1;
    } else {
      x++;
    }
  } else {
    y++;
  }
  return *this;
}