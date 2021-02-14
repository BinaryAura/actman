#include "scene.h"
#include "entity.h"
#include "components/transform.h"
#include "components/cursessprite.h"

Entity Scene::create_entity() {
  return { this->registry.create(), this };
}

void Scene::on_input() const {
  // User Input
  // AIS
}

void Scene::on_update(Physics* physics) const {
  // Physics
  // Board Moves


}

void Scene::on_render(Window* window) const {
  // Update Animations
  // Buffers are not swapped here!

  // I can't get this to work. Will be submiting an issue to entt repo
  // auto group = this->registry.group<Transform>(entt::get<CursesSprite>());
  // for(auto entity : group) {
  //   this->curses_renderer->on_update(window, group.get<Transform>(entity), (Sprite&)group.get<CursesSprite>(entity));
  // }
  auto view = this->registry.view<CursesSprite>();

}
