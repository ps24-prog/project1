#include <minitui.h>
#include <debug.h>
#include <basics.h>

tui_canvas::tui_canvas(
  tui_rect area,
  tui_widget *parent
) : tui_widget(area, parent) { 
  content = (char *) calloc(area.height() * area.width(), sizeof(char));
  memset(content, ' ', area.height() * area.width() * sizeof(char));
  formatter = (tui_formatter *) calloc(area.height() * area.width(), sizeof(tui_formatter));
}

tui_canvas::~tui_canvas() {
  free(content);
  free(formatter);
}

tui_canvas::tui_canvas_line
tui_canvas::operator [] (
  int x
) const {
  if (x < 0 || x >= area.height()) {
    Warn("Height exceeded!");
  }
  auto res = (tui_canvas_line) {
    .length = area.width(),
    .content = content + x * area.width(),
    .formatter = formatter + x * area.width()
  };
  return res;
}

tui_canvas::tui_canvas_unit
tui_canvas::tui_canvas_line::operator [] (
  int y
) const {
  if (y < 0 || y >= length) {
    Warn("Width exceeded!");
  }
  auto res = (tui_canvas_unit) {
    .content = content + y,
    .formatter = formatter + y
  };
  return res;
}

void
tui_canvas::draw(
  tui_point point
) const {
  ANSI_CMD(ANSI_RST);

  auto canvas_unit = (*this)[point.x][point.y];
  auto formatter = *canvas_unit.formatter;

  formatter.set();
  putchar(*canvas_unit.content);
  
  ANSI_CMD(ANSI_RST);
}