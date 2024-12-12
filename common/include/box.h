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
  BORDER_DOWN_AND_LEFT,
  BORDER_DOWN_AND_RIGHT,
  BORDER_UP_AND_LEFT,
  BORDER_UP_AND_RIGHT,
  BORDER_HORIZONTAL_AND_DOWN,
  BORDER_HORIZONTAL_AND_UP,
  BORDER_VERTICAL_AND_LEFT,
  BORDER_VERTICAL_AND_RIGHT,
  BORDER_TYPE_NR
};

const char* box_drawing(tui_border_type border_type, tui_border_style border_style=BORDER_LIGHT);

extern const char half_block[];

#endif



