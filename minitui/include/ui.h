#ifndef UI_H__
#define UI_H__

struct tui_widget;

extern tui_widget *focus;

struct tui_widget_list {
  tui_widget *body;
  tui_widget_list *next;
};

extern tui_widget_list *wl_head;
extern long long draw_counter;

void tui_ui_init();
void tui_focus_on(tui_point point);
void tui_draw();
void tui_reg_widget(tui_widget *widget);
void tui_erase_widget(tui_widget *widget);
void tui_reset_widget(tui_widget *widget, tui_rect area);
void tui_adjust_widget(tui_widget *widget, tui_rect area, bool proxy_penetrate=true);
void tui_update_widget(tui_widget *old_widget, tui_widget *new_widget);

#endif