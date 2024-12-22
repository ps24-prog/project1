#include <minitui.h>
#include <basics.h>
#include <ansi.h>

const char tui_msgbox::hint[] = "[Y]es, (N)o, (C)ancel";

tui_msgbox::tui_msgbox(
  tui_rect area,
  tui_widget *parent,
  char *message
) : tui_widget(area, NULL, parent)
  , message(message)
  , msg_length(strlen(message)) {
}

// @param event triggered event
// @return event for parents
tui_event *
tui_msgbox::on_event(
  tui_event *event
) {
  if (!event) {
    Warn("A NULL event given!");
  }
  if (event->event_type == TUI_KEYBD_EVENT) {
    if (!event->event_body) {
      Warn("An empty event given!");
    }
    auto kbd_event = (tui_kbd_event *) event->event_body;
    char code = kbd_event->code;
    if (code >= 'A' && code <= 'Z') code += 'a' - 'A';
    switch (code)
    {
    case 'y': case 'n':  case 'c': {
      delete event;
      return new tui_event(TUI_EXIT_EVENT, new tui_exit_event(code));
    }
    default:
      break;
    }
  }
  if (event->event_body)
    free(event->event_body);
  free(event);
  return NULL;
}

// @param point location relative to up-left corner
void
tui_msgbox::draw(
  tui_point point
) const {
  ANSI_CMD(ANSI_RST);
  if (point.x == area.height() - 1) {
    if (point.y < (int) sizeof(hint) - 1) {
      putchar(hint[point.y]);
    } else {
      putchar(' ');
    }
  } else {
    int pos = point.x * area.width() + point.y;
    if (pos < msg_length)
      putchar(message[pos]);
    else putchar(' ');
  }
  ANSI_CMD(ANSI_RST);
}

