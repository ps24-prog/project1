#include <minitui.h>

/*
  UI SYSTEM

  There is a linked list for all visible widgets called widget list, the head of the list is `wl_head`, the closer a widget is to the head, the higher it is in the screen (higher widgets will "cover" lower widgets). The list gives the height of widgets in the screen with contrast to the hierarchy given by widgets' parent-children relations.

  There is a 2D array of pointers to widgets of the same size as the screen, `full_map`, where the pointer in each postion points to the widget that is shown (i.e. highest) there.

  When drawing the screen, the `full_map` will be iterated, in each position, the shown widget's `draw()` method will be called **if it is updated**. Note that we flush the output after drawing the whole screen to speed up the process.

  When a widget is registered to show in the screen, it should be inserted to the head of the linked list (highest) and the `full_map` should be updated. Just set the element of full_map in each position of the new widget to point to the new widget, because it is the highest.

  When a widget is erased from the screen, it should be removed from the linked list, and the full_map should be updated. To find a new widget to show in a position, just iterate the widget list from head to tail, and pick the first widget that covers the position.

  Sometimes a widget need to change its position or size (namely, change its `area` rect). To achieve this, just erase the widget first, change its `area` rect and register it over again. It means that the widget will be reset to the top of the screen. `tui_reset_widget()` is a function that can be used to reset a widget without erasing it.

  The focus is the first target of events passed to widgets. When any widget is registered, the focus should be set to it, i.e., the widget in the `body` of `wl_head`. When a focused widget is erased, the focus will be set to its parent. If the parent is NULL, it means that the root focus is erased, then the program should exit as soon as possible. 
  
  Note that the implementation of `tui_reset_widget()` makes the reset operation (conducted when a widget changes size or move in the screen as mentioned above) will change the focus to the reset widget (and set it to the top of the screen). So it can also be used when a widget "gains" the focus -- just pass the same `area` as its current `area`.
*/

// now focus
tui_widget *focus;

tui_widget_list *wl_head;

long long draw_counter = 0;

static
void
tui_append_widget (
  tui_widget *widget
) {
  auto widget_list = new tui_widget_list;
  widget_list->body = widget;
  widget_list->next = wl_head;
  wl_head = widget_list;
}

static
void
tui_delete_widget (
  tui_widget *widget
) {
  if (widget == wl_head->body) {
    auto tmp_head = wl_head;
    wl_head = wl_head->next;
    delete tmp_head;
    return ;
  }
  
  auto it = wl_head;
  for (; it->next && it->next->body != widget; it = it->next) ;

  if (!it->next) {
    Warn("Try to delete a non-exist widget!");
  } else {
    auto tmp_target = it->next;
    it->next = it->next->next;
    delete tmp_target;
  }
}

tui_widget ***full_map;

static
tui_widget *&
full_map_at(tui_point point) {
  return full_map[point.x][point.y];
}

void 
tui_ui_init() {
  // get full_map
  full_map = (tui_widget ***) malloc(sizeof(tui_widget **) * scr_size.x);
  for (int i = 0; i < scr_size.x; i++) {
    full_map[i] = (tui_widget **) malloc(sizeof(tui_widget *) * scr_size.y);
  }
  global_rect = tui_rect(
    tui_point(0, 0), 
    tui_point(scr_size.x - 1, scr_size.y - 1)
  );
  return ;
}

static
void
tui_paste_widget(
  tui_widget *widget
) {
  for (auto point : widget->area) {
    full_map_at(point) = widget;
  }
  widget->set_updated();
}

static
void
tui_clear_widget(
  tui_widget *widget
) {
  for (auto point : widget->area) {
    for (auto it = wl_head; it; it = it->next) {
      if (it->body == widget) continue;
      if (point.is_in(it->body->area)) {
        full_map_at(point) = it->body;
        it->body->set_updated();
        break;
      }
    }
  }
}

void
tui_reg_widget(
  tui_widget *widget
) {
  widget->instaniated = true;
  tui_paste_widget(widget);
  tui_append_widget(widget);
  focus = widget;
}

void
tui_erase_widget(
  tui_widget *widget
) {
  if (focus == widget) {
    focus = widget->parent;
    if (!focus) {
      Warn("Erasing root widget");
    }
  }
  tui_delete_widget(widget);
  tui_clear_widget(widget);
  return ;
}

void
tui_draw() {
  draw_counter++;

  auto it = wl_head;
  for (auto it = wl_head; it; it = it->next) {
    if (it->body->get_updated()) break;
  }

  // no need to draw
  if (!it) {
    Debug("Skip drawing!");
    return ;
  }

  Debug("Trigger drawing!");
  ansi_cursor_set(1, 1);
  fflush(stdout);
  for (auto point : global_rect) {
    // Debug("%p %p", point.rect, &global_rect);
    Debug("(%d, %d) ", point.x, point.y);
    if (!full_map_at(point)) {
      putchar(' ');
      continue;
    }
    if (full_map_at(point)->get_updated()) {
      full_map_at(point)->draw(
        full_map_at(point)->position_interpreter(point)
      );
    }
    else ansi_cursor_fw(1);
    if (point.y == global_rect.tail.y)
      putchar('\n');
  }
  fflush(stdout);
  for (auto it = wl_head; it; it = it->next)
    it->body->reset_updated();
}

void
tui_reset_widget(
  tui_widget *widget,
  tui_rect area
) {
  tui_erase_widget(widget);
  widget->area = area;
  tui_reg_widget(widget);
  return ;
}

void 
tui_focus_on(
  tui_point point
) {
  for (auto it = wl_head; it; it = it->next) {
    Debug("Focus: checking %p", it->body);
    it->body->area.log_rect();
    if (point.is_in(it->body->area)) {
      // change focus if it is a click
      if (focus != it->body) {
        Debug("Change focus to %p", it->body);
      } else {
        Debug("Focus remains %p", it->body);
      }
      // root widget can only be focused upon its children's exit
      if (it->body->parent)
        tui_reset_widget(it->body, it->body->area);
      break;
    }
  }
}

void
tui_adjust_widget(
  tui_widget *widget,
  tui_rect area
) {
  tui_clear_widget(focus);
  widget->area = area;
  tui_paste_widget(widget);
  return ;
}