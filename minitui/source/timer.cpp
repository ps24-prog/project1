#include <minitui.h>

static std::vector<tui_timer *> timers;
 
void
tui_timer::handle() {
  for (auto it = timers.begin(); it != timers.end(); it = release(it)) ;
}

void
tui_timer::append(tui_timer *timer) {
  timers.push_back(timer);
}

bool
tui_timer::check() {
  auto now = std::chrono::steady_clock::now();
  auto duration = std::chrono::duration_cast<std::chrono::milliseconds>(now - start);
  return duration.count() >= interval;
}

void
tui_timer::execute() {
  callback(widget);
}

std::vector<tui_timer *>::iterator
tui_timer::release(std::vector<tui_timer *>::iterator it) {
  tui_timer *timer = *it;
  if (!timer->widget->deleted && !timer->check()) {
    return it++;
  }
  if (!timer->widget->deleted) {
    timer->execute();
  }
  if (timer->periodic && !timer->widget->deleted) {
    timer->start = std::chrono::steady_clock::now();
    return it++;
  } else {
    delete timer;
    return timers.erase(it);
  }
}

tui_timer::~tui_timer() {
  assert(widget->deleted || widget->livecnt > 1);
  tui_widget::delete_widget(widget, false);
}




