#ifndef COMPONENTS_TILE_MOVE_H
#define COMPONENTS_TILE_MOVE_H

#include "components/mobile.h"

struct TileMove : public Mobile {
  TileMove() = default;
  TileMove(const TileMove&) = default;
  TileMove(const uint32_t x, const uint32_t y): x(x), y(y) {}

  uint32_t x;
  uint32_t y;
};

#endif // COMPONENTS_TILE_MOVE_H
