#include <minitui.h>

factory_reg_impl(tui_border_proxy);

tui_border_proxy::tui_border_proxy(
  tui_widget *content,
  std::vector<tui_fmt> args
) : tui_widget_proxy(content) {
  
  this->content = content;
  if (args.size() != 0 && args[0] == "shrink") {
    // TODO: add shrinkable check
    content->reset_area(tui_rect(
      content->area.head + tui_point(1, 1), 
      content->area.tail - tui_point(1, 1)
    ));
  } else {
    if (this->instaniated) {
      tui_adjust_widget(this, tui_rect(
        content->area.head - tui_point(1, 1), 
        content->area.tail + tui_point(1, 1)
      ), false);
    } else {
      this->area = tui_rect(
        content->area.head - tui_point(1, 1), 
        content->area.tail + tui_point(1, 1)
      );
    }
  }
  
  
  tui_assert(this->area.isvalid());
  strcpy(name, "BORDER_PROXY");
}    

tui_rect
tui_border_proxy::area_transform(
  tui_rect area,
  std::vector<tui_fmt> args
) {
  args.clear();
  return tui_rect(
    area.head + tui_point(1, 1), 
    area.tail - tui_point(1, 1)
  );
}

void
tui_border_proxy::draw(
  tui_point point
) const {
  if (point.x == 0 || point.x == area.height() - 1) {
    if (point.y == 0 || point.y == area.width() - 1) {
      printf(box_drawing(
        point.x == 0 ? 
          point.y == 0 ?
            BORDER_DOWN_AND_RIGHT : BORDER_DOWN_AND_LEFT
        : point.y == 0 ?
            BORDER_UP_AND_RIGHT : BORDER_UP_AND_LEFT
      ));
    } else {
      printf(box_drawing(BORDER_HORIZONTAL));
    }
  } else if (point.y == 0 || point.y == area.width() - 1) {
    printf(box_drawing(BORDER_VERTICAL));
  } else {
    content->draw(point - tui_point(1, 1));
  }
}