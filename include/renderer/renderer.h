#ifndef RENDERER_H
#define RENDERER_H

#include "components/sprite.h"
#include "components/transform.h"
#include "window/window.h"

class Renderer {
public:
  Renderer() = default;
  virtual void clear(Window*) const = 0;
  virtual void on_update(Window*, Transform&, Sprite&) const = 0;
};

#endif // RENDERER_H
