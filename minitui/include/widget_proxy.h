#ifndef WIDGET_PROXY_H__
#define WIDGET_PROXY_H__

#include <minitui.h>

extern std::vector <std::pair <tui_fmt, std::function <tui_widget *(tui_widget *, std::vector<tui_fmt>)>>> proxy_create_list;
extern std::vector <std::pair <tui_fmt, std::function <tui_rect(tui_rect, std::vector<tui_fmt>)>>> proxy_area_transform_list;

template <typename T>
inline void register_proxy(const char *name) {
  proxy_create_list.push_back(std::make_pair(
    tui_fmt(name),
    T::create
  ));
  proxy_area_transform_list.push_back(std::make_pair(
    tui_fmt(name),
    static_cast<tui_rect (*)(tui_rect, std::vector<tui_fmt>)>(T::area_transform)
  ));
}

#define factory_reg(T, name) \
  static tui_widget *create(tui_widget *content, std::vector<tui_fmt> args=std::vector<tui_fmt>()) { \
    return new T(content, args); \
  } \
  struct T##_reg { \
    T##_reg() { \
      register_proxy<T>(name); \
    } \
  }; \
  static T##_reg T##_reg_instance;

#define factory_reg_impl(T) \
  T::T##_reg T::T##_reg_instance;

struct tui_widget_proxy : public tui_widget {
  factory_reg(tui_widget_proxy, "PROXY");

  tui_widget *content;
  tui_widget_proxy(tui_widget *content, std::vector<tui_fmt> args=std::vector<tui_fmt>());

  static tui_rect area_transform(tui_rect area);
  static tui_rect area_transform(tui_rect area, std::vector<tui_fmt> args);

  void draw(tui_point point) const override;
  tui_event *on_event(tui_event *event) override;
  tui_event *on_child_exit(tui_widget *child) override;
  void update() override;
  tui_widget *proxy() const override;
  ~tui_widget_proxy();
};

#endif