#include <event.h>
#include <debug.h>

static 
void 
tui_timer_interrupter() {
  while (true) {
    std::this_thread::sleep_for(std::chrono::milliseconds(20));
    tui_event *event = new tui_event(
      TUI_TIMER_INTERUPTER_EVENT,
      NULL
    );
    gbl_event_queue.push_event(event);
  }
}

static 
void 
tui_keyboard_interrupter() {
  while (true) {
    auto event = tui_get_event();
    if (event) {
      auto point = event->check_mouse_move();
      auto ex_point = tui_get_gbl_mouse_point();
      if (point != tui_point(-1, -1)) {
        gbl_event_queue.push_event(
          new tui_event(
            TUI_MOUSE_EVENT,
            new tui_mouse_event(
              MOUSE_FADE_OUT,
              ex_point.x, ex_point.y, false
            )
          )
        );
        tui_set_gbl_mouse_point(point);
      }
      gbl_event_queue.push_event(event);
    }
    else {
      Warn("Event is NULL");
    }
  }
}

void 
tui_threads_init() {
  std::thread t_timer(tui_timer_interrupter);
  std::thread t_keyboard(tui_keyboard_interrupter);
  t_timer.detach();
  t_keyboard.detach();
}