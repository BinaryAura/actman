#include "scene.h"
#include "entity.h"
#include "components/transform.h"
#include "components/cursessprite.h"
#include "components/tag.h"
#include "components/ai.h"
#include "components/tilemove.h"
#include "components/tilecollider.h"
#include "components/turn.h"
#include "log.h"

#include <iostream>

Scene::Scene() {
  this->curses_renderer = new CursesRenderer();
  this->tile_engine = new TilePhysics();
}

Entity Scene::create_entity() {
  auto entity = this->registry.create();
  Log::get_core_logger()->trace("Creating Entity: {}", entity);
  return {entity , this};
}

bool Scene::is_valid(entt::entity& entity) {
  return this->registry.valid(entity);
}

void Scene::clear() {
  this->registry.clear();
}

void Scene::on_input(Application* app) {
  // User Input
  // AIS
  Log::get_core_logger()->trace("on_input() call");
  auto group = this->registry.group<AI>(entt::get<Transform, TileMove, Tag, Turn>);
  group.sort<Turn>([](auto &lhs, auto &rhs){
    return lhs.number < rhs.number;
  });
  for(auto entity : group) {
    auto [ai, transform, move, tag, turn] = group.get(entity);
    Log::get_core_logger()->trace("AI for {} @ ({}, {})", tag.name, transform.x, transform.y);
    ai.action(app, {entity, this});
    // Log::get_core_logger()->info("x({}) -> {}, y({}) -> {}", transform.x, move.x, transform.y, move.y);
  }
}

void Scene::on_update() {
  // Physics
  // Board Moves
  Log::get_core_logger()->trace("on_update() call");
  auto group = this->registry.group<TileMove>(entt::get<Transform, TileCollider, Tag>);
  for(auto entity : group) {
    auto [move, transform, collider, tag] = group.get(entity);
    Log::get_core_logger()->trace("TilePhysics for {} @ ({}, {})", tag.name, transform.x, transform.y);
    this->tile_engine->on_update(*this, transform, (Mobile&)move, (Collider&)collider);
    // Log::get_core_logger()->info("x({}), y({})", transform.x, transform.y);
  }
}

void Scene::on_render(Window* window) {
  // Update Animations
  // Buffers are not swapped here!
  Log::get_core_logger()->trace("on_render() call");
  this->curses_renderer->clear(window);
  auto group = this->registry.group<CursesSprite>(entt::get<Transform, Tag>);
  group.sort<Transform>([](const auto& lhs, const auto& rhs)->bool{
    return lhs.z < rhs.z;
  });
  for(auto entity : group) {
    auto [sprite, transform, tag] = group.get(entity);
    Log::get_core_logger()->trace("CursesRender for {} @ ({}, {})", tag.name, transform.x, transform.y);
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
