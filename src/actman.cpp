// #include <stdlib.h>
// #include <iostream>
// #include <sstream>
// #include <string>
// #include <math.h>
#include <random>
// #include <ncurses.h>
// #include <algorithm>

#include "hadron/hadron.h"
#include "hadron/curses.h"
#include "hadron/physics/tilephysics.h"

#include "actman.h"
#include "components/score.h"
#include "components/movelog.h"
#include "components/dunkymemory.h"
#include "components/runkymemory.h"

Application* create(const int32_t argc, const char** argv) {
  return new ActMan(argc, argv);
}

void ActMan::parse(std::vector<std::string> argv) {

  uint32_t pos = 0;

  for (auto opt = argv.begin(); opt != argv.end(); opt++) {
    if((*opt)[0] == '-') { // option
      if((*opt)[1] == '-') { // long option
      } else { // short option
        switch((*opt)[1]) {
          case 'h': // help
          case '?':
            printf("%s\n", this->print_help().c_str());
            this->running = false;
            return;
          case 'v': // verbose
            this->core_level = spdlog::level::trace;
            this->client_level = spdlog::level::trace;
            break;
          case 't': // number of turns
            opt++;
            this->ticks = std::stoi(*opt);
            break;
          default:
            throw NoOptError(&(*opt)[1]);
        }
      }
    } else { // positional argument
      switch(pos) {
        case 0: // config file
          this->config_file = std::filesystem::path(*opt);
          break;
        case 1:
          this->output_file = std::filesystem::path(*opt);
          break;
        default:
          throw UnexpPosOptError((*opt).c_str());
      }
      pos++;
    }
  }
}

void ActMan::on_close() {
  Log::get_client_logger()->info("Generating output file: {}", this->output_file);
  auto board = this->scene.get_entity("Board");
  auto &tilemap = board.get_component<TileMapComponent>();
  auto &score = board.get_component<Score>().value;
  auto &moves = board.get_component<MoveLog>().moves;
  std::ofstream output(this->output_file);
  if(!output.is_open()) {
    Log::get_client_logger()->error("Couldn't open file: ", this->output_file);
  }
  for(auto move : moves) {
    switch(((int32_t)(move + 45) % 360) / 90) {
      case 0:
        output << 'U';
        break;
      case 1:
        output << 'R';
        break;
      case 2:
        output << 'D';
        break;
      case 3:
        output << 'L';
    }
  }
  output << "\n" << score << "\n";
  for(uint32_t y = 1; y < this->config.rows + 1; y++) {
    for(uint32_t x = 1; x < this->config.cols + 1; x++) {
      auto entities = tilemap.get_entities(this->scene, TransformComponent(x, y, 0));
      std::sort(entities.begin(), entities.end(), [](Entity &lhs, Entity &rhs){
        auto &lhs_trans = lhs.get_component<TransformComponent>();
        auto &rhs_trans = rhs.get_component<TransformComponent>();
        return glm::vec3(lhs_trans).z > glm::vec3(rhs_trans).z;
      });
      bool nothing = true;
      for(auto entity : entities) {
        Log::get_client_logger()->trace("({}, {}) = {}", x, y, entity.get_component<TagComponent>().name);
        if(entity.has_component<CursesSpriteComponent>()) {
          auto &sprite = entity.get_component<CursesSpriteComponent>().sprite;
          output << (char)sprite;
          Log::get_client_logger()->trace("({}, {}) = {}", x, y, (char)sprite);
          nothing = false;
          break;
        }
      }
      if(nothing) {
        output << ' ';
      }
    }
    output << "\n";
  }
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
  Log::get_client_logger()->info("Configuring game with '{}'", this->config_file);
  this->config.load_config(this->config_file);
  this->scene.add_engine(new TilePhysics());
  this->scene.add_renderer(new CursesRenderer());
  this->window = Window::create({"ActMan", this->config.cols + 2, this->config.rows + 2});
  this->window->set_fps(4);
}

void ActMan::create_board() {
  Entity entity = this->scene.create_entity();
  Log::get_core_logger()->info("Spawning Board ({}x{})", this->config.rows, this->config.cols);
  entity.add_component<TagComponent>("Board");
  entity.add_component<TransformComponent>(0, 0, 0);
  entity.add_component<TileMapComponent>(this->config.rows, this->config.cols);
  entity.add_component<Score>(0);
  entity.add_component<MoveLog>();
  entity.add_component<InputComponent>([](Application* app, Entity entity){
    if(Input::is_key_pressed(BA_KEY_Q)) {
      // quit
      app->close();
    }
  });
}

