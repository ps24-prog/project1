#include <minitui.h>

factory_reg_impl(tui_title_proxy);

tui_title_proxy::tui_title_proxy(tui_widget *content, std::vector<tui_fmt> args)
: tui_widget_proxy(content) {
  args.clear();
  strcpy(name, "TITLE_PROXY");
}

void tui_title_proxy::draw(tui_point point) const {
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
