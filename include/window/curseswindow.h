#ifndef CURSES_WINDOW_H
#define CURSES_WINDOW_H


#include <ncurses.h>

#include "window/window.h"

using RawCursesWindow = WINDOW;

// Todo: error callback

class CursesWindow : public Window {
public:
  CursesWindow(const Window::Properties& props);
  virtual ~CursesWindow();

  void on_update() override;

  inline uint32_t get_width() const override {
    return getmaxx(this->window);
  }

  inline uint32_t get_height() const override {
    return getmaxy(this->window);
  }

  // Todo: set_event_callback
  // Todo: set vsync
  // Todo: is vsync

  inline void* get_native_window() const override {
    return this->window;
  }

  inline float get_fps() const override {
    return this->fps;
  }

  inline void set_fps(const float fps) override {
    this->fps = fps;
  }

protected:
  static uint32_t n_windows;
  static bool curses_initialized;
private:
  void shutdown();
  float fps = 60;

  RawCursesWindow* window;
};


#endif // CURSES_WINDOW_H
