#ifndef APPLICATION_H
#define APPLICATION_H

#include <inttypes.h>
#include <vector>
#include <filesystem>
#include <string>
#include <fstream>
#include <unistd.h>

#include <spdlog/spdlog.h>

#include "error.h"
#include "scene.h"
#include "window/window.h"

#include "physics/physics.h"
#include "renderer/renderer.h"

#ifdef NCURSES
  #include "window/curseswindow.h"
#endif

#ifdef DEBUG
  #define CORE_DEF_LVL spdlog::level::info
  #define CLIENT_DEF_LVL spdlog::level::info
#else // RELEASE
  #define CORE_DEF_LVL spdlog::level::warn
  #define CLIENT_DEF_LVL spdlog::level::info
#endif // DEBUG

class Application {
public:
  int32_t run() {
    while(this->running) {
      // Todo: process_events();
      this->scene.on_input();
      this->scene.on_update();
      #ifndef NORENDER
        this->scene.on_render(this->window);
        this->window->on_update();
      #endif
    }
    return 0;
  }
  virtual void parse(std::vector<std::string> argv) = 0;
  virtual void configure() { }
  void setup() {
    this->setup(std::vector<std::string>());
  }
  void setup(std::vector<std::string> argv) {
    try {
      this->parse(argv);
      if(this->running) this->configure();
    } catch (const ArgError& err) {
      fprintf(stderr, "%s: '%s'\n", err.what(), err.arg);
      fprintf(stderr, "actman %s", this->usage().c_str());
      exit(err.id);
    } catch (const ConfigError& err) {
      fprintf(stderr, "%s: %s:%d:", err.what(), err.file.c_str(), err.line);
      if(err.column >= 0)
        fprintf(stderr, "%d:", err.column);
      fprintf(stderr, " %s\n", err.text);
      exit(err.id);
    } catch (const FileError& err) {
      fprintf(stderr, "%s: %s\n", err.what(), err.file.c_str());
      exit(err.id);
    } catch (const std::ifstream::failure& err) {
      fprintf(stderr, "%s\n", err.what());
      exit(err.code().value());
    }
    this->reset();
  }
  virtual void reset() = 0;
  virtual void close() = 0;
  virtual const std::string usage() const { return ""; }

protected:
  virtual const std::string print_help() const { return ""; }
  // void on_input();
  // void on_update();
  // void on_render();

  bool running = true;
  Scene scene;
  Window *window;
  spdlog::level::level_enum core_level = CORE_DEF_LVL;
  spdlog::level::level_enum client_level = CLIENT_DEF_LVL;
};

#endif // APPLICATION_H
