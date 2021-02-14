#ifndef CURSES_INPUT_H
#define CURSES_INPUT_H

#include <ncurses.h>
#include "input/input.h"

class CursesInput : public Input {
public:
  CursesInput();
protected:
  virtual bool _is_key_pressed(int32_t keycode) override;
  // Todo: raw mouse button pressed
  // Todo: raw mouse x
  // Todo: raw mouse y
  // Todo: raw mouse position
};

#endif // CURSES_INPUT_H