void ActMan::spawn_wall(const uint32_t x, const uint32_t y) {
  Entity entity = this->scene.create_entity();
  Log::get_client_logger()->info("Spawning Wall @ ({}, {})", x, y);
  entity.add_component<TagComponent>("Wall");
  entity.add_component<TransformComponent>(x, y, 0);
  entity.add_component<CursesSpriteComponent>('#', true);
  entity.add_component<TileColliderComponent>(true, true);
}

void ActMan::spawn_nugget(const uint32_t x, const uint32_t y) {
  Entity entity = this->scene.create_entity();
  Log::get_client_logger()->info("Spawning Gold Nugget @ ({},{})", x, y);
  entity.add_component<TagComponent>("Gold Nugget");
  entity.add_component<TransformComponent>(x, y, 0);
  entity.add_component<CursesSpriteComponent>('.', true);
}

void ActMan::spawn_bar(const uint32_t x, const uint32_t y) {
  Entity entity = this->scene.create_entity();
  Log::get_client_logger()->info("Spawning Gold Bar @ ({},{})", x, y);
  entity.add_component<TagComponent>("Gold Bar");
  entity.add_component<TransformComponent>(x, y, 0);
  entity.add_component<CursesSpriteComponent>('$', true);
}

void ActMan::spawn_diamond(const uint32_t x, const uint32_t y) {
  Entity entity = this->scene.create_entity();
  Log::get_client_logger()->info("Spawning Diamond @ ({},{})", x, y);
  entity.add_component<TagComponent>("Diamond");
  entity.add_component<TransformComponent>(x, y, 0);
  entity.add_component<CursesSpriteComponent>('*', true);
}

