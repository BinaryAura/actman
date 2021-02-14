#ifndef SCENE_H
#define SCENE_H

#include <entt.hpp>
#include "physics/physics.h"
#include "window/window.h"
#include "renderer/cursesrenderer.h"

class Entity;

class Scene {
public:
  Scene() = default;

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

  void on_input() const;

  void on_update(Physics* physics) const;

  void on_render(Window* window) const;

private:
  entt::registry registry;
  CursesRenderer* curses_renderer;
};

#endif // SCENE_H
