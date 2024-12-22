#include <minitui.h>

std::vector <std::pair <tui_fmt, std::function <tui_widget *(tui_widget *, std::vector<tui_fmt>)>>> proxy_create_list;
std::vector <std::pair <tui_fmt, std::function <tui_rect(tui_rect, std::vector<tui_fmt>)>>> proxy_area_transform_list;

factory_reg_impl(tui_widget_proxy);

tui_widget_proxy::tui_widget_proxy(
  tui_widget *content,
  std::vector<tui_fmt> args
) : tui_widget(content->area) {
  args.clear();
  strcpy(name, "PROXY");
  // tui_assert(!content->instaniated);
  // tui_assert(!content->parent);
  // tui_assert(content->children.empty());
  this->content = content;
  content->proxize_widget(this);
  this->area = content->area;
  // area.log_rect();
  tui_assert(this->area.isvalid());
  
}

tui_rect 
tui_widget_proxy::area_transform(
  tui_rect area
) {
  return area;
}

tui_rect 
tui_widget_proxy::area_transform(
  tui_rect area, 
  std::vector<tui_fmt> args
) {
  args.clear();
  return area;
}

void 
tui_widget_proxy::draw(
  tui_point point
) const {
  content->draw(point);
}

tui_event *
tui_widget_proxy::on_event(
  tui_event *event
) {
  return content->on_event(event);
}

tui_widget *
tui_widget_proxy::proxy() const {
  return content;
}

tui_event *
tui_widget_proxy::on_child_exit(
  tui_widget *child
) {
  return content->on_child_exit(child);
}

tui_widget_proxy::~tui_widget_proxy() {
  tui_widget::delete_widget(content, true);
}