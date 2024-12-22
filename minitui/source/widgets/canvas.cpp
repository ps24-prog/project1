#include <minitui.h>
#include <debug.h>
#include <basics.h>

tui_canvas::tui_canvas(
  tui_rect area,
  tui_formatter default_formatter
) : tui_widget(area) { 
  content = new char[area.height() * area.width()];
  formatter = new tui_formatter[area.height() * area.width()];
  auto area_local = this->area.local_rect();
  for (auto point: area_local) {
    // Debug("canvas_init %d %d", point.x, point.y);
    update_content(
      point, 
      ' ', 
      default_formatter
    );
  }
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
tui_canvas::operator [] (
  tui_point point
) const {
  return (*this)[point.x][point.y];
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
  auto canvas_unit = (*this)[point];
  auto formatter = *canvas_unit.formatter;

  formatter.set();
  // Debug("%d %d %d", point.x, point.y, *canvas_unit.content);
  putchar(*canvas_unit.content);
}

void
tui_canvas::update_content(
  tui_point point,
  char content,
  tui_formatter formatter
) {
  // Debug("update_content called %d %d %d", point.x, point.y, content);
  auto unit = (*this)[point];
  *unit.content = content;
  *unit.formatter = formatter;
  set_updated();
}

tui_point
tui_canvas::print_content(
  tui_point point,
  bool linelimit,
  tui_formatter formatter,
  const char *content
) {
  // Debug("print_content called");
  tui_point point_copy = point;
  Debug("%d, %d", point.x, point.y);
  int length = strlen(content);
  auto local_area = area.local_rect();
  point.rect = &local_area;
  // Debug("%d, %d", point.x, point.y);
  // area.log_rect();
  for (int i = 0; i < length; i++) {
    // Debug("%d, %d", point.x, point.y);
    if (content[i] == '\n') {
      if (linelimit) {
        Warn("Line limit breaks!");
        break;
      }
      point.x++;
      point.y = 0;
      continue;
    }
    if (!isprint(content[i])) {
      continue;
    } 
    if (!point.is_valid()) {
      Warn("Point exceeded!");
      break;
    }
    if (linelimit && point.x != point_copy.x) {
      Warn("Line limit breaks!");
      break;
    }
    update_content(point, content[i], formatter);
    ++point;
  }
  point.rect = &global_rect;
  return point;
}