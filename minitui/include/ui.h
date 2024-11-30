#ifndef UI_H__
#define UI_H__

extern tui_widget *focus;

struct tui_widget_list {
  tui_widget *body;
  tui_widget_list *next;
};

extern tui_widget_list *wl_head, *wl_tail;

void tui_ui_init();
void tui_draw();
void tui_reg_widget(tui_widget *widget);
void tui_erase_widget(tui_widget *widget);

#endif