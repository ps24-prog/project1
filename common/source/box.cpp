#include <box.h>

const char half_block[] = "\u2580";

const char *box_drawings[BORDER_STYLE_NR][BORDER_TYPE_NR] = {
  [BORDER_LIGHT] = {
    [BORDER_HORIZONTAL] = "\u2500",
    [BORDER_VERTICAL] = "\u2502",
    [BORDER_DOWN_AND_LEFT] = "\u2514",
    [BORDER_DOWN_AND_RIGHT] = "\u2518",
    [BORDER_UP_AND_LEFT] = "\u250c",
    [BORDER_UP_AND_RIGHT] = "\u2510",
    [BORDER_HORIZONTAL_AND_DOWN] = "\u2534",
    [BORDER_HORIZONTAL_AND_UP] = "\u252c",
    [BORDER_VERTICAL_AND_LEFT] = "\u251c",
    [BORDER_VERTICAL_AND_RIGHT] = "\u2524",
  },
  [BORDER_HEAVY] = {
    [BORDER_HORIZONTAL] = "\u2501",
    [BORDER_VERTICAL] = "\u2503",
    [BORDER_DOWN_AND_LEFT] = "\u2516",
    [BORDER_DOWN_AND_RIGHT] = "\u251a",
    [BORDER_UP_AND_LEFT] = "\u250f",
    [BORDER_UP_AND_RIGHT] = "\u2513",
    [BORDER_HORIZONTAL_AND_DOWN] = "\u2537",
    [BORDER_HORIZONTAL_AND_UP] = "\u252f",
    [BORDER_VERTICAL_AND_LEFT] = "\u2520",
    [BORDER_VERTICAL_AND_RIGHT] = "\u2528",
  },
};


const char* box_drawing(tui_border_type border_type, tui_border_style border_style) {
  return box_drawings[border_style][border_type];
}
