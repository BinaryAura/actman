#ifndef APPLICATION_H
#define APPLICATION_H

#include <inttypes.h>
#include <vector>
#include <filesystem>
#include <string>
#include <fstream>
#include <unistd.h>
#include <chrono>

#include <spdlog/spdlog.h>

#include "log.h"
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
  Application() = default;
  Application(std::string name) : name(name) {}

  ~Application() {
      delete this->window;
  }

  int32_t run() {
    uint32_t tick = 0;
    #ifndef NORENDER
      this->scene.on_render(this->window);
      this->window->on_update();
    #endif
    Log::get_core_logger()->info("Starting engine loop");
    Log::get_core_logger()->info("Running for {} ticks", this->ticks);
    while(this->running && (tick < this->ticks || this->ticks == -1)) {
      Log::get_core_logger()->trace("Start Tick {}", tick);
      // Todo: process_events();
      this->scene.on_input(this);
      this->scene.on_update();
      #ifndef NORENDER
        this->scene.on_render(this->window);
        Log::get_core_logger()->trace("Render Tick {}", tick);
        this->window->on_update();
      #endif
      tick++;
    }
    Log::get_core_logger()->trace("Closing Application");
    this->on_close();
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
      Log::init(this->name, this->core_level, this->client_level);
      Log::get_core_logger()->flush_on(this->core_level);
      Log::get_client_logger()->flush_on(this->client_level);
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
    Log::get_core_logger()->info("Engine Started");
    this->reset();
  }
  virtual void reset() = 0;
  virtual void on_close() = 0;
  void close() {
    this->running = false;
  }
  virtual const std::string usage() const { return ""; }
  virtual const std::string print_help() const { return ""; }
  const std::string& get_name() const { return this->name; }
  void set_name(const std::string& name) { this->name = name; }
  Scene &get_scene() {
    return this->scene;
  }

protected:
  // void on_input();
  // void on_update();
  // void on_render();
  int32_t ticks = -1;
  std::string name;
  bool running = true;
  Scene scene;
  Window *window;
  spdlog::level::level_enum core_level = CORE_DEF_LVL;
  spdlog::level::level_enum client_level = CLIENT_DEF_LVL;
};

#endif // APPLICATION_H
