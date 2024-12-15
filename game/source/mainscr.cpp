#include <minitui.h>
#include <mainscr.h>

mainscr::mainscr(
  tui_rect area
) : tui_canvas(area) {
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
    else if (kbd_event->check_key('p')) {
      create_widget(new tui_ncanvas(
        tui_rect(tui_point(SCR_HEIGHT - 5, SCR_WIDTH / 2), area.tail)
      ));
      delete event;
      return NULL;
    }
    else if (kbd_event->check_key('r')) {
      time_counter = 0;
      // this->set_time_counter(tui_formatter());
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
  print_content(tui_point(x, y), true, formatter, message);
  set_time_counter(formatter);
  tui_timer::append(new tui_timer(
    this,
    1000,
    [formatter](tui_widget *widget) {
      mainscr *self = (mainscr *) widget;
      self->time_counter++;
      self->set_time_counter(formatter);
    },
    true
  ));
}

void
mainscr::set_time_counter(tui_formatter formatter) {
  int x = area.height() / 2;
  int y = (area.width() - 4) / 2;
  print_content(tui_point(x + 1, y), true, formatter, tui_fmt("%02d:%02d", time_counter / 60, time_counter % 60)());
  return ;
}