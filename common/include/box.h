#ifndef BOX_H__
#define BOX_H__

enum tui_border_style {
  BORDER_LIGHT,
  BORDER_HEAVY,
  BORDER_STYLE_NR
};

enum tui_border_type {
  BORDER_HORIZONTAL,
  BORDER_VERTICAL,
  BORDER_UP_AND_RIGHT,
  BORDER_UP_AND_LEFT,
  BORDER_DOWN_AND_RIGHT,
  BORDER_DOWN_AND_LEFT,
  BORDER_HORIZONTAL_AND_UP,
  BORDER_HORIZONTAL_AND_DOWN,
  BORDER_VERTICAL_AND_RIGHT,
  BORDER_VERTICAL_AND_LEFT,
  BORDER_HORIZONTAL_AND_VERTICAL,
  BORDER_TYPE_NR
};

bool operator + (tui_border_type a, tui_border_type b);

const char* box_drawing(tui_border_type border_type, tui_border_style border_style=BORDER_LIGHT);

extern const char half_block[];

#endif



