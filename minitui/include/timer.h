#ifndef TIMER_H__
#define TIMER_H__

#include <event.h>
#include <widget.h> 

struct tui_timer {
  std::chrono::time_point<std::chrono::steady_clock> start;
  tui_widget *widget;
  int interval; // by milliseconds
  bool periodic;
  std::function<void(tui_widget *)> callback;
  tui_timer(
    tui_widget *widget,
    int interval,
    std::function<void(tui_widget *)> callback,
    bool periodic = false
  ) : widget(widget), interval(interval), periodic(periodic), callback(callback) {
    this->widget->livecnt++;
    start = std::chrono::steady_clock::now();
  }
  ~tui_timer();

  bool check();
  void execute();
  
  static void append(tui_timer *timer);
  static void handle();
  static std::vector<tui_timer *>::iterator release(std::vector<tui_timer *>::iterator it);
};

#endif