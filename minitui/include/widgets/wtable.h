#ifndef WTABLE_H__
#define WTABLE_H__

#include "../widget.h"

enum tui_wtable_border_style {
  WTABLE_NO_BORDER = 0,
  WTABLE_INNER_BORDER = 1,
  WTABLE_OUTER_BORDER = 2,
  WTABLE_FULL_BORDER = 3
};

struct tui_wtable : public tui_widget {
  tui_widget ***widgets, *local_focus;
  int row, col;
  tui_wtable_border_style border_style;
  int *length_row, *length_col;
  int length_row_tot, length_col_tot;

  tui_wtable(tui_rect area, int row, int col, int *length_row=NULL, int *length_col=NULL, tui_wtable_border_style=WTABLE_NO_BORDER, tui_widget *parent=NULL)
  : tui_widget(area, parent), row(row), col(col), border_style(border_style) {
    widgets = (tui_widget ***) malloc(sizeof(tui_widget **) * row);
    for (int i = 0; i < row; i++) {
      widgets[i] = (tui_widget **) calloc(col, sizeof(tui_widget *));
    }
    length_row_tot = area.height();
    length_col_tot = area.width();
    if (border_style & WTABLE_OUTER_BORDER) {
      length_col_tot -= 2;
      length_row_tot -= 2;
    }
    if (border_style & WTABLE_INNER_BORDER) {
      length_col_tot -= col - 1;
      length_row_tot -= row - 1;
    }
    this->length_row = (int *) malloc(sizeof(int) * row);
    if (length_row) {
      memcpy(this->length_row, length_row, sizeof(int) * row);
    } else {
      for (int i = 0; i < row; i++) {
        this->length_row[i] = length_row_tot / row;
      }
    }
    this->length_col = (int *) malloc(sizeof(int) * col);
    if (length_col) {
      memcpy(this->length_col, length_col, sizeof(int) * col);
    } else {
      for (int i = 0; i < col; i++) {
        this->length_col[i] = length_col_tot / col;
      }
    }
  }


};

#endif