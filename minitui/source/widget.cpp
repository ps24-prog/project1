#include <minitui.h>

tui_widget::tui_widget(
  tui_rect area,
  tui_widget *parent
) : area(area)
  , updated(false)
  , parent(parent) {
}

tui_point
tui_widget::position_interpreter (
  tui_point point
) const {
  if (!point.is_in(area)) {
    Warn("Point not in widget!");
  }
  return point - area.start;
}

tui_point
tui_widget::position_mapper (
  tui_point local_point
) const {
  auto res_point = local_point + area.start;
  if (!res_point.is_in(area)) {
    Warn("Point not in widget!");
  }
  return res_point;
}