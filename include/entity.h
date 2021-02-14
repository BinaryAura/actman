#ifndef ENTITY_H
#define ENTITY_H

#include <entt.hpp>

#include "scene.h"

class Entity {
public:
  Entity() = default;
  Entity(entt::entity id, Scene* scene) : id(id), scene(scene) {}

  template<typename T>
  bool has_component() {
    return this->scene->has_component<T>(this->id);
  }

  template<typename T, typename ... Args>
  T& add_component(Args&& ... args) {
    return this->scene->add_component<T>(this->id, std::forward<Args>(args) ...);
  }

  template<typename T>
  T& get_component() {
    return this->scene->get_component<T>(this->id);
  }

  template<typename T>
  T& remove_component() {
    return this->scene->remove_component<T>(this->id);
  }

  operator bool() const {
    return (uint32_t)this->id != 0;
  }

private:
  entt::entity id{ 0 };
  Scene* scene = nullptr;
};

#endif // ENTITY_H