void ActMan::spawn_actman(const uint32_t x, const uint32_t y) {
  Entity entity = this->scene.create_entity();
  auto &scene = this->scene;
  Log::get_client_logger()->info("Spawning ActMan @ ({},{})", x, y);
  entity.add_component<TagComponent>("ActMan");
  entity.add_component<TransformComponent>(x, y, 0);
  entity.add_component<CursesSpriteComponent>('A', true);
  entity.add_component<TileMoveComponent>();
  entity.add_component<TurnComponent>(0);
  entity.add_component<TileColliderComponent>(true, false);
  entity.add_component<AIComponent>([](Application* app, Entity entity) {
    Log::get_client_logger()->trace("ai action() call for ActMan");
    static std::default_random_engine rng;
    auto seed = std::chrono::system_clock::now().time_since_epoch().count();
    Log::get_client_logger()->trace("Seed: {}", seed);
    rng.seed(seed);
    auto &tag = entity.get_component<TagComponent>();
    auto &tilemap = app->get_scene().get_entity("Board").get_component<TileMapComponent>();

    // Find valid moves
    auto free = tilemap.get_neighbors(entity.get_component<TransformComponent>());
    std::vector<TransformComponent> actions;
    for(auto neighbor : free) {
      bool accessable = true;
      for(auto entity : tilemap.get_entities(app->get_scene(), neighbor)) {
        if(entity.get_component<TagComponent>().name == "Wall") {
          accessable = false;
        }
      }
      if(accessable) {
        actions.push_back(neighbor);
      }
    }
    auto& transform = entity.get_component<TransformComponent>();
    auto& move = entity.get_component<TileMoveComponent>();
    bool found = false;

    auto &target = actions[(rng() - 1) % actions.size()];


    for(int i = 0; i < 4; i++) {
      Log::get_client_logger()->trace("({}, {}) -> ({}, {})", glm::vec3(transform).x, glm::vec3(transform).y, glm::vec3(target).x, glm::vec3(target).y);
      auto &target = actions[(rng() - 1) % actions.size()];
    }

    // Update move
    move.x = glm::vec3(target).x;
    move.y = glm::vec3(target).y;

    // Update rotation
    uint32_t dx = move.x - glm::vec3(transform).x;
    uint32_t dy = move.y - glm::vec3(transform).y;
    if (dx == 0) {
      if (dy == -1) {
        transform.rotation.z = 0;
      } else {
        transform.rotation.z = 180;
      }
    } else if (dx == 1) {
      transform.rotation.z = 90;
    } else {
      transform.rotation.z = 270;
    }

    auto &move_log = app->get_scene().get_entity("Board").get_component<MoveLog>().moves;
    move_log.emplace_back(transform.rotation.z);

    // Pick up Treasure and update score
    TransformComponent t;
    t.transform.x = move.x;
    t.transform.y = move.y;

    auto &score = app->get_scene().get_entity("Board").get_component<Score>();
    for(auto entity : tilemap.get_entities(app->get_scene(), t)) {
      auto &tag = entity.get_component<TagComponent>();
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
}

void ActMan::spawn_punky(const uint32_t x, const uint32_t y) {
  Entity entity = this->scene.create_entity();
  auto &scene = this->scene;
  Log::get_client_logger()->info("Spawning Punky @ ({},{})", x, y);
  float dir = this->config.dirs[(uint32_t)Ghost::PUNKY];
  entity.add_component<TagComponent>("Punky");
  entity.add_component<TransformComponent>(x, y, 1);
  entity.get_component<TransformComponent>().rotation.z = dir;
  entity.add_component<CursesSpriteComponent>('P', true);
  entity.add_component<TileColliderComponent>(true, false);
  entity.add_component<TileMoveComponent>();
  entity.add_component<TurnComponent>(1);
  entity.add_component<AIComponent>([](Application* app, Entity entity){
    auto &tilemap = app->get_scene().get_entity("Board").get_component<TileMapComponent>();

    auto &transform = entity.get_component<TransformComponent>();
    Log::get_client_logger()->trace("rotation addr: {:p}", (void*)(&transform.rotation));
    auto &move = entity.get_component<TileMoveComponent>();
    auto &tag = entity.get_component<TagComponent>();

    auto actman = app->get_scene().get_entity("ActMan");
    if(!actman) {
      move.x = glm::vec3(transform).x;
      move.y = glm::vec3(transform).y;
      return;
    }
    auto &at = actman.get_component<TransformComponent>();
    auto &amv = actman.get_component<TileMoveComponent>();

    std::vector<TransformComponent> actions;

    // Kill ActMan if he's moving onto Punky
    if(amv.x == glm::vec3(transform).x && amv.y == glm::vec3(transform).y) {
      auto grave = app->get_scene().create_entity();
      grave.add_component<TagComponent>("Grave");
      grave.add_component<TransformComponent>(glm::vec3(transform).x, glm::vec3(transform).y, 10);
      grave.add_component<CursesSpriteComponent>('X', true);
      Log::get_client_logger()->info("Punky killed ActMan!");
      app->close();
      actman.kill();
      return;
    }

    // Get valid moves
    auto free = tilemap.get_neighbors(entity.get_component<TransformComponent>());
    for(auto neighbor : free) {
      bool accessable = true;
      for(auto entity : tilemap.get_entities(app->get_scene(), neighbor)) {
        if(entity.get_component<TagComponent>().name == "Wall") {
          accessable = false;
        }
      }
      if(accessable) {
        actions.push_back(neighbor);
      }
    }

    auto dir = ((int32_t)(transform.rotation.z + 45) % 360) / 90;
    Log::get_client_logger()->trace("rotation: {}, dir: {}", transform.rotation.z, dir);
    TransformComponent choice;
    switch(actions.size()) {
      case 1: // Ghost Rule 1
        Log::get_client_logger()->trace("({}, {}) -> ({}, {})", glm::vec3(transform).x, glm::vec3(transform).y, glm::vec3(actions[0]).x, glm::vec3(actions[0]).y);
        choice.transform.x = glm::vec3(actions[0]).x;
        choice.transform.y = glm::vec3(actions[0]).y;
        break;
      case 2: // Ghost Rule 2
        Log::get_client_logger()->trace("({}, {})", glm::vec3(transform).x - (dir % 2)*(dir - 2), glm::vec3(transform).y + (1 - (dir % 2))*(dir - 1));
        for(auto m : actions) {
          Log::get_client_logger()->trace("({}, {}) -> ({}, {})", glm::vec3(transform).x, glm::vec3(transform).y, glm::vec3(m).x, glm::vec3(m).y);
          if(glm::vec3(m).x != glm::vec3(transform).x + (dir % 2)*(dir - 2) || glm::vec3(m).y != glm::vec3(transform).y - (1 - (dir % 2))*(dir - 1)) {
            choice.transform.x = glm::vec3(m).x;
            choice.transform.y = glm::vec3(m).y;
          }
          if(glm::vec3(m).x == glm::vec3(transform).x - (dir % 2)*(dir - 2) && glm::vec3(m).y == glm::vec3(transform).y + (1 - (dir % 2))*(dir - 1)) {
            break;
          }
        }
        break;
      default: // Punky Rule
        TransformComponent target;
        target.transform.x = amv.x;
        target.transform.y = amv.y;
        Log::get_client_logger()->trace("target: ({}, {})", amv.x, amv.y);
        int32_t dist = -1;
        for(auto m : actions) {
          int32_t d = tilemap.distance(target, m);
          Log::get_client_logger()->trace("({}, {}) -> ({}, {}) d: {}", glm::vec3(transform).x, glm::vec3(transform).y, glm::vec3(m).x, glm::vec3(m).y, d);
          if(d < dist || dist == -1) {
            choice.transform.x = glm::vec3(m).x;
            choice.transform.y = glm::vec3(m).y;
            dist = d;
            Log::get_client_logger()->trace("({}, {}) -> ({}, {}) dist: {}", glm::vec3(transform).x, glm::vec3(transform).y, glm::vec3(choice).x, glm::vec3(choice).y, dist);
          }
        }
        break;
    }

    // Update move
    Log::get_client_logger()->trace("({}, {}) -> ({}, {})", glm::vec3(transform).x, glm::vec3(transform).y, glm::vec3(choice).x, glm::vec3(choice).y);
    move.x = glm::vec3(choice).x;
    move.y = glm::vec3(choice).y;

    // Update Rotation
    int32_t dx = glm::vec3(choice).x - glm::vec3(transform).x;
    int32_t dy = glm::vec3(choice).y - glm::vec3(transform).y;
    Log::get_client_logger()->trace("choice: ({}, {})", glm::vec3(choice).x, glm::vec3(choice).y);
    Log::get_client_logger()->trace("dx: {}, dy: {}", dx, dy);
    transform.rotation.z = (std::abs(dx)*(2-dx) + std::abs(dy)*(dy+1))*90;
    Log::get_client_logger()->trace("rotation: {}", transform.rotation.z);
    Log::get_client_logger()->trace("rotation: {}", entity.get_component<TransformComponent>().rotation.z);

    // Kill ActMan if he moves onto the same tile as Punky
    if(amv.x == move.x && amv.y == move.y) {
      auto grave = app->get_scene().create_entity();
      grave.add_component<TagComponent>("Grave");
      grave.add_component<TransformComponent>(glm::vec3(choice).x, glm::vec3(choice).y, 10);
      grave.add_component<CursesSpriteComponent>('X', true);
      Log::get_client_logger()->info("Punky killed ActMan!");
      app->close();
      actman.kill();
      return;
    }
  });
}

void ActMan::spawn_bunky(const uint32_t x, const uint32_t y) {
  Entity entity = this->scene.create_entity();
  auto &scene = this->scene;
  Log::get_client_logger()->info("Spawning Bunky @ ({},{})", x, y);
  float dir = this->config.dirs[(uint32_t)Ghost::BUNKY];
  entity.add_component<TagComponent>("Bunky");
  entity.add_component<TransformComponent>(x, y, 2);
  entity.get_component<TransformComponent>().rotation.z = dir;
  entity.add_component<CursesSpriteComponent>('B', true);
  entity.add_component<TileColliderComponent>(true, false);
  entity.add_component<TileMoveComponent>();
  entity.add_component<TurnComponent>(2);
  entity.add_component<AIComponent>([](Application* app, Entity entity){
    auto &tilemap = app->get_scene().get_entity("Board").get_component<TileMapComponent>();

    auto &transform = entity.get_component<TransformComponent>();
    auto &move = entity.get_component<TileMoveComponent>();
    auto &tag = entity.get_component<TagComponent>();

    auto actman = app->get_scene().get_entity("ActMan");
    if(!actman) {
      move.x = glm::vec3(transform).x;
      move.y = glm::vec3(transform).y;
      return;
    }
    auto &at = actman.get_component<TransformComponent>();
    auto &amv = actman.get_component<TileMoveComponent>();

    std::vector<TransformComponent> actions;

    // Kill ActMan if he's moving onto Bunky
    if(amv.x == glm::vec3(transform).x && amv.y == glm::vec3(transform).y) {
      auto grave = app->get_scene().create_entity();
      grave.add_component<TagComponent>("Grave");
      grave.add_component<TransformComponent>(glm::vec3(transform).x, glm::vec3(transform).y, 10);
      grave.add_component<CursesSpriteComponent>('X', true);
      Log::get_client_logger()->info("Punky killed ActMan!");
      app->close();
      actman.kill();
      return;
    }

    // Get valid moves
    auto free = tilemap.get_neighbors(entity.get_component<TransformComponent>());
    for(auto neighbor : free) {
      bool accessable = true;
      for(auto entity : tilemap.get_entities(app->get_scene(), neighbor)) {
        if(entity.get_component<TagComponent>().name == "Wall") {
          accessable = false;
        }
      }
      if(accessable) {
        actions.push_back(neighbor);
      }
    }

    auto dir = ((int32_t)(transform.rotation.z + 45) % 360) / 90;
    TransformComponent choice;
    switch(actions.size()) {
      case 1: // Ghost Rule 1
        Log::get_client_logger()->trace("({}, {}) -> ({}, {})", glm::vec3(transform).x, glm::vec3(transform).y, glm::vec3(actions[0]).x, glm::vec3(actions[0]).y);
        choice.transform.x = glm::vec3(actions[0]).x;
        choice.transform.y = glm::vec3(actions[0]).y;
        break;
      case 2: // Ghost Rule 2
        for(auto m : actions) {
          Log::get_client_logger()->trace("({}, {}) -> ({}, {})", glm::vec3(transform).x, glm::vec3(transform).y, glm::vec3(m).x, glm::vec3(m).y);
          if(glm::vec3(m).x != glm::vec3(transform).x + (dir % 2)*(dir - 2) || glm::vec3(m).y != glm::vec3(transform).y - (1 - (dir % 2))*(dir - 1)) {
            choice.transform.x = glm::vec3(m).x;
            choice.transform.y = glm::vec3(m).y;
          }
          if(glm::vec3(m).x == glm::vec3(transform).x - (dir % 2)*(dir - 2) && glm::vec3(m).y == glm::vec3(transform).y + (1 - (dir % 2))*(dir - 1)) {
            break;
          }
        }
        break;
      default: // Bunky Rule
        TransformComponent target;
        auto tdir = ((int32_t)(target.rotation.z + 45) % 360 / 90); // Get direction of ActMan
        target.transform.x = amv.x - (tdir & 2)*(tdir - 2)*4;
        target.transform.y = amv.y + (1 - (tdir % 2))*(tdir - 1)*4;
        Log::get_client_logger()->trace("target: ({}, {})", glm::vec3(target).x, glm::vec3(target).y);
        int32_t dist = -1;
        for(auto m : actions) {
          int32_t d = tilemap.distance(target, m);
          Log::get_client_logger()->trace("({}, {}) -> ({}, {}) d: {}", glm::vec3(target).x, glm::vec3(target).y, glm::vec3(m).x, glm::vec3(m).y, d);
          if(d < dist || dist == -1) {
            choice.transform.x = glm::vec3(m).x;
            choice.transform.y = glm::vec3(m).y;
            dist = d;
            Log::get_client_logger()->trace("({}, {}) -> ({}, {}) dist: {}", glm::vec3(target).x, glm::vec3(target).y, glm::vec3(choice).x, glm::vec3(choice).y, dist);
          }
        }
        break;
    }

    // Update move
    Log::get_client_logger()->trace("({}, {}) -> ({}, {})", glm::vec3(transform).x, glm::vec3(transform).y, glm::vec3(choice).x, glm::vec3(choice).y);
    move.x = glm::vec3(choice).x;
    move.y = glm::vec3(choice).y;

    // Update Rotation
    uint32_t dx = glm::vec3(choice).x - glm::vec3(transform).x;
    uint32_t dy = glm::vec3(choice).y - glm::vec3(transform).y;
    if (dx == 0) {
      if (dy == -1) {
        transform.rotation.z = 0;
      } else {
        transform.rotation.z = 180;
      }
    } else if (dx == 1) {
      transform.rotation.z = 90;
    } else {
      transform.rotation.z = 270;
    }

    // Kill ActMan if he moves onto the same tile as Bunky
    if(amv.x == move.x && amv.y == move.y) {
      auto grave = app->get_scene().create_entity();
      grave.add_component<TagComponent>("Grave");
      grave.add_component<TransformComponent>(glm::vec3(choice).x, glm::vec3(choice).y, 10);
      grave.add_component<CursesSpriteComponent>('X', true);
      Log::get_client_logger()->info("Bunky killed ActMan!");
      app->close();
      actman.kill();
      return;
    }
  });
}

void ActMan::spawn_dunky(const uint32_t x, const uint32_t y) {
  Entity entity = this->scene.create_entity();
  auto &scene = this->scene;
  Log::get_client_logger()->info("Spawning Dunky @ ({},{})", x, y);
  float dir = this->config.dirs[(uint32_t)Ghost::DUNKY];
  entity.add_component<TagComponent>("Dunky");
  entity.add_component<TransformComponent>(x, y, 3);
  entity.get_component<TransformComponent>().rotation.z = dir;
  entity.add_component<CursesSpriteComponent>('D', true);
  entity.add_component<TileColliderComponent>(true, false);
  entity.add_component<TileMoveComponent>();
  entity.add_component<DunkyMemory>(this->config.patrol);
  entity.add_component<TurnComponent>(3);
  entity.add_component<AIComponent>([](Application* app, Entity entity){
    auto &tilemap = app->get_scene().get_entity("Board").get_component<TileMapComponent>();

    auto &move = entity.get_component<TileMoveComponent>();
    auto &transform = entity.get_component<TransformComponent>();
    auto &tag = entity.get_component<TagComponent>();

    auto actman = app->get_scene().get_entity("ActMan");
    if(!actman) {
      move.x = glm::vec3(transform).x;
      move.y = glm::vec3(transform).y;
      return;
    }
    auto &at = actman.get_component<TransformComponent>();
    auto &amv = actman.get_component<TileMoveComponent>();

    std::vector<TransformComponent> actions;

    // Kill ActMan if he's moving onto Punky
    if(amv.x == glm::vec3(transform).x && amv.y == glm::vec3(transform).y) {
      auto grave = app->get_scene().create_entity();
      grave.add_component<TagComponent>("Grave");
      grave.add_component<TransformComponent>(glm::vec3(transform).x, glm::vec3(transform).y, 10);
      grave.add_component<CursesSpriteComponent>('X', true);
      Log::get_client_logger()->info("Dunky killed ActMan!");
      app->close();
      actman.kill();
      return;
    }

    // Get valid moves
    auto free = tilemap.get_neighbors(entity.get_component<TransformComponent>());
    for(auto neighbor : free) {
      bool accessable = true;
      for(auto entity : tilemap.get_entities(app->get_scene(), neighbor)) {
        if(entity.get_component<TagComponent>().name == "Wall") {
          accessable = false;
        }
      }
      if(accessable) {
        actions.push_back(neighbor);
      }
    }

    auto dir = ((int32_t)(transform.rotation.z + 45) % 360) / 90;
    TransformComponent choice;
    auto &patrol = entity.get_component<DunkyMemory>().data;
    static uint32_t next = 0;
    if(glm::vec3(transform).x == patrol[next].x && glm::vec3(transform).y == patrol[next].y) {
      next = (next + 1) % patrol.size();
    }
    switch(actions.size()) {
      case 1: // Ghost Rule 1
        Log::get_client_logger()->trace("({}, {}) -> ({}, {})", glm::vec3(transform).x, glm::vec3(transform).y, glm::vec3(actions[0]).x, glm::vec3(actions[0]).y);
        choice.transform.x = glm::vec3(actions[0]).x;
        choice.transform.y = glm::vec3(actions[0]).y;
        break;
      case 2: // Ghost Rule 2
        for(auto m : actions) {
          Log::get_client_logger()->trace("({}, {}) -> ({}, {})", glm::vec3(transform).x, glm::vec3(transform).y, glm::vec3(m).x, glm::vec3(m).y);
          if(glm::vec3(m).x != glm::vec3(transform).x + (dir % 2)*(dir - 2) || glm::vec3(m).y != glm::vec3(transform).y - (1 - (dir % 2))*(dir - 1)) {
            choice.transform.x = glm::vec3(m).x;
            choice.transform.y = glm::vec3(m).y;
          }
          if(glm::vec3(m).x == glm::vec3(transform).x - (dir % 2)*(dir - 2) && glm::vec3(m).y == glm::vec3(transform).y + (1 - (dir % 2))*(dir - 1)) {
            break;
          }
        }
        break;
      default: // Punky Rule
        TransformComponent target;
        target.transform.x = patrol[next].x;
        target.transform.y = patrol[next].y;
        Log::get_client_logger()->trace("target: ({}, {})", amv.x, amv.y);
        int32_t dist = -1;
        for(auto m : actions) {
          int32_t d = tilemap.distance(target, m);
          Log::get_client_logger()->trace("({}, {}) -> ({}, {}) d: {}", glm::vec3(transform).x, glm::vec3(transform).y, glm::vec3(m).x, glm::vec3(m).y, d);
          if(d < dist || dist == -1) {
            choice.transform.x = glm::vec3(m).x;
            choice.transform.y = glm::vec3(m).y;
            dist = d;
            Log::get_client_logger()->trace("({}, {}) -> ({}, {}) dist: {}", glm::vec3(transform).x, glm::vec3(transform).y, glm::vec3(choice).x, glm::vec3(choice).y, dist);
          }
        }
        break;
    }

    // Update move
    Log::get_client_logger()->trace("({}, {}) -> ({}, {})", glm::vec3(transform).x, glm::vec3(transform).y, glm::vec3(choice).x, glm::vec3(choice).y);
    move.x = glm::vec3(choice).x;
    move.y = glm::vec3(choice).y;

    // Update Rotation
    uint32_t dx = glm::vec3(choice).x - glm::vec3(transform).x;
    uint32_t dy = glm::vec3(choice).y - glm::vec3(transform).y;
    if (dx == 0) {
      if (dy == -1) {
        transform.rotation.z = 0;
      } else {
        transform.rotation.z = 180;
      }
    } else if (dx == 1) {
      transform.rotation.z = 90;
    } else {
      transform.rotation.z = 270;
    }

    // Kill ActMan if he moves onto the same tile as Dunky
    if(amv.x == move.x && amv.y == move.y) {
      auto grave = app->get_scene().create_entity();
      grave.add_component<TagComponent>("Grave");
      grave.add_component<TransformComponent>(glm::vec3(choice).x, glm::vec3(choice).y, 10);
      grave.add_component<CursesSpriteComponent>('X', true);
      Log::get_client_logger()->info("Dunky killed ActMan!");
      app->close();
      actman.kill();
      return;
    }
  });
}

void ActMan::spawn_runky(const uint32_t x, const uint32_t y) {
  Entity entity = this->scene.create_entity();
  auto &scene = this->scene;
  float dir = this->config.dirs[(uint32_t)Ghost::RUNKY];
  Log::get_client_logger()->info("Spawning Runky @ ({},{})", x, y);
  entity.add_component<TagComponent>("Runky");
  entity.add_component<TransformComponent>(x, y, 4);
  entity.get_component<TransformComponent>().rotation.z = dir;
  entity.add_component<CursesSpriteComponent>('R', true);
  entity.add_component<TileColliderComponent>(true, false);
  entity.add_component<TileMoveComponent>();
  entity.add_component<TurnComponent>(4);
  entity.add_component<RunkyMemory>(this->config.dir_pool);
  entity.add_component<AIComponent>([](Application* app, Entity entity){
    auto &tilemap = app->get_scene().get_entity("Board").get_component<TileMapComponent>();

    auto &move = entity.get_component<TileMoveComponent>();
    auto &transform = entity.get_component<TransformComponent>();
    auto &tag = entity.get_component<TagComponent>();

    auto actman = app->get_scene().get_entity("ActMan");
    if(!actman) {
      move.x = glm::vec3(transform).x;
      move.y = glm::vec3(transform).y;
      return;
    }
    auto &at = actman.get_component<TransformComponent>();
    auto &amv = actman.get_component<TileMoveComponent>();

    std::vector<TransformComponent> actions;

    // Kill ActMan if he's moving onto Runky
    if(amv.x == glm::vec3(transform).x && amv.y == glm::vec3(transform).y) {
      auto grave = app->get_scene().create_entity();
      grave.add_component<TagComponent>("Grave");
      grave.add_component<TransformComponent>(glm::vec3(transform).x, glm::vec3(transform).y, 10);
      grave.add_component<CursesSpriteComponent>('X', true);
      Log::get_client_logger()->info("Runky killed ActMan!");
      app->close();
      actman.kill();
      return;
    }

    // Get valid moves
    auto free = tilemap.get_neighbors(entity.get_component<TransformComponent>());
    for(auto neighbor : free) {
      bool accessable = true;
      for(auto entity : tilemap.get_entities(app->get_scene(), neighbor)) {
        if(entity.get_component<TagComponent>().name == "Wall") {
          accessable = false;
        }
      }
      if(accessable) {
        actions.push_back(neighbor);
      }
    }

    auto dir = ((int32_t)(transform.rotation.z + 45) % 360) / 90;
    TransformComponent choice;
    auto &pool = entity.get_component<RunkyMemory>().data;
    static uint32_t next = 0;
    switch(actions.size()) {
      case 1: // Ghost Rule 1
        Log::get_client_logger()->trace("Rule 1");
        Log::get_client_logger()->trace("({}, {}) -> ({}, {})", glm::vec3(transform).x, glm::vec3(transform).y, glm::vec3(actions[0]).x, glm::vec3(actions[0]).y);
        choice.transform.x = glm::vec3(actions[0]).x;
        choice.transform.y = glm::vec3(actions[0]).y;
        break;
      case 2: // Ghost Rule 2
        Log::get_client_logger()->trace("Rule 2");
        for(auto m : actions) {
          Log::get_client_logger()->trace("({}, {}) -> ({}, {})", glm::vec3(transform).x, glm::vec3(transform).y, glm::vec3(m).x, glm::vec3(m).y);
          Log::get_client_logger()->trace("dir: {}", dir);
          if(glm::vec3(m).x != glm::vec3(transform).x + (dir % 2)*(dir - 2) || glm::vec3(m).y != glm::vec3(transform).y - (1 - (dir % 2))*(dir - 1)) {
            choice.transform.x = glm::vec3(m).x;
            choice.transform.y = glm::vec3(m).y;
          }
          if(glm::vec3(m).x == glm::vec3(transform).x - (dir % 2)*(dir - 2) && glm::vec3(m).y == glm::vec3(transform).y + (1 - (dir % 2))*(dir - 1)) {
            break;
          }
        }
        break;
      default: // Runky Rule
        Log::get_client_logger()->trace("Rule Runky");
        TransformComponent target;
        bool found = false;
        do {
          dir = ((int32_t)(pool[next] + 45) % 360) / 90;
          next = (next + 1) % pool.size();
          target.transform.x = glm::vec3(transform).x - (dir % 2)*(dir - 2);
          target.transform.y = glm::vec3(transform).y + (1 - (dir % 2))*(dir - 1);
          Log::get_client_logger()->trace("({}, {}) -> ({}, {})", glm::vec3(transform).x, glm::vec3(transform).y, glm::vec3(target).x, glm::vec3(target).y);
          for(auto m : actions) {
            Log::get_client_logger()->trace("({}, {}) ? ({}, {})", glm::vec3(target).x, glm::vec3(target).y, glm::vec3(m).x, glm::vec3(m).y);
            if(glm::vec3(m).x == glm::vec3(target).x && glm::vec3(m).y == glm::vec3(target).y) {
              found = true;
              choice.transform.x = glm::vec3(target).x;
              choice.transform.y = glm::vec3(target).y;
              break;
            }
          }
        } while(!found);
        break;
    }

    // Update move
    Log::get_client_logger()->trace("Move: ({}, {}) -> ({}, {})", glm::vec3(transform).x, glm::vec3(transform).y, glm::vec3(choice).x, glm::vec3(choice).y);
    move.x = glm::vec3(choice).x;
    move.y = glm::vec3(choice).y;


    // Update Rotation
    uint32_t dx = glm::vec3(choice).x - glm::vec3(transform).x;
    uint32_t dy = glm::vec3(choice).y - glm::vec3(transform).y;
    if (dx == 0) {
      if (dy == -1) {
        transform.rotation.z = 0;
      } else {
        transform.rotation.z = 180;
      }
    } else if (dx == 1) {
      transform.rotation.z = 90;
    } else {
      transform.rotation.z = 270;
    }

    // Kill ActMan if he moves onto the same tile as Runky
    if(amv.x == move.x && amv.y == move.y) {
      auto grave = app->get_scene().create_entity();
      grave.add_component<TagComponent>("Grave");
      grave.add_component<TransformComponent>(glm::vec3(choice).x, glm::vec3(choice).y, 10);
      grave.add_component<CursesSpriteComponent>('X', true);
      Log::get_client_logger()->info("Runky killed ActMan!");
      app->close();
      actman.kill();
      return;
    }
  });
}

void ActMan::reset() {
  this->scene.clear();
  uint32_t y = 1, x = 1;
  this->create_board();
  for(auto line : this->config.init_state) {
    for(auto cell : line) {
      switch(cell) {
        case '#': // Wall
          this->spawn_wall(x, y);
          break;
        case '.': // Gold Nugget
          this->spawn_nugget(x, y);
          break;
        case '$': // Gold Bar
          this->spawn_bar(x, y);
          break;
        case '*': // Diamond
          this->spawn_diamond(x, y);
          break;
        case 'A': // ActMan
          this->spawn_actman(x, y);
          break;
        case 'P': // Punky
          this->spawn_punky(x, y);
          break;
        case 'B': // Bunky
          this->spawn_bunky(x, y);
          break;
        case 'D': // Dunky
          this->spawn_dunky(x, y);
          break;
        case 'R': // Runky
          this->spawn_runky(x, y);
          break;
      }
      x++;
    }
    x = 1;
    y++;
  }
}
