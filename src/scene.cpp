#include "scene.h"
#include "entity.h"

Entity Scene::create_entity() {
  return { this->registry.create(), this };
}
