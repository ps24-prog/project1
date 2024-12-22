#include <box.h>
#include <debug.h>

const char half_block[] = "\u2580";

const char *box_drawings[BORDER_STYLE_NR][BORDER_TYPE_NR] = {
  [BORDER_LIGHT] = {
    [BORDER_HORIZONTAL] = "\u2500",
    [BORDER_VERTICAL] = "\u2502",
    [BORDER_UP_AND_RIGHT] = "\u2514",
    [BORDER_UP_AND_LEFT] = "\u2518",
    [BORDER_DOWN_AND_RIGHT] = "\u250c",
    [BORDER_DOWN_AND_LEFT] = "\u2510",
    [BORDER_HORIZONTAL_AND_UP] = "\u2534",
    [BORDER_HORIZONTAL_AND_DOWN] = "\u252c",
    [BORDER_VERTICAL_AND_RIGHT] = "\u251c",
    [BORDER_VERTICAL_AND_LEFT] = "\u2524",
  },
  [BORDER_HEAVY] = {
    [BORDER_HORIZONTAL] = "\u2501",
    [BORDER_VERTICAL] = "\u2503",
    [BORDER_UP_AND_RIGHT] = "\u2516",
    [BORDER_UP_AND_LEFT] = "\u251a",
    [BORDER_DOWN_AND_RIGHT] = "\u250f",
    [BORDER_DOWN_AND_LEFT] = "\u2513",
    [BORDER_HORIZONTAL_AND_UP] = "\u2537",
    [BORDER_HORIZONTAL_AND_DOWN] = "\u252f",
    [BORDER_VERTICAL_AND_RIGHT] = "\u2520",
    [BORDER_VERTICAL_AND_LEFT] = "\u2528",
  },
};


static bool is_left(tui_border_type a) {
  return a == BORDER_UP_AND_LEFT || a == BORDER_DOWN_AND_LEFT || a == BORDER_VERTICAL_AND_LEFT;
}
static bool is_right(tui_border_type a) {
  return a == BORDER_UP_AND_RIGHT || a == BORDER_DOWN_AND_RIGHT || a == BORDER_VERTICAL_AND_RIGHT;
}
static bool is_up(tui_border_type a) {
  return a == BORDER_UP_AND_LEFT || a == BORDER_UP_AND_RIGHT || a == BORDER_HORIZONTAL_AND_UP;
}
static bool is_down(tui_border_type a) {
  return a == BORDER_DOWN_AND_LEFT || a == BORDER_DOWN_AND_RIGHT || a == BORDER_HORIZONTAL_AND_DOWN;
}
static bool is_vertical(tui_border_type a) {
  return a == BORDER_VERTICAL || a == BORDER_VERTICAL_AND_LEFT || a == BORDER_VERTICAL_AND_RIGHT || a == BORDER_HORIZONTAL_AND_VERTICAL;
}
static bool is_horizontal(tui_border_type a) {
  return a == BORDER_HORIZONTAL || a == BORDER_HORIZONTAL_AND_DOWN || a == BORDER_HORIZONTAL_AND_UP || a == BORDER_HORIZONTAL_AND_VERTICAL;
}

tui_border_type border_add(tui_border_type a, tui_border_type b) {
  bool has_up = is_vertical(a) || is_vertical(b) || is_up(a) || is_up(b);
  bool has_down = is_vertical(a) || is_vertical(b) || is_down(a) || is_down(b);
  bool has_left = is_horizontal(a) || is_horizontal(b) || is_left(a) || is_left(b);
  bool has_right = is_horizontal(a) || is_horizontal(b) || is_right(a) || is_right(b);
  
  if (has_up && has_down && has_left && has_right) {
    return BORDER_HORIZONTAL_AND_VERTICAL;
  } else if (has_up && has_down && has_left) {
    return BORDER_HORIZONTAL_AND_UP;
  } else if (has_up && has_down && has_right) {
    return BORDER_HORIZONTAL_AND_DOWN;
  } else if (has_up && has_left && has_right) {
    return BORDER_VERTICAL_AND_LEFT;
  } else if (has_down && has_left && has_right) {
    return BORDER_VERTICAL_AND_RIGHT;
  } else if (has_up && has_down) {
    return BORDER_VERTICAL;
  } else if (has_left && has_right) {
    return BORDER_HORIZONTAL;
  } else if (has_up && has_left) {
    return BORDER_UP_AND_LEFT;
  } else if (has_up && has_right) {
    return BORDER_UP_AND_RIGHT;
  } else if (has_down && has_left) {
    return BORDER_DOWN_AND_LEFT;
  } else if (has_down && has_right) {
    return BORDER_DOWN_AND_RIGHT;
  }

  Error("Invalid border type combination");
}

const char* box_drawing(tui_border_type border_type, tui_border_style border_style) {
  return box_drawings[border_style][border_type];
}
