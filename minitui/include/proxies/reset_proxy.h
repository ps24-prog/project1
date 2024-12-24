#ifndef RESET_PROXY_H__
#define RESET_PROXY_H__

#include <minitui.h>
 
struct tui_reset_proxy : public tui_widget_proxy {
  factory_reg(tui_reset_proxy, "reset");

  tui_reset_proxy(tui_widget *content, std::vector<tui_fmt> args=std::vector<tui_fmt>()) : tui_widget_proxy(content) {
    args.clear();
    strcpy(name, "tui_reset_proxy");
  }

  tui_event *on_event(tui_event *event) override {
    if (event->check_key('r')) {
      // memory leak
      this->unproxy_penetrator()->transform_widget(content->proxy_penetrator());
      delete event;
      return NULL;
    }
    return content->on_event(event);
  }

  ~tui_reset_proxy() {};
};

#endif