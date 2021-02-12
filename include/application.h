#ifndef APPLICATION_H
#define APPLICATION_H

#include <inttypes.h>
#include <vector>
#include <filesystem>
#include <string>
#include <fstream>

#include <spdlog/spdlog.h>

#include "error.h"
#include "boardconfig.h"
// #include "scene.h"

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
      process_events();
      update();
      render();
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
      fprintf(stderr, "actman %s", this->usage());
      exit(err.id);
    } catch (const FileError& err) {
      fprintf(stderr, "%s: %s\n", err.what(), err.file.c_str());
      exit(err.id);
    } catch (const std::ifstream::failure& err) {
      fprintf(stderr, "%s", err.what());
      exit(err.code().value());
    }
  }
  virtual void reset() = 0;
  virtual const char* usage() const { return ""; }

protected:
  virtual const char* print_help() const { return ""; }
  void update();
  void render();
  void process_events();

  // Scene scene;
  bool running;
  // Scene scene;
  BoardConfig config;
  spdlog::level::level_enum core_level = CORE_DEF_LVL;
  spdlog::level::level_enum client_level = CLIENT_DEF_LVL;
};

#endif // APPLICATION_H
