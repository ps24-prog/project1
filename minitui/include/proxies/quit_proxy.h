#ifndef QUIT_PROXY_H__
#define QUIT_PROXY_H__

#include <minitui.h>

struct tui_quit_proxy : public tui_widget_proxy {
  factory_reg(tui_quit_proxy, "quit");

  char quit_key;

  tui_quit_proxy(tui_widget *content, char quit_key)
  : tui_widget_proxy(content), quit_key(quit_key) {
    strcpy(name, "QUIT_PROXY");
  }

  tui_quit_proxy(tui_widget *content, std::vector<tui_fmt> args)
  : tui_widget_proxy(content) {
    tui_assert(!args.empty());
    quit_key = atoi(args[0]());
    args.clear();
    Debug("quit key: %d", quit_key);
    strcpy(name, tui_fmt("%d_QUIT_PROXY", quit_key)());
  }

  tui_event *on_event(tui_event *event) override {
    tui_event* res = tui_event::exit_on_key(event, quit_key);
    if (!res) {
      return content->on_event(event);
    } else {
      return res;
    }
  }

  ~tui_quit_proxy() {};
};

#endif