#include<chrono>
#include<iostream>

#include<ncurses.h>

#include "log.h"
#include "window/curseswindow.h"
#include "input/cursesinput.h"

// Todo: error callback

Window* Window::create(const Window::Properties& props) {
  return new CursesWindow(props);
}

uint32_t CursesWindow::n_windows = 0;
bool CursesWindow::curses_initialized = false;

CursesWindow::CursesWindow(const Window::Properties& props) {
  Log::get_core_logger()->trace("Creating Window with size ({}, {})", props.width, props.height);
  if (!CursesWindow::curses_initialized) {
    initscr();
    Input::_set_instance(new CursesInput());
    curs_set(0);
    CursesWindow::curses_initialized = true;
  }

  int32_t y = (LINES - props.height) / 2;
  int32_t x = (COLS - props.width) / 2;

  CursesWindow::n_windows++;

  this->window = newwin(props.height, props.width, y, x);
  keypad(this->window, true);
  box(this->window, 0, 0);
  wrefresh(this->window);
}

CursesWindow::~CursesWindow() {
  Log::get_core_logger()->trace("Destroying window");
  wborder(this->window, ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ');
  wrefresh(this->window);
  delwin(this->window);
  CursesWindow::n_windows--;
  if(CursesWindow::n_windows <= 0) {
    curs_set(1);
    endwin();
  }
}

void CursesWindow::on_update() {
  Log::get_core_logger()->trace("Update Window");
  // poll events
  static uint64_t last_update = std::chrono::duration_cast<std::chrono::milliseconds>(std::chrono::system_clock::now().time_since_epoch()).count();
  box(this->window, 0, 0);
  // sync to 60 Hz
  uint64_t now;
  do {
    now = std::chrono::duration_cast<std::chrono::milliseconds>(std::chrono::system_clock::now().time_since_epoch()).count();
  } while( this->fps < 1000 / float(now - last_update));
  // getch();
  wrefresh(this->window);
  last_update = now;
}
