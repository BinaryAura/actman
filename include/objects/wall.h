#ifndef WALL_H
#define WALL_H

#include <stdint.h>

#include "objects/gameobject.h"

class Wall : public GameObject {
public:
  Wall(uint32_t rows, uint32_t cols) : GameObject(rows, cols) {
    this->visable = true;
    this->solid = true;
  }
private:
};

#endif // WALL_H
