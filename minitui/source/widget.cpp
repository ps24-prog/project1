#include <minitui.h>

tui_widget::tui_widget(
  tui_rect area,
  tui_widget *parent
) : area(area)
  , updated(false)
  , instaniated(false)
  , parent(parent) 
  , retcode(0)
  , livecnt(1)
  , deleted(false) {
  children.clear(); 
}

tui_point
tui_widget::position_interpreter (
  tui_point point
) const {
  if (!point.is_in(area)) {
    Warn("Point not in widget!");
  }
  // point.rect->log_rect();
  assert(point.is_valid());
  return point - area.head;
}

tui_point
tui_widget::position_mapper (
  tui_point local_point
) const {
  auto res_point = local_point + area.head;
  if (!res_point.is_in(area)) {
    Warn("Point not in widget!");
  }
  return res_point;
}

void 
tui_widget::add_widget (
  tui_widget *widget
) {
  widget->parent = this;
  children.push_back(widget);
}

void 
tui_widget::create_widget (
  tui_widget *widget
) {
  add_widget(widget);
  tui_reg_widget(widget);
}

void
tui_widget::remove_widget (
  tui_widget *widget
) {
  for (auto it = children.begin(); it != children.end(); it++) {
    if (*it == widget) {
      children.erase(it);
      break;
    }
  }
}

// event blackhole
tui_event *
tui_widget::on_event (
  tui_event *event
) {
  if (!event) {
    Warn("A NULL event given!");
  }
  delete event;
  return NULL;
}

void
tui_widget::draw (
  tui_point point
) const {
  ANSI_CMD(ANSI_RST);
  putchar(' ');
  ANSI_CMD(ANSI_RST);
}

tui_widget::~tui_widget() {
  Debug("Widget %p deleted", this);
}

tui_event *
tui_widget::on_child_exit (
  tui_widget *child
) {
  // if not root, do nothing
  if (this != root) {
    return NULL;
  }
  if (children.size() == 1) {
    return new tui_event(
      TUI_EXIT_EVENT,
      new tui_exit_event(child->retcode)
    );
  }
  return NULL;
}

void
tui_widget::delete_widget (
  tui_widget *widget,
  bool hard_delete
) {
  Debug("%s deleting Widget %p with livecnt %d", hard_delete ? "hard" : "soft", widget, widget->livecnt);
  if (hard_delete && !widget->deleted) {
    widget->deleted = true;
    tui_erase_widget(widget);
    if (!widget->parent) {
      tui_assert(root == widget);
      tui_assert(widget->children.empty());
      Debug("Root widget deleted");
    } else {
      // children become orphans
      // make them root's children
      for (auto child : widget->children) {
        root->add_widget(child);
      }
      // will be called by event loop
      // widget->parent->on_child_exit(widget);
      // remove from parent's children list
      widget->parent->remove_widget(widget);
      widget->parent = NULL;
      widget->children.clear();
    }
  }
  
  if ((--widget->livecnt) == 0) {
    assert(widget->deleted);
    delete widget;
  }
}

tui_widget *root;

void tui_widget_init() {
  root = new tui_widget(global_rect);
  tui_reg_widget(root);
}