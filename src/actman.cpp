#include <stdlib.h>
#include <iostream>
#include <sstream>
#include <string>

#include "log.h"
#include "actman.h"
#include "error.h"
#include "entity.h"
#include "components/transform.h"
#include "components/cursessprite.h"
#include "physics/tilephysics.h"

void ActMan::parse(std::vector<std::string> argv) {

  uint32_t pos = 0;

  for (auto opt : argv) {
    if(opt[0] == '-') { // option
      if(opt[1] == '-') { // long option
      } else { // short option
        switch(opt[1]) {
          case 'h': // help
          case '?':
            printf("%s\n", this->print_help());
            this->running = false;
            return;
          case 'v': // verbose
            this->core_level = spdlog::level::trace;
            this->client_level = spdlog::level::trace;
            break;
          default:
            throw NoOptError(&opt[1]);
        }
      }
    } else { // positional argument
      switch(pos) {
        case 0: // config file
          this->config_file = std::filesystem::path(opt);
          break;
        case 1:
          this->output_file = std::filesystem::path(opt);
          break;
        default:
          throw UnexpPosOptError(opt.c_str());
      }
      pos++;
    }
  }
  Log::init("ActMan", this->core_level, this->client_level);
}

const char* ActMan::print_help() const {
  std::stringstream ss;
  ss << ActMan::usage() << "\n";
  ss << "\t" << "config_file - path to board configuration file" << "\n";
  ss << "\t" << "output_file - path to output file of simulation" << "\n";
  ss << "\n" << "OPTIONS" << "\n\n";
  ss << "\t" << "-v" << "\t\t" << "Enable verbose output" << "\n";
  return ss.str().c_str();
}

const char* ActMan::usage() const {
  std::stringstream ss;
  ss << "USAGE: actman [OPTIONS] config_file output_file" << "\n";
  return ss.str().c_str();
}

void ActMan::configure() {
  this->config.load_config(this->config_file);
  this->window = Window::create({"ActMan", this->config.cols + 2, this->config.rows + 2});
  this->physics = (Physics*)(new TilePhysics());
  this->renderer = (Renderer*)(new CursesRenderer());
}

void ActMan::reset() {
  uint32_t y = 0, x = 0;
  for(auto line : this->config.init_state) {
    for(auto cell : line) {
      Entity entity = this->scene.create_entity();
      switch(cell) {
        case '#': // Wall
          entity.add_component<Transform>(x, y, 0);
          entity.add_component<CursesSprite>("#", true);
          // entity.add_component<BoardPhysics>(0, true);
          break;
        case '.': // Gold Nugget
          entity.add_component<Transform>(x, y, 0);
          entity.add_component<CursesSprite>(".", true);
          // entity.add_component<BoardPhysics>(0, false);
          break;
        case '$': // Gold Bar
          entity.add_component<Transform>(x, y, 0);
          entity.add_component<CursesSprite>("$", true);
          // entity.add_component<BoardPhysics>(0, false);
          break;
        case 'A': // ActMan
          entity.add_component<Transform>(x, y, 0);
          entity.add_component<CursesSprite>("A", true);
          // entity.add_component<BoardPhysics>(1, false);
          break;
        case 'P': // Punky
          entity.add_component<Transform>(x, y, this->config.dirs[(uint32_t)Ghost::PUNKY]);
          entity.add_component<CursesSprite>("P", true);
          // entity.add_component<BoardPhysics>(1, false);
          break;
        case 'B': // Bunky
          entity.add_component<Transform>(x, y, this->config.dirs[(uint32_t)Ghost::BUNKY]);
          entity.add_component<CursesSprite>("B", true);
          // entity.add_component<BoardPhysics>(1, false);
          break;
        case 'D': // Dunky
          entity.add_component<Transform>(x, y, this->config.dirs[(uint32_t)Ghost::DUNKY]);
          entity.add_component<CursesSprite>("D", true);
          // entity.add_component<BoardPhysics>(1, false);
          break;
        case 'R': // Runky
          entity.add_component<Transform>(x, y, this->config.dirs[(uint32_t)Ghost::RUNKY]);
          entity.add_component<CursesSprite>("R", true);
          // entity.add_component<BoardPhysics>(1, false);
          break;
      }
      x++;

    }
    x = 0;
    y++;
  }
}
