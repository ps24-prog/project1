#ifndef BORDER_PROXY_H__
#define BORDER_PROXY_H__

#include <minitui.h>

struct tui_border_proxy : public tui_widget_proxy {
  factory_reg(tui_border_proxy, "border");

  tui_border_proxy(tui_widget *content, std::vector<tui_fmt> args=std::vector<tui_fmt>());

  static tui_rect area_transform(tui_rect area, std::vector<tui_fmt> args=std::vector<tui_fmt>());

  void draw(tui_point point) const override;
};

#endif