#ifndef TILE_PHYSICS_H
#define TILE_PHYSICS_H

#include "physics/physics.h"

class TilePhysics {
public:
  void on_update(Scene&, Transform&, Mobile&, Collider&);
};

#endif // TILE_PHYSICS_H
