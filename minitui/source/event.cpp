// An input FSM

#include <ansi.h>
#include <debug.h>
#include <minitui.h>
#include <basics.h>
#include <ui.h>

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

      auto event = new tui_event(
        TUI_KEYBD_EVENT, 
        new tui_kbd_event(ch - 'A')
      );
      
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

      auto event = new tui_event(TUI_MOUSE_EVENT, new tui_mouse_event(
        arg_type.first, 
        arg_x.first - 1, 
        arg_y.first - 1, 
        arg_x.second == 'M'
      ));
      tui_mouse_event::log_mouse_event((tui_mouse_event *) event->event_body);
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

  char ch = tui_getchar();
  switch (ch)
  {
  // ESC
  case ESC_CODE: {
    return tui_get_ansi_event();
  }

  case '\x3': {
    return new tui_event(
      TUI_EXIT_EVENT, 
      new tui_exit_event(0)
    );
  }

  default: {
    Debug("Get a key %d!", ch);
    return new tui_event(
      TUI_KEYBD_EVENT, 
      new tui_kbd_event(ch)
    );
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

  int final_retcode = 0;

  Debug("start game mainloop");
  
#ifdef TEST_MODE
  Info("Test mode");
  return tui_test_exec();
#endif

  // There should be a root widget
  if (!wl_head) {
    Error("No root widget");
  }
  // set root widget as default focus
  focus = wl_head->body;

  while (true) {
    tui_draw();

    if (gbl_event_queue.empty()) {
      continue;
    }

    tui_event *event = gbl_event_queue.pop_event();
    if (!event) {
      Error("Event is NULL");
    }
    // tui_event::log_event(event, false);
    
    switch (event->event_type) {
      case TUI_EXIT_EVENT: {
        Debug("Get a exit event");
        final_retcode = \
        ((tui_exit_event *) event->event_body)->retcode;
        delete event;
        return final_retcode;
      }
      case TUI_TIMER_INTERUPTER_EVENT: {
        tui_timer::handle();
        delete event;
        continue;
      }
      case TUI_MOUSE_EVENT: {
        if (mouse_enabled) {
          auto mouse_event = (tui_mouse_event *) event->event_body;
          tui_point mouse_point = mouse_event->get_point();
          if ((mouse_event->type == MOUSE_LEFT_CLICK
            || mouse_event->type == MOUSE_RIGHT_CLICK 
            || mouse_event->type == MOUSE_MID_CLICK) && mouse_event->ispress)
            tui_focus_on(mouse_point);
        } else {
          delete event;
          continue;
        }
        break;
      }
    }

    tui_widget *current = focus;

    while (current && event) {
      // invariant: event is not NULL and not TUI_EXIT_EVENT
      assert(event);
      assert(event->event_type != TUI_EXIT_EVENT);
      
      event = current->on_event(event);
      tui_widget *next = current->parent;
      // invariant: event is targeted to next widget
      // next event is TUI_EXIT_EVENT, process next widget in advance
      while (event && event->event_type == TUI_EXIT_EVENT) {
        // invariant: event is TUI_EXIT_EVENT
        // invariant: the widget to be deleted is the current widget
        if (next) {
          current->retcode = \
          ((tui_exit_event *) event->event_body)->retcode;
          delete event;
          // next widget should process the exit event
          event = next->on_child_exit(current);
          // delete current and update `current` & `next` to meet the invariant
          assert(!current->deleted);
          tui_widget::delete_widget(current, true);
          current = next;
          next = next->parent;
        } else {
          tui_assert(current == root);
          final_retcode = \
          ((tui_exit_event *) event->event_body)->retcode;
          tui_widget::delete_widget(current, true);
          current = next = root = NULL;
          delete event;
          event = NULL;
          break;
        }
      }
      // invariant: event is not TUI_EXIT_EVENT 
      // invariant: event is targeted to next widget
      current = next;
    }

    // root widget exits, end the exection loop
    if (!root) {
      return final_retcode;
    }

    delete event;
  }
  // Impossible
  return -1;
}

tui_event::~tui_event() {
  switch(event_type) {
    case TUI_PASTE_EVENT: {
      delete ((tui_paste_event *) event_body);
      break;
    }
    default: {
      // only free memory allocated, no destruction function called
      // free(NULL) is valid
      free(event_body);
      break;
    }
  }
}