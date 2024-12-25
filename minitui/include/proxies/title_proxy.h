#ifndef TITLE_PROXY_H__
#define TITLE_PROXY_H__

#include <minitui.h>

struct tui_title_proxy : public tui_widget_proxy {
  factory_reg(tui_title_proxy, "title");

  tui_title_proxy(tui_widget *content, std::vector<tui_fmt> args=std::vector<tui_fmt>());

  void draw(tui_point point) const override;

  ~tui_title_proxy() {};
};

#endif