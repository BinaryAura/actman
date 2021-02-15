#include "scene.h"
#include "entity.h"
#include "components/transform.h"
#include "components/cursessprite.h"
#include "components/tag.h"
#include "components/ai.h"
#include "components/tilemove.h"
#include "components/tilecollider.h"
#include "log.h"

#include <iostream>

Scene::Scene() {
  this->curses_renderer = new CursesRenderer();
  this->tile_engine = new TilePhysics();
}

Entity Scene::create_entity() {
  return { this->registry.create(), this };
}

void Scene::on_input() {
  // User Input
  // AIS
  auto group = this->registry.group<AI>(entt::get<Transform, TileMove>);
  for(auto entity : group) {
    auto [ai, transform, move] = group.get(entity);
    ai.action(*this, {entity, this});
    // Log::get_core_logger()->info("x({}) -> {}, y({}) -> {}", transform.x, move.x, transform.y, move.y);
  }
}

void Scene::on_update() {
  // Physics
  // Board Moves
  auto group = this->registry.group<TileMove>(entt::get<Transform, TileCollider>);
  for(auto entity : group) {
    auto [move, transform, collider] = group.get(entity);
    this->tile_engine->on_update(*this, transform, (Mobile&)move, (Collider&)collider);
    // Log::get_core_logger()->info("x({}), y({})", transform.x, transform.y);
  }
}

void Scene::on_render(Window* window) {
  // Update Animations
  // Buffers are not swapped here!
  this->curses_renderer->clear(window);
  auto group = this->registry.group<CursesSprite>(entt::get<Transform, Tag>);
  group.sort<Transform>([](const auto& lhs, const auto& rhs)->bool{
    return lhs.z < rhs.z;
  });
  for(auto entity : group) {
    auto [sprite, transform, tag] = group.get(entity);
    this->curses_renderer->on_update(window, transform, (Sprite&)sprite);
  }

}

Entity Scene::get_entity(const std::string& name) {
  auto view = this->registry.view<Tag>();
  for(auto entity : view) {
    auto &tag = view.get<Tag>(entity);
    if(tag.name == name)
      return {entity, this};
  }
  return Entity();
}
