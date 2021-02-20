#ifndef COMPONENTS_SPRITE_H
#define COMPONENTS_SPRITE_H

struct Sprite {
  Sprite() = default;
  Sprite(const Sprite&) = default;
  Sprite(const bool visible) : visible(visible) {}

  bool visible;
};

#endif // COMPONENTS_SPRITE_H
