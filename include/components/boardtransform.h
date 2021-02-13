#ifndef BOARD_TRANSFORM_H
#define BOARD_TRANSFORM_H


enum class Direction {NORTH, EAST, SOUTH, WEST, NUM_DIRS};

struct BoardTransform {
  BoardTransform() = default;
  BoardTransform(const BoardTransform&)  = default;
  BoardTransform(uint32_t x, uint32_t y, Direction dir) : x(x), y(y), dir(dir) {}

  uint32_t x;
  uint32_t y;
  Direction dir;
};

#endif // BOARD_TRANSFORM_H
