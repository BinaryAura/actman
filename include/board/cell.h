#ifndef CELL_H
#define CELL_H

#include <stdint.h>

class Cell {
public:
  uint32_t x, y;
  Cell(uint32_t x=0, uint32_t y=0) : x(x), y(y) {}

  bool operator== (const Cell &other) {
    return this->x == other.x && this->y == other.y;
  }
};

#endif // CELL_H
