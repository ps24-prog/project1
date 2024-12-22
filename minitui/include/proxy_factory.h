#ifndef PROXY_FACTORY_H__
#define PROXY_FACTORY_H__

#include <minitui.h>

// #define proxy_create(TP) new TP(create<T>(TP::area_transform(area, curr_args), attrs, args...), curr_args)

struct tui_proxy_factory {

  static tui_widget *proxy_add(tui_widget *widget, const char *proxy_name, std::vector<tui_fmt> args) {
    for (auto &proxy : proxy_create_list) {
      // Debug("Now Proxy name: %s", proxy.first());
      if (proxy.first == proxy_name) {
        return proxy.second(widget, args);
      }
    }
    Warn("Proxy not found: %s", proxy_name);
    return widget;
  }

  static tui_rect proxy_area_transform(tui_rect area, const char *proxy_name, std::vector<tui_fmt> args) {
    for (auto &proxy : proxy_area_transform_list) {
      if (proxy.first == proxy_name) {
        return proxy.second(area, args);
      }
    }
    Warn("Proxy not found: %s", proxy_name);
    return area;
  }

  template <typename T, typename... Args>
  static tui_widget *create(tui_rect area, std::vector<tui_fmt> attrs, Args... args) {
    if (attrs.empty()) {
      return new T(area, args...);
    }
    auto attr = attrs.back();
    attrs.pop_back();
    tui_fmt proxy_name(strtok(attr(), " "));
    // Debug("Proxy name: %s", proxy_name());
    std::vector<tui_fmt> curr_args{};
    char *attr_str;
    while ((attr_str = strtok(NULL, " "))) {
      curr_args.push_back(tui_fmt(attr_str));
    }

    auto new_area = proxy_area_transform(area, proxy_name(), curr_args);
    auto inner_widget = create<T>(new_area, attrs, args...);
    return proxy_add(inner_widget, proxy_name(), curr_args);
  }

  static tui_widget *add(tui_widget *widget, std::vector<tui_fmt> attrs) {
    if (attrs.empty()) {
      return widget;
    }
    tui_widget *res = widget;
    for (auto attr : attrs) {
      // parse the proxy name in tui_fmt attr
      tui_fmt proxy_name(strtok(attr(), " "));
      Debug("Proxy name: %s", proxy_name());
      
      // parse the attributes in tui_fmt attr
      std::vector<tui_fmt> curr_args{};
      char *attr_str;
      while ((attr_str = strtok(NULL, " "))) {
        curr_args.push_back(tui_fmt(attr_str));
      }
      res = proxy_add(res, proxy_name(), curr_args);
    }
    return res;
  }
};

#endif