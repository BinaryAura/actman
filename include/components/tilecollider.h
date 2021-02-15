#ifndef COMPONENTS_TILE_COLLIDER_H
#define COMPONENTS_TILE_COLLIDER_H

#include "components/collider.h"

struct TileCollider : public Collider {
  TileCollider() = default;
  TileCollider(const TileCollider&) = default;
  TileCollider(const bool solid, const bool fills_tile) : Collider(solid), fills_tile(fills_tile) {}

  bool fills_tile;
};

#endif // COMPONENTS_TILE_COLLIDER_H
