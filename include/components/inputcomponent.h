#ifndef COMPONENTS_INPUT_COMPONENT_H
#define COMPONENTS_INPUT_COMPONENT_H

#include "window/window.h"
#include "scene.h"
#include "entity.h"

struct InputComponent {
  InputComponent() = default;
  InputComponent(const InputComponent&) = default;

  InputComponent(void (* const action)(Application*, Entity)) : action(action) {}
  void (*action)(Application*, Entity);
};

#endif // COMPONENTS_INPUT_COMPONENT_H
