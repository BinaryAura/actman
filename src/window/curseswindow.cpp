#include<chrono>
#include<ncurses.h>

#include "window/curseswindow.h"
#include "input/cursesinput.h"

// Todo: error callback

Window* Window::create(const Window::Properties& props) {
  return new CursesWindow(props);
}

CursesWindow::CursesWindow(const Window::Properties& props) {
  if (!curses_initialized) {
    initscr();
    Input::_set_instance(new CursesInput());
    keypad(this->window, true);
    curs_set(0);
    curses_initialized = true;
  }

  int32_t y = (LINES - props.height) / 2;
  int32_t x = (COLS - props.width) / 2;

  this->window = newwin(props.height, props.width, y, x);
  box(this->window, 0, 0);
  wrefresh(this->window);
}

CursesWindow::~CursesWindow() {
  wborder(this->window, ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ');
  wrefresh(this->window);
  delwin(this->window);
}

void CursesWindow::on_update() {
  // poll events
  static auto last_update = 0;
  box(this->window, 0, 0);
  // sync to 60 Hz
  uint64_t now;
  do {
    now = std::chrono::duration_cast<std::chrono::milliseconds>(std::chrono::system_clock::now().time_since_epoch()).count();
  } while( this->fps > 1000 / float(now - last_update));
  wrefresh(this->window);
  wclear(this->window);
  last_update = now;
}
