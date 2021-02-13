#ifndef SCENE_H
#define SCENE_H

#include <entt.hpp>

class Entity;

class Scene {
public:
  Scene() = default;

  Entity create_entity();

  template<typename T>
  bool has_component(entt::entity entity) {
    return this->registry.has<T>(entity);
  }

  template<typename T, typename ... Args>
  T& add_component(entt::entity entity, Args&& ... args) {
    return this->registry.emplace<T>(entity, std::forward<Args>(args) ...);
  }

  template<typename T>
  T& get_component(entt::entity entity) {
    return this->registry.get<T>(entity);
  }

  template<typename T>
  T& remove_component(entt::entity entity) {
    return this->registry.remove<T>(entity);
  }

private:
  entt::registry registry;
};

#endif // SCENE_H
