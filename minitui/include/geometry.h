#ifndef GEOMETRY_H__
#define GEOMETRY_H__

#include <debug.h>

#ifndef SCR_WIDTH
#define SCR_WIDTH 50
#endif
#ifndef SCR_HEIGHT
#define SCR_HEIGHT 20
#endif

struct tui_rect;

struct tui_point {
  int x, y;
  const tui_rect *rect;
  tui_point(int x, int y, const tui_rect *rect=NULL);
  tui_point(const tui_point &p, const tui_rect *rect);
  bool is_valid() const;
  bool is_in(const tui_rect &rect) const;
  bool operator == (const tui_point &b) const;
  bool operator != (const tui_point &b) const;
  tui_point operator - (const tui_point &b) const;
  tui_point operator + (const tui_point &b) const;
  tui_point& operator ++ ();
};

extern tui_point scr_size;
extern tui_rect global_rect;

struct tui_rect {
  tui_point head, tail;
  tui_rect(tui_point head, tui_point tail) : head(head), tail(tail) {
    this->head.rect = this;
    this->tail.rect = this;
  }
  tui_rect& operator = (const tui_rect &b) {
    head = b.head; head.rect = this;
    tail = b.tail; tail.rect = this;
    return *this;
  }
  tui_rect(const tui_rect &_rect) : head(_rect.head), tail(_rect.tail) {
    head.rect = this;
    tail.rect = this;
  }
  bool operator == (const tui_rect &b) const;
  int height() const;
  int width() const;
  struct Iterator {
    tui_point current;
    Iterator(tui_point current) : current(current) {}
    tui_point operator * () const {
      return current;
    }
    Iterator &operator ++ () {
      ++current;
      return *this;
    }
    bool operator != (const Iterator &b) const {
      return current != b.current;
    }
  };
  Iterator begin() const {
    return Iterator(head);
  }
  Iterator end() const {
    return Iterator(tui_point(-1, -1));
  }
  void log_rect() const {
    Debug("Rect: (%d, %d) -> (%d, %d)", head.x, head.y, tail.x, tail.y);
  }
};





#endif