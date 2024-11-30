// An input FSM

#include <ansi.h>
#include <debug.h>
#include <minitui.h>
#include <basics.h>
#include <ui.h>
// tui_event_t 

bool mouse_enabled = true;

static
char
tui_getchar() {
  char ch = getchar();
  if (ch < '\x7f' && ch >= ' ')
    Trace("Get %c", ch);
  else
    Trace("Get %u", (uint8_t) ch);
  return ch;
}

static
std::pair<int, char>
tui_getansi_int() {
  int ret = 0;
  char ch = tui_getchar();
  while (isdigit(ch)) {
    ret = ret * 10 + ch - '0';
    ch = tui_getchar();
  }
  return std::make_pair(ret, ch);
}

static
tui_event *
tui_get_ansi_event() {
  Debug("Enter an ansi event");
  char ch = tui_getchar();
  switch (ch) 
  {
  // CSI
  case '[': {
    ch = tui_getchar();
    switch (ch) {
    case 'A':
    case 'B':
    case 'C':
    case 'D': {
      Debug("Get a arrow key event");
      auto event = CREATE_OBJ(tui_event);
      auto kbd_event = CREATE_OBJ(tui_kbd_event);
      
      event->event_type = TUI_KEYBD_EVENT;
      event->event_body = kbd_event;
      kbd_event->code = ch;
      return event;
    }
    case '<': {
      Debug("Get a mouse event");
      auto arg_type = tui_getansi_int();
      auto arg_y = tui_getansi_int();
      auto arg_x = tui_getansi_int();
      
      tui_assert(arg_type.second == ';');
      tui_assert(arg_y.second == ';');
      tui_assert(arg_x.second == 'm' || arg_x.second == 'M');

      auto mouse_event = new tui_mouse_event(
        arg_type.first, 
        arg_x.first, 
        arg_y.first, 
        arg_x.second == 'M'
      );
      mouse_event->log_mouse_event(mouse_event);
      auto event = new tui_event(TUI_MOUSE_EVENT, mouse_event);
      return event;
    }
    default: {
      if (isdigit(ch)) {
        while (isdigit(ch)) {
          ch = tui_getchar();
        }
        if (ch == '~') {
          Debug("Get a ^[d~ event");
          return NULL;
        }
        return NULL;
      } else if ((ch <= 'Z' && ch >= 'A') || (ch >= 'a' && ch <= 'z')) {
        return NULL;
      }
      return NULL;
    }
    } 
    return NULL;
  }
  case 'O': {
    ch = tui_getchar();
    return NULL;
  }
  default: {
    return NULL;
  }
  }
  return NULL;
}

tui_event *
tui_get_event() {
#ifdef _WIN64
  if (!_kbhit()) {
    return NULL;
  }
#endif
  char ch = tui_getchar();
  switch (ch)
  {
  // ESC
  case ESC_CODE: {
    return tui_get_ansi_event();
  }

  case '\x3': {
    auto event = CREATE_OBJ(tui_event);
    auto exit_event = CREATE_OBJ(tui_exit_event);
    
    event->event_type = TUI_EXIT_EVENT;
    event->event_body = exit_event;
    exit_event->retcode = 0;
    return event;
  }

  case 't': {
    auto event = CREATE_OBJ(tui_event);
    auto exit_event = CREATE_OBJ(tui_exit_event);
    
    event->event_type = TUI_EXIT_EVENT;
    event->event_body = exit_event;
    exit_event->retcode = 0;
    return event;
  }

  default: {
    Debug("Get a key!");
    auto event = CREATE_OBJ(tui_event);
    auto kbd_event = CREATE_OBJ(tui_kbd_event);

    event->event_type = TUI_KEYBD_EVENT;
    event->event_body = kbd_event;
    kbd_event->code = ch;
    return event;
  }

  }
  return NULL;
}

static
int
tui_test_exec() {
  while (true) {
    char ch = tui_getchar();
    if (ch == ESC_CODE) {
      auto event = tui_get_ansi_event();
      if (event->event_type == TUI_MOUSE_EVENT) {
        // log_mouse_event((tui_mouse_event *) event->event_body);
      }
    }
    if (ch == '\x3') {
      Info("Get a ^C");
      return 0;
    }
  }
}

int 
tui_exec() {

  Debug("start game mainloop");
  
#ifdef TEST_MODE
  Info("Test mode");
  return tui_test_exec();
#endif

  // There should be a top widget
  if (!wl_head) {
    Error("No top widget");
  }
  // set top widget as default focus
  focus = wl_head->body;

  while (true) {
    for (auto it = wl_head; it; it = it->next) {
      if (it->body->get_updated()) {
        tui_draw();
        break;
      }
    }

    tui_event *event = tui_get_event();
    if (!event) {
      // Debug("A NULL event returned.");
      continue;
    }
    
    if (event->event_type == TUI_EXIT_EVENT) {
      auto exit_event = (tui_exit_event *) event->event_body;
      return exit_event->retcode;
    }

    if (event->event_type == TUI_MOUSE_EVENT && !mouse_enabled) continue;

    // find the mouse's target
    if (event->event_type == TUI_MOUSE_EVENT) {
      auto mouse_event = (tui_mouse_event *) event->event_body;
      tui_point mouse_point = mouse_event->get_point();

      for (auto it = wl_head; it; it = it->next) {
        Debug("Checking %p", it->body);
        it->body->area.log_rect();
        if (mouse_point.is_in(it->body->area)) {
          // change focus if it is a click
          if ((mouse_event->type == MOUSE_LEFT_CLICK || mouse_event->type == MOUSE_RIGHT_CLICK || mouse_event->type == MOUSE_MID_CLICK) && mouse_event->ispress) {
            if (focus != it->body) {
              Debug("Change focus to %p", it->body);
            } else {
              Debug("Focus remains %p", it->body);
            }
            focus = it->body;
          }
          break;
        }
      }
    }

    tui_widget *current = focus;
    
    while (current && event) {
      event = current->on_event(event);
      tui_widget *next = current->parent;
      if (event && event->event_type == TUI_EXIT_EVENT) {
        tui_erase_widget(current);
        delete current;
      }
      current = next;
    }

    // top widget exits, end the exection loop
    if (event && event->event_type == TUI_EXIT_EVENT) {
      auto exit_event = (tui_exit_event *) event->event_body;
      return exit_event->retcode;
    }

    if (event) {
      if (event->event_body) free(event->event_body);
      free(event);
    }
  }
  // Impossible
  return -1;
}