#ifndef PHYSICS_H
#define PHYSICS_H

#include "components/transform.h"
#include "components/mobile.h"
#include "components/collider.h"

class Scene;

class Physics {
public:
  virtual void on_update(Scene&, Transform&, Mobile&, Collider&) = 0;
};

#endif // PHYSICS_H
