#ifndef GEOMETRY_H__
#define GEOMETRY_H__

#ifndef SCR_WIDTH
#define SCR_WIDTH 50
#endif
#ifndef SCR_HEIGHT
#define SCR_HEIGHT 20
#endif

struct tui_rect;

struct tui_point {
  int x, y;
  bool is_valid() const;
  bool is_in(const tui_rect &rect) const;
  tui_point operator - (const tui_point &b) const;
};

extern tui_point scr_size;

struct tui_rect {
  tui_point start, end;
  int height() const;
  int width() const;
  void log_rect() const {
    Debug("Rect: (%d, %d) -> (%d, %d)", start.x, start.y, end.x, end.y);
  }
};

extern tui_rect global_rect;

#endif