#include <minitui.h>

tui_ncanvas::tui_ncanvas(
  tui_rect area,
  tui_widget *parent
) : tui_widget(area, NULL, parent), boff(0) { 
  strcpy(name, "NCANVAS");
  tui_timer::append(new tui_timer(this, 1000, [](tui_widget *widget) -> void {
    tui_ncanvas *ncanvas = (tui_ncanvas *) widget;
    ncanvas->boff += 16;
    ncanvas->set_updated();
  }, true));
}

tui_ncanvas::~tui_ncanvas() {
  
}

void
tui_ncanvas::draw(
  tui_point point
) const {
  uint8_t fr, fg, fb, br, bg, bb;
  // 渐变色，从左上到右下的图谱
  fr = point.x * 2 * 255 / (area.height() * 2) + boff;
  fg = 0;
  fb = point.y * 255 / area.width() + boff;
  br = (point.x * 2 + 1) * 255 / (area.height() * 2) + boff;
  bg = 0;
  bb = point.y * 255 / area.width() + boff;

  auto formatter = tui_formatter(fr, fg, fb, br, bg, bb);

  formatter.set();
  printf(half_block);
}

tui_event *
tui_ncanvas::on_event(
  tui_event *event
) {
  Debug("Triggered event on ncanvas");
  if (!event || !event->event_body) {
    Error("Invalid event given!");
  }
  if (event->check_key('i')) {
    boff += 16;
    set_updated();
  } else if (event->event_type == TUI_MOUSE_EVENT) {
    auto mouse_event = (tui_mouse_event *) event->event_body;
    // this->area.log_rect();
    if (!mouse_event->get_point().is_in(this->area))
      return event;
    
    if (mouse_event->type == MOUSE_LEFT_CLICK && mouse_event->ispress) {
      boff += 16;
      set_updated();
    } else if (mouse_event->type == MOUSE_RIGHT_CLICK && mouse_event->ispress) {
      boff -= 16;
      set_updated();
    }
  }
  FREE_EVENT(event);
  return NULL;
}