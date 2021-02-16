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
#include "components/memory.h"
#include "components/dunkymemory.h"
#include "components/runkymemory.h"
#include "components/score.h"
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

void ActMan::close() {

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
  e.add_component<Score>(0);
  for(auto line : this->config.init_state) {
    for(auto cell : line) {
      Entity entity = this->scene.create_entity();
      float dir;
      auto &patrol = this->config.patrol;
      auto &pool = this->config.dir_pool;
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
          // entity.add_component<CursesSprite>('.', true);
          break;
        case '$': // Gold Bar
          entity.add_component<Tag>("Gold Bar");
          entity.add_component<Transform>(x, y, 0, 0);
          // entity.add_component<CursesSprite>('$', true);
          break;
        case '*': // Diamond
          entity.add_component<Tag>("Diamond");
          entity.add_component<Transform>(x, y, 0, 0);
          // entity.add_component<CursesSprite>('*', true);
        case 'A': // ActMan
          entity.add_component<Tag>("ActMan");
          entity.add_component<Transform>(x, y, 10, 0);
          entity.add_component<CursesSprite>('A', true);
          entity.add_component<TileMove>();
          entity.add_component<TileCollider>(true, false);
          entity.add_component<AI>([](Scene& scene, Entity entity) {
            static std::default_random_engine rng;
            auto &tag = entity.get_component<Tag>();
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
            auto& transform = entity.get_component<Transform>();
            auto& move = entity.get_component<TileMove>();
            bool found = false;
            uint32_t dir;
            do {
              auto target = transform;
              dir = ((int32_t)((90 * ((rng() - 1) % 4)) + 45) % 360) / 90;
              target.x -= (dir % 2)*(dir - 2);
              target.y += (1 - (dir % 2))*(dir - 1);
              for(auto m : actions) {
                if(m.x == target.x && m.y == target.y)
                  found = true;
              }
            } while(!found);
            auto new_tile = transform;
            new_tile.x = transform.x + round(sin(transform.rotation * M_PI / 180));
            new_tile.y = transform.y + round(cos(transform.rotation * M_PI / 180));
            move.x = new_tile.x;
            move.y = new_tile.y;
            transform.rotation = dir*90;
            auto &score = scene.get_entity("Board").get_component<Score>();
            for(auto entity : tilemap.get_entities(scene, new_tile)) {
              auto &tag = entity.get_component<Tag>();
              if(tag.name == "Gold Nugget") {
                score.value += 1;
                entity.kill();
              } else if(tag.name == "Gold Bar") {
                score.value += 5;
                entity.kill();
              } else if(tag.name == "Diamond") {
                score.value += 10;
                entity.kill();
              }
            }
          });
          break;
        case 'P': // Punky
          dir = this->config.dirs[(uint32_t)Ghost::PUNKY];
          entity.add_component<Tag>("Punky");
          entity.add_component<Transform>(x, y, 1, dir);
          entity.add_component<CursesSprite>('P', true);
          entity.add_component<TileCollider>(true, false);
          entity.add_component<TileMove>();
          entity.add_component<AI>([](Scene& scene, Entity entity){
            auto actman = scene.get_entity("ActMan");
            auto &current = entity.get_component<Transform>();
            auto &at = entity.get_component<Transform>();
            if(at.x == current.x && at.y == current.y) {
              auto grave = scene.create_entity();
              grave.add_component<Tag>("Grave");
              grave.add_component<Transform>(current);
              grave.add_component<CursesSprite>('X', true);
              actman.kill();
            }
            auto &tag = entity.get_component<Tag>();
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
            auto dir = ((int32_t)(current.rotation + 45) % 360) / 90;
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
                  if(m.x == current.x - (dir % 2)*(dir - 2) && m.y == current.y + (1 - (dir % 2))*(dir - 1)) {
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
            uint32_t dx = choice->x - current.x;
            uint32_t dy = choice->y - current.y;
            if (dx == 0) {
              if (dy == -1) {
                current.rotation = 0;
              } else {
                current.rotation = 180;
              }
            } else if (dx == 1) {
              current.rotation = 90;
            } else {
              current.rotation = 270;
            }
            if(at.x == current.x && at.y == current.y) {
              auto grave = scene.create_entity();
              grave.add_component<Tag>("Grave");
              grave.add_component<Transform>(current);
              grave.add_component<CursesSprite>('X', true);
              actman.kill();
            }
          });
          break;
        case 'B': // Bunky
          dir = this->config.dirs[(uint32_t)Ghost::BUNKY];
          entity.add_component<Tag>("Bunky");
          entity.add_component<Transform>(x, y, 2, dir);
          entity.add_component<CursesSprite>('B', true);
          entity.add_component<TileCollider>(true, false);
          entity.add_component<TileMove>();
          entity.add_component<AI>([](Scene& scene, Entity entity){
            auto &tag = entity.get_component<Tag>();
            auto actman = scene.get_entity("ActMan");
            auto &current = entity.get_component<Transform>();
            auto &at = actman.get_component<Transform>();
            if(at.x == current.x && at.y == current.y) {
              auto grave = scene.create_entity();
              grave.add_component<Tag>("Grave");
              grave.add_component<Transform>(current);
              grave.add_component<CursesSprite>('X', true);
              actman.kill();
            }
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
            auto dir = ((int32_t)(current.rotation + 45) % 360) / 90;
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
                  if(m.x == current.x - (dir % 2)*(dir - 2) && m.y == current.y + (1 - (dir % 2))*(dir - 1)) {
                    break;
                  }
                }
                break;
              default:
                auto &target = scene.get_entity("ActMan").get_component<Transform>();
                auto tdir = ((int32_t)(target.rotation + 45) % 360) / 90;
                auto intercept = Transform(target);
                intercept.x -= (dir & 2)*(dir - 2)*4;
                intercept.y += (1 - (dir % 2))*(dir - 1)*4;
                uint32_t dist = -1;
                for(auto m : actions) {
                  uint32_t d = tilemap.distance(intercept, m);
                  if(d < dist || dist == -1) {
                    choice = &m;
                    dist = d;
                  }
                }
                break;
            }
            move.x = choice->x;
            move.y = choice->y;
            uint32_t dx = choice->x - current.x;
            uint32_t dy = choice->y - current.y;
            if (dx == 0) {
              if (dy == -1) {
                current.rotation = 0;
              } else {
                current.rotation = 180;
              }
            } else if (dx == 1) {
              current.rotation = 90;
            } else {
              current.rotation = 270;
            }
            if(at.x == current.x && at.y == current.y) {
              auto grave = scene.create_entity();
              grave.add_component<Tag>("Grave");
              grave.add_component<Transform>(current);
              grave.add_component<CursesSprite>('X', true);
              actman.kill();
            }
          });
          break;
        case 'D': // Dunky
          dir = this->config.dirs[(uint32_t)Ghost::DUNKY];
          entity.add_component<Tag>("Dunky");
          entity.add_component<Transform>(x, y, 3, dir);
          entity.add_component<CursesSprite>('D', true);
          entity.add_component<TileCollider>(true, false);
          entity.add_component<TileMove>();
          entity.add_component<DunkyMemory>(this->config.patrol);
          entity.add_component<AI>([](Scene& scene, Entity entity){
              auto &tag = entity.get_component<Tag>();
              auto actman = scene.get_entity("ActMan");
              auto &current = entity.get_component<Transform>();
              auto &at = actman.get_component<Transform>();
              if(at.x == current.x && at.y == current.y) {
                auto grave = scene.create_entity();
                grave.add_component<Tag>("Grave");
                grave.add_component<Transform>(current);
                grave.add_component<CursesSprite>('X', true);
                actman.kill();
              }
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
              if(at.x == current.x && at.y == current.y) {
                auto grave = scene.create_entity();
                grave.add_component<Tag>("Grave");
                grave.add_component<Transform>(current);
                grave.add_component<CursesSprite>('X', true);
                actman.kill();
              }
              auto dir = ((int32_t)(current.rotation + 45) % 360) / 90;
              Transform *choice = nullptr;
              auto &patrol = entity.get_component<DunkyMemory>().data;
              static uint32_t next = 0;
              if(current.x == patrol[next].x && current.y == patrol[next].y) {
                next = (next + 1) % patrol.size();
              }
              switch(actions.size()) {
                case 1:
                  choice = &actions[0];
                  break;
                case 2:
                  for(auto m : actions) {
                    if(m.x != current.x + (dir % 2)*(dir - 2) || m.y != current.y - (1 - (dir % 2))*(dir - 1)) {
                      choice = &m;
                    }
                    if(m.x == current.x - (dir % 2)*(dir - 2) && m.y == current.y + (1 - (dir % 2))*(dir - 1)) {
                      break;
                    }
                  }
                  break;
                default:
                Transform target;
                target.x = patrol[next].x;
                target.y = patrol[next].y;
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
              uint32_t dx = choice->x - current.x;
              uint32_t dy = choice->y - current.y;
              if (dx == 0) {
                if (dy == -1) {
                  current.rotation = 0;
                } else {
                  current.rotation = 180;
                }
              } else if (dx == 1) {
                current.rotation = 90;
              } else {
                current.rotation = 270;
              }
              if(at.x == current.x && at.y == current.y) {
                auto grave = scene.create_entity();
                grave.add_component<Tag>("Grave");
                grave.add_component<Transform>(current);
                grave.add_component<CursesSprite>('X', true);
                actman.kill();
              }
          });
          break;
        case 'R': // Runky
          entity.add_component<Tag>("Runky");
          entity.add_component<Transform>(x, y, 4, dir);
          entity.add_component<CursesSprite>('R', true);
          entity.add_component<TileCollider>(true, false);
          entity.add_component<TileMove>();
          entity.add_component<RunkyMemory>(this->config.dir_pool);
          entity.add_component<AI>([](Scene& scene, Entity entity){
            auto &tag = entity.get_component<Tag>();
            auto actman = scene.get_entity("ActMan");
            auto &tilemap = scene.get_entity("Board").get_component<TileMap>();
            auto free = tilemap.get_neighbors(entity.get_component<Transform>());
            auto &current = entity.get_component<Transform>();
            auto &at = actman.get_component<Transform>();
            if(at.x == current.x && at.y == current.y) {
              auto grave = scene.create_entity();
              grave.add_component<Tag>("Grave");
              grave.add_component<Transform>(current);
              grave.add_component<CursesSprite>('X', true);
              actman.kill();
            }
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
            auto dir = ((int32_t)(current.rotation + 45) % 360) / 90;
            auto &pool = entity.get_component<RunkyMemory>().data;
            Transform *choice = nullptr;
            static uint32_t next = 0;
            switch(actions.size()) {
              case 1:
                choice = &actions[0];
                break;
              case 2:
                for(auto m : actions) {
                  if(m.x != current.x + (dir % 2)*(dir - 2) || m.y != current.y - (1 - (dir % 2))*(dir - 1)) {
                    choice = &m;
                  }
                  if(m.x == current.x - (dir % 2)*(dir - 2) && m.y == current.y + (1 - (dir % 2))*(dir - 1)) {
                    break;
                  }
                }
                break;
              default:
                auto target = current;
                bool found = false;
                do {
                  dir = ((int32_t)(pool[next] + 45) % 360) / 90;
                  next = (next + 1) % pool.size();
                  target.x -= (dir & 2)*(dir - 2);
                  target.y += (1 - (dir % 2))*(dir - 1);
                  for(auto m : actions) {
                    if(m.x == target.x && m.y == target.y)
                      found = true;
                  }
                } while(!found);
                break;
            }
            move.x = choice->x;
            move.y = choice->y;
            current.rotation = dir*90;
            if(at.x == current.x && at.y == current.y) {
              auto grave = scene.create_entity();
              grave.add_component<Tag>("Grave");
              grave.add_component<Transform>(current);
              grave.add_component<CursesSprite>('X', true);
              actman.kill();
            }
          });
          break;
      }
      x++;
    }
    x = 1;
    y++;
  }
}
