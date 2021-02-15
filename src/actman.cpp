#include <stdlib.h>
#include <iostream>
#include <sstream>
#include <string>
#include <math.h>
#include <random>

#include "log.h"
#include "actman.h"
#include "error.h"
#include "entity.h"
#include "components/transform.h"
#include "components/cursessprite.h"
#include "components/tag.h"
#include "components/tilemove.h"
#include "components/ai.h"
#include "components/tilecollider.h"
#include "components/tilemap.h"
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
            printf("%s\n", this->print_help().c_str());
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

const std::string ActMan::print_help() const {
  std::stringstream ss;
  ss << ActMan::usage() << "\n";
  ss << "\t" << "config_file - path to board configuration file" << "\n";
  ss << "\t" << "output_file - path to output file of simulation" << "\n";
  ss << "\n" << "OPTIONS" << "\n\n";
  ss << "\t" << "-v" << "\t\t" << "Enable verbose output" << "\n";
  return ss.str();
}

const std::string ActMan::usage() const {
  std::stringstream ss;
  ss << "USAGE: actman [OPTIONS] config_file output_file" << "\n";
  return ss.str();
}

void ActMan::configure() {
  this->config.load_config(this->config_file);
  this->window = Window::create({"ActMan", this->config.cols + 2, this->config.rows + 2});
  this->window->set_fps(4);
}

void ActMan::reset() {
  uint32_t y = 1, x = 1;
  Entity e = this->scene.create_entity();
  e.add_component<Tag>("Board");
  e.add_component<Transform>(0, 0, 0, 0);
  e.add_component<TileMap>(this->config.rows, this->config.cols);
  for(auto line : this->config.init_state) {
    for(auto cell : line) {
      Entity entity = this->scene.create_entity();
      float dir;
      switch(cell) {
        case '#': // Wall
          entity.add_component<Tag>("Wall");
          entity.add_component<Transform>(x, y, 0, 0);
          entity.add_component<CursesSprite>('#', true);
          entity.add_component<TileCollider>(true, true);
          break;
        case '.': // Gold Nugget
          entity.add_component<Tag>("Gold Nugget");
          entity.add_component<Transform>(x, y, 0, 0);
          entity.add_component<CursesSprite>('.', true);
          break;
        case '$': // Gold Bar
          entity.add_component<Tag>("Gold Bar");
          entity.add_component<Transform>(x, y, 0, 0);
          entity.add_component<CursesSprite>('$', true);
          break;
        case 'A': // ActMan
          entity.add_component<Tag>("ActMan");
          entity.add_component<Transform>(x, y, 10, 0);
          entity.add_component<CursesSprite>('A', true);
          entity.add_component<TileMove>();
          entity.add_component<TileCollider>(true, true);
          entity.add_component<AI>([](Scene& scene, Entity entity) {
            static std::default_random_engine rng;
            auto& transform = entity.get_component<Transform>();
            auto& move = entity.get_component<TileMove>();
            transform.rotation = 90 * ((rng() - 1) % 4);
            move.x = transform.x + round(sin(transform.rotation * M_PI / 180));
            move.y = transform.y + round(cos(transform.rotation * M_PI / 180));
          });
          break;
        case 'P': // Punky
          dir = this->config.dirs[(uint32_t)Ghost::PUNKY];
          entity.add_component<Tag>("Punky");
          entity.add_component<Transform>(x, y, 1, dir);
          entity.add_component<CursesSprite>('P', true);
          entity.add_component<TileCollider>(true, true);
          entity.add_component<TileMove>();
          entity.add_component<AI>([](Scene& scene, Entity entity){
            auto &tilemap = scene.get_entity("Board").get_component<TileMap>();
            auto free = tilemap.get_neighbors(entity.get_component<Transform>());
            std::vector<Transform> actions;
            for(auto neighbor : free) {
              bool accessable = true;
              for(auto entity : tilemap.get_entities(scene, neighbor)) {
                if(entity.get_component<Tag>().name == "Wall") {
                  accessable = false;
                }
              }
              if(accessable) {
                actions.push_back(neighbor);
              }
            }
            auto &move = entity.get_component<TileMove>();
            auto &current = entity.get_component<Transform>();
            auto dir = ((int32_t)(current.rotation + 45) % 360) / 4;
            Transform *choice = nullptr;
            switch(actions.size()) {
              case 1:
                choice = &actions[0];
                break;
              case 2:
                for(auto m : actions) {
                  if(m.x != current.x + (dir % 2)*(dir - 2) || m.y != current.y - (1 - (dir % 2))*(dir - 1)) {
                    choice = &m;
                  }
                  if(m.x != current.x - (dir % 2)*(dir - 2) || m.y != current.y + (1 - (dir % 2))*(dir - 1)) {
                    break;
                  }
                }
                break;
              default:
                auto &target = scene.get_entity("ActMan").get_component<Transform>();
                uint32_t dist = -1;
                for(auto m : actions) {
                  uint32_t d = tilemap.distance(target, m);
                  if(d < dist || dist == -1) {
                    choice = &m;
                    dist = d;
                  }
                }
                break;
            }
            move.x = choice->x;
            move.y = choice->y;
          });
          break;
        case 'B': // Bunky
          dir = this->config.dirs[(uint32_t)Ghost::BUNKY];
          entity.add_component<Tag>("Bunky");
          entity.add_component<Transform>(x, y, 2, dir);
          entity.add_component<CursesSprite>('B', true);
          entity.add_component<TileCollider>(true, true);
          // entity.add_component<TileMove>();
          break;
        case 'D': // Dunky
          dir = this->config.dirs[(uint32_t)Ghost::DUNKY];
          entity.add_component<Tag>("Dunky");
          entity.add_component<Transform>(x, y, 3, dir);
          entity.add_component<CursesSprite>('D', true);
          entity.add_component<TileCollider>(true, true);
          // entity.add_component<TileMove>();
          break;
        case 'R': // Runky
          dir = this->config.dirs[(uint32_t)Ghost::RUNKY];
          entity.add_component<Transform>(x, y, 4, dir);
          entity.add_component<CursesSprite>('R', true);
          entity.add_component<TileCollider>(true, true);
          // entity.add_component<TileMove>();
          break;
      }
      x++;
    }
    x = 1;
    y++;
  }
}
