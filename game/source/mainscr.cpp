#include <minitui.h>
#include <mainscr.h>

mainscr::mainscr(
  tui_rect area,
  tui_widget *parent
) : tui_canvas(area, parent) {
}

tui_event *
mainscr::on_event(
  tui_event *event
) {
  Debug("Triggered event on mainscr");
  if (!event || !event->event_body) {
    Error("Invalid event given!");
  }
  if (event->event_type == TUI_KEYBD_EVENT) {
    auto kbd_event = (tui_kbd_event *) event->event_body;
    if (kbd_event->check_key('q')) {
      delete event;
      return new tui_event(
        TUI_EXIT_EVENT, 
        new tui_exit_event(0)
      );
    }
    if (kbd_event->check_key('p')) {
      create_widget(new tui_ncanvas(
        (tui_rect) {(tui_point) {SCR_HEIGHT - 5, SCR_WIDTH / 2}, (tui_point) {SCR_HEIGHT - 1, SCR_WIDTH - 1}}
      ));
      delete event;
      return NULL;
    }
  }
  delete event;
  return NULL;
}

void
mainscr::set_global_formatter(
  tui_formatter formatter
) {
  for (int i = 0; i < area.height(); i++)
    for (int j = 0; j < area.width(); j++) {
      auto unit = (*this)[i][j];
      *(unit.formatter) = formatter;
    }
  set_updated();
  return ;
}

void 
mainscr::set_central_message(
  const char *message,
  tui_formatter formatter
) {
  int length = strlen(message);
  if (length >= area.width()) {
    Warn("The given message is too long!");
  }
  int x = area.height() / 2;
  int y = (area.width() - length) / 2;
  for (int i = 0; i < length; i++) {
    auto unit = (*this)[x][y+i];
    (*unit.content) = message[i];
    (*unit.formatter) = formatter;
  }
  set_updated();
}