#include <minitui.h>

tui_widget::tui_widget(
  tui_rect area,
  const char *name,     // not recommend to use
  tui_widget *parent    // not recommend to use
) 
: area(area)
, updated(false)
, instaniated(false)
, reset_block(false)
, parent(parent) 
, unproxy(NULL)
, frame(NULL)
, retcode(0)
, livecnt(1)
, deleted(false) {
  if (!name) {
    strcpy(this->name, "DEFAULT_WIDGET");
  }
  else {
    strcpy(this->name, name);
  }
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

tui_widget *
tui_widget::add_widget (
  tui_widget *widget
) {
  // a not instaniated widget cannot have children
  tui_assert(instaniated);
  widget->parent = this;
  children.push_back(widget);
  return widget;
}

tui_widget *
tui_widget::create_widget (
  tui_widget *widget
) {
  tui_reg_widget(widget);
  return add_widget(widget);
}

tui_widget *
tui_widget::set_attrs (
  std::vector<tui_fmt> attrs
) {
  return tui_proxy_factory::add(this, attrs);
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
  putchar(' ');
}

tui_widget::~tui_widget() {
  Debug("Widget %p %s deleted", this, this->name);
}

tui_event *
tui_widget::on_child_exit (
  tui_widget *child
) {
  Debug("Child %d %p %s exited", children.size(), child, child->name);
  // if not root, do nothing
  if (this->unproxy_penetrator() != root) {
    return NULL;
  }
  if (this->get_children().size() == 1 && child->children.empty()) {
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
  Debug("%s deleting Widget %p %s with livecnt %d", hard_delete ? "hard" : "soft", widget, widget->name, widget->livecnt);
  if (hard_delete && !widget->deleted) {
    widget->deleted = true;
    if (widget->instaniated) {
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
  }
  
  if ((--widget->livecnt) == 0) {
    assert(widget->deleted);
    delete widget;
  }
}

void
tui_widget::transform_widget(
  tui_widget *target
) {
  if (this == root) {
    Debug("Transforming root widget to %s", target->name);
    root = target;
  }
  Debug("Transforming widget %s to %s", this->name, target->name);
  tui_widget *widget = this;
  if (widget->instaniated) {
    tui_update_widget(widget, target);
  }
  if (widget->parent) {
    widget->parent->remove_widget(widget);
    widget->parent->add_widget(target);
    target->parent = widget->parent;
    widget->parent = NULL;
  }
  if (widget->frame) {
    // TODO
    target->frame = widget->frame;
    widget->frame = NULL;
  }
  if (!widget->children.empty()) {
    printf("Transforming children of %s\n", widget->name);
    for (auto child : widget->children) {
      target->add_widget(child);
      // do not erase child here, or the iterator will become invalid
    }
    widget->children.clear();
  }
}

void
tui_widget::proxize_widget(
  tui_widget *proxy
) {
  this->unproxy = proxy;
  tui_assert(proxy->proxy() == this);
  transform_widget(proxy);
}

// no proxy
tui_widget *
tui_widget::proxy() const {
  return NULL;
}

tui_widget *
tui_widget::proxy_penetrator() const {
  if (this->proxy()) {
    return this->proxy()->proxy_penetrator();
  }
  return (tui_widget *) this;
}

tui_widget *
tui_widget::unproxy_penetrator() const {
  if (this->unproxy) {
    return this->unproxy->unproxy_penetrator();
  }
  return (tui_widget *) this;
}

void
tui_widget::reset_area(
  tui_rect area
) {
  if (this->proxy()) {
    this->proxy()->reset_area(
      tui_rect(
        area.head + this->proxy()->area.head - this->area.head,
        area.tail + this->proxy()->area.tail - this->area.tail
      )
    );
  }
  this->area = area;
}

tui_widget *root;

void tui_widget_init() {
  root = new tui_widget(global_rect);
  strcpy(root->name, "ROOT");
  root->reset_block = true;
  tui_reg_widget(root);
}
