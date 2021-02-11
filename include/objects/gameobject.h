#ifndef GAMEOBJECT_H
#define GAMEOBJECT_H

#include "cell.h"

#define sprite_t char

class GameObject {
public:
  GameObject(uint32_t x = 0, uint32_t y = 0) : x(x), y(y) {}

  Cell get_cell() {
    return {this->x, this->y};
  }

  bool is_solid() {
    return this->solid;
  }

  bool is_visable() {
    return this->visable;
  }

  sprite_t get_sprite() {
    return this->sprite;
  }

protected:
  uint32_t x, y;
  bool solid;
  bool visable;
  sprite_t sprite;
};

#endif // GAMEOBJECT_H
