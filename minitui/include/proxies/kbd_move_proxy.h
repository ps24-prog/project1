#ifndef KBD_MOVE_PROXY_H__
#define KBD_MOVE_PROXY_H__

#include <minitui.h>

struct tui_kbd_move_proxy : public tui_widget_proxy {
  factory_reg(tui_kbd_move_proxy, "kbd_move");
  
  tui_kbd_move_proxy(tui_widget *content, std::vector<tui_fmt> args=std::vector<tui_fmt>())
  : tui_widget_proxy(content) {
    args.clear();
    strcpy(name, "KBD_MOVE_PROXY");
  }    

  tui_event *on_event(tui_event *event) override {
    if (event->check_key(KEY_ARROW_LEFT)) {
      delete event;
      if (this->unproxy_penetrator()->area.head.y > global_rect.head.y) {
        tui_reset_widget(
          this->unproxy_penetrator(), 
          tui_rect(
            this->unproxy_penetrator()->area.head + tui_point(0, -1),
            this->unproxy_penetrator()->area.tail + tui_point(0, -1)
          )
        );
      }
      return NULL;
    } else if (event->check_key(KEY_ARROW_DOWN)) {
      delete event;
      if (this->unproxy_penetrator()->area.tail.x < global_rect.tail.x) {
        tui_reset_widget(
          this->unproxy_penetrator(), 
          tui_rect(
            this->unproxy_penetrator()->area.head + tui_point(1, 0),
            this->unproxy_penetrator()->area.tail + tui_point(1, 0)
          )
        );
      }
      return NULL;
    } else if (event->check_key(KEY_ARROW_UP)) {
      delete event;
      if (this->unproxy_penetrator()->area.head.x > global_rect.head.x) {
        tui_reset_widget(
          this->unproxy_penetrator(), 
          tui_rect(
            this->unproxy_penetrator()->area.head + tui_point(-1, 0),
            this->unproxy_penetrator()->area.tail + tui_point(-1, 0)
          )
        );
      }
      return NULL;
    } else if (event->check_key(KEY_ARROW_RIGHT)) {
      delete event;
      if (this->unproxy_penetrator()->area.tail.y < global_rect.tail.y) {
        tui_reset_widget(
          this->unproxy_penetrator(), 
          tui_rect(
            this->unproxy_penetrator()->area.head + tui_point(0, 1),
            this->unproxy_penetrator()->area.tail + tui_point(0, 1)
          )
        );
      }
      return NULL;
    }
    return content->on_event(event);
  }
};

#endif