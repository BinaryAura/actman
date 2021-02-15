#ifndef SCENE_H
#define SCENE_H

#include <entt.hpp>
#include "window/window.h"
#include "physics/tilephysics.h"
#include "renderer/cursesrenderer.h"

class Entity;

class Scene {
public:
  Scene();

  Entity create_entity();

  template<typename T>
  bool has_component(entt::entity& entity) {
    return this->registry.has<T>(entity);
  }

  template<typename T, typename ... Args>
  T& add_component(entt::entity& entity, Args&& ... args) {
    return this->registry.emplace<T>(entity, std::forward<Args>(args) ...);
  }

  template<typename T>
  T& get_component(entt::entity& entity) {
    return this->registry.get<T>(entity);
  }

  template<typename T>
  T& remove_component(entt::entity& entity) {
    return this->registry.remove<T>(entity);
  }

  void destroy_entity(const entt::entity& entity) {
    this->registry.destroy(entity);
  }

  template<typename ... Owned, typename ... Get, typename ... Exclude>
  entt::basic_group<entt::entity, entt::exclude_t<Exclude ...>, entt::get_t<Get ...>, Owned ...> get_group(entt::get_t<Get ...>, entt::exclude_t<Exclude ...> = {}) {
    return this->registry.group<Owned ...>(entt::get_t<Get ...>{}, entt::exclude_t<Exclude ...>{});
  }

  Entity get_entity(const std::string&);

  template<typename ... T, typename Filter, typename ... Exclude>
  std::vector<Entity> filtered_view(Filter filter, entt::exclude_t<Exclude ...> = {}) {
    auto view = this->registry.view<T ...>();
    std::vector<Entity> fg;
    for(auto entity : view) {
      if(filter(Entity{entity, this})) {
        fg.emplace_back(entity, this);
      }
    }
    return fg;
  }

  template<typename Filter, typename ... Owned, typename ... Get, typename ... Exclude>
  std::vector<Entity> filtered_group(Filter filter, entt::get_t<Get ...>, entt::exclude_t<Exclude ...>) {
    auto group = this->registry.group<Owned ...>(entt::get_t<Get ...>{}, entt::exclude_t<Exclude ...>{});
    std::vector<Entity> fg;
    for(auto entity : group) {
      if(filter(Entity{entity, this})) {
        fg.emplace_back(entity, this);
      }
    }
    return fg;
  }

  void on_input();

  void on_update();

  void on_render(Window* window);

private:
  entt::registry registry;
  CursesRenderer* curses_renderer;
  TilePhysics* tile_engine;
};

#endif // SCENE_H
