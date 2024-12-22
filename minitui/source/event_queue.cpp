#include <event.h>

tui_event_queue::~tui_event_queue() {
  while (!queue.empty()) {
    tui_event *event = queue.front();
    queue.pop();
    delete event;
  }
}

void 
tui_event_queue::push_event(tui_event *event) {
  std::lock_guard<std::mutex> lock(mutex);
  queue.push(event);
  Trace("push event %p", event);
  tui_event::log_event(event, true);
  cv.notify_one();
}

tui_event *
tui_event_queue::pop_event() {
  std::unique_lock<std::mutex> lock(mutex);
  cv.wait(lock, [this] { return !queue.empty(); });
  tui_event *event = queue.front();
  queue.pop();
  Trace("pop event %p", event);
  // tui_event::log_event(event, true);
  return event;
}

bool 
tui_event_queue::empty() {
  std::lock_guard<std::mutex> lock(mutex);
  return queue.empty();
}

void 
tui_event_queue::clear() {
  std::lock_guard<std::mutex> lock(mutex);
  while (!queue.empty()) {
    tui_event *event = queue.front();
    queue.pop();
    delete event;
  }
}

tui_event_queue gbl_event_queue;