#include <minitui.h>

void 
tui_mouse_event::log_mouse_event(
  tui_mouse_event *mouse_event, 
  bool trace
) {
  if (trace) {
    Trace("mouse event %s type: %d (%d, %d)", mouse_event->ispress ? "press" : "release", mouse_event->type, mouse_event->x, mouse_event->y);
  } else {
    Debug("mouse event %s type: %d (%d, %d)", mouse_event->ispress ? "press" : "release", mouse_event->type, mouse_event->x, mouse_event->y);
  }
  return ;
}