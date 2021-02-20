#ifndef WINDOW_H
#define WINDOW_H

#include <string>

class Window {
public:
  struct Properties {
    std::string title;
    uint32_t width;
    uint32_t height;

    Properties(const std::string& title, uint32_t width, uint32_t height) : title(title), width(width), height(height) {}
  };

  virtual ~Window() {};
  virtual void on_update() = 0;
  virtual uint32_t get_width() const = 0;
  virtual uint32_t get_height() const = 0;
  virtual float get_fps() const = 0;
  virtual void set_fps(const float) = 0;

  // Todo: set_event_callback
  // Todo: set vsync
  // Todo: is vsync

  virtual void* get_native_window() const = 0;

  static Window* create(const Window::Properties& props);
private:
};

#endif // WINDOW_H
