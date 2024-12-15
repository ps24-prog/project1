#include <minitui.h>

void 
tui_mouse_event::log_mouse_event(
  const tui_mouse_event *mouse_event,
  bool trace
) {
  if (trace) {
    Trace("mouse event %s type: %d (%d, %d)", mouse_event->ispress ? "press" : "release", mouse_event->type, mouse_event->x, mouse_event->y);
  } else {
    Debug("mouse event %s type: %d (%d, %d)", mouse_event->ispress ? "press" : "release", mouse_event->type, mouse_event->x, mouse_event->y);
  }
  return ;
}

void 
tui_event::log_event(
  const tui_event *event,
  bool trace
) {
  switch (event->event_type) {
  case TUI_MOUSE_EVENT: {
    trace ? Trace("mouse event") : Debug("mouse event");
    break;
  }
  case TUI_KEYBD_EVENT: {
    auto kbd_event = (tui_kbd_event *) event->event_body;
    trace ? Trace("keyboard event") : Debug("keyboard event");
    break;
  }
  case TUI_EXIT_EVENT: {
    auto exit_event = (tui_exit_event *) event->event_body;
    trace ? Trace("exit event with retcode %d", exit_event->retcode) : Debug("exit event with retcode %d", exit_event->retcode);
    break;
  }
  case TUI_TIMER_INTERUPTER_EVENT: {
    trace ? Trace("timer interrupter event") : Debug("timer interrupter event");
    break;
  }
  default: {
    trace ? Trace("unknown event") : Debug("unknown event");
    break;
  }
  }
  return ;
}