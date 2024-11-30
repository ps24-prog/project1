#include <minitui.h>

tui_widget *focus;
tui_widget_list *wl_head;

static
void
tui_append_widget (
  tui_widget *widget
) {
  auto widget_list = CREATE_OBJ(tui_widget_list);
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

void tui_ui_init() {
  // get full_map
  full_map = (tui_widget ***) malloc(sizeof(tui_widget **) * scr_size.x);
  for (int i = 0; i < scr_size.x; i++) {
    full_map[i] = (tui_widget **) malloc(sizeof(tui_widget *) * scr_size.y);
  }
  return ;
}

void
tui_reg_widget(
  tui_widget *widget
) {
  for (int i = widget->area.start.x; i <= widget->area.end.x; i++) {
    for (int j = widget->area.start.y; j <= widget->area.end.y; j++) {
      full_map[i][j] = widget;
    }
  }
  tui_append_widget(widget);
  widget->set_updated();
  focus = widget;
}

void
tui_erase_widget(
  tui_widget *widget
) {
  tui_delete_widget(widget);
  if (focus == widget) {
    focus = widget->parent;
    if (!focus) {
      Info("Erasing top focus");
    }
  }
  for (int i = widget->area.start.x; i <= widget->area.end.x; i++) {
    for (int j = widget->area.start.y; j <= widget->area.end.y; j++) {
      full_map[i][j] = NULL;
      auto point = (tui_point) {i, j};
      for (auto it = wl_head; it; it = it->next) {
        if (point.is_in(it->body->area)) {
          full_map[i][j] = it->body;
          it->body->set_updated();
          break;
        }
      }
    }
  }
  return ;
}

void
tui_draw() {
  Debug("Trigger drawing!");
  ansi_cursor_set(1, 1);
  fflush(stdout);
  for (int i = 0; i < SCR_HEIGHT; i++) {
    for (int j = 0; j < SCR_WIDTH; j++) {
      if (!full_map[i][j]) {
        putchar(' ');
        continue;
      }
      if (full_map[i][j]->get_updated()) {
        full_map[i][j]->draw(
          full_map[i][j]->position_interpreter((tui_point) {i, j})
        );
      }
      else ansi_cursor_fw(1);
    }
    putchar('\n');
  }
  fflush(stdout);
  for (auto it = wl_head; it; it = it->next)
    it->body->reset_updated();
}