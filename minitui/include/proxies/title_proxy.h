#ifndef TITLE_PROXY_H__
#define TITLE_PROXY_H__

#include <minitui.h>

struct tui_title_proxy : public tui_widget_proxy {
  factory_reg(tui_title_proxy, "title");

  tui_title_proxy(tui_widget *content, std::vector<tui_fmt> args=std::vector<tui_fmt>())
  : tui_widget_proxy(content) {
    args.clear();
    strcpy(name, "TITLE_PROXY");
  }

  void draw(tui_point point) const override {
    int title_start = area.width() / 2 - strlen(content->proxy_penetrator()->name) / 2;
    int title_end = title_start + strlen(content->proxy_penetrator()->name);
    if (title_start < 0 || title_end > area.width()) {
      title_start = title_end = 0;
    }
    if (point.x == 0 && point.y >= title_start && point.y < title_end) {
      putchar(content->proxy_penetrator()->name[point.y - title_start]);
    } else {
      content->draw(point);
    }
  }

  ~tui_title_proxy() {};
};

#endif