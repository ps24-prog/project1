#ifndef WIDGET_H__
#define WIDGET_H__


#include "geometry.h"
#include "event.h"
#include "ui.h"
#include <debug.h>

struct tui_widget {
  tui_rect area;
  bool updated;
  bool instaniated;
  tui_widget *parent;
  int retcode;
  int livecnt;
  bool deleted;
  std::vector<tui_widget *> children;
  
  static void delete_widget(tui_widget *widget, bool hard_delete);

  tui_widget(tui_rect area=global_rect, tui_widget *parent=NULL);
  tui_point position_interpreter(tui_point point) const;
  tui_point position_mapper(tui_point local_point) const;
  void create_widget(tui_widget *widget);
  void add_widget(tui_widget *widget);
  void remove_widget(tui_widget *widget);

  void set_updated() {updated = true;};
  void reset_updated() {updated = false;};
  bool get_updated() const {return updated;}

  virtual tui_event *on_event(tui_event *event);
  virtual void draw(tui_point point) const;
  virtual tui_event *on_child_exit(tui_widget *child);
  virtual ~tui_widget();
};

extern tui_widget *root;
void tui_widget_init();

#endif