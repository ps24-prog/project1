#ifndef BORDER_PROXY_H__
#define BORDER_PROXY_H__

#include <minitui.h>

struct border_proxy : public tui_widget {
  tui_widget *content;
  border_proxy(tui_widget *content) {
    this->content = content;
    this->area = tui_rect(
      content->area.head - tui_point(1, 1), 
      content->area.tail + tui_point(1, 1)
    );
    tui_assert(this->area.isvalid());
  }

  // a factory to create a widget with a border
  template <typename T, typename... Args>
  static border_proxy *create(tui_rect area, Args... args) {
    tui_rect inner_area = tui_rect(
      area.head + tui_point(1, 1), 
      area.tail - tui_point(1, 1)
    );
    return new border_proxy(new T(inner_area, args...));
  }

  void draw(tui_point point) const override {
    if (point.x == 0 || point.x == area.height() - 1) {
      if (point.y == 0 || point.y == area.width() - 1) {
        printf(box_drawing(
          point.x == 0 ? 
            point.y == 0 ?
              BORDER_UP_AND_LEFT : BORDER_UP_AND_RIGHT
          : point.y == 0 ?
              BORDER_DOWN_AND_LEFT : BORDER_DOWN_AND_RIGHT
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

  tui_event *on_event(tui_event *event) override {
    return content->on_event(event);
  }

  ~border_proxy() {
    tui_widget::delete_widget(content, true);
  }
};

#endif