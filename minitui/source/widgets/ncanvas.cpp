#include <minitui.h>

tui_ncanvas::tui_ncanvas(
  tui_rect area,
  tui_widget *parent
) : tui_widget(area, parent), boff(0) { 
}

tui_ncanvas::~tui_ncanvas() {
}

void
tui_ncanvas::draw(
  tui_point point
) const {
  ANSI_CMD(ANSI_RST);

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
  
  ANSI_CMD(ANSI_RST);
}

tui_event *
tui_ncanvas::on_event(
  tui_event *event
) {
  Debug("Triggered event on ncanvas");
  if (!event || !event->event_body) {
    Error("Invalid event given!");
  }
  if (event->event_type == TUI_KEYBD_EVENT) {
    auto kbd_event = (tui_kbd_event *) event->event_body;
    if (kbd_event->code == 'q' || kbd_event->code == 'Q') {
      FREE_EVENT_BODY(event);
      auto exit_event = CREATE_OBJ(tui_exit_event);
      exit_event->retcode = 0;
      
      event->event_type = TUI_EXIT_EVENT;
      event->event_body = exit_event;
      return event;
    } else if (kbd_event->code == 'i' || kbd_event->code == 'I') {
      boff += 16;
      set_updated();
    }
  } else if (event->event_type == TUI_MOUSE_EVENT) {
    auto mouse_event = (tui_mouse_event *) event->event_body;
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