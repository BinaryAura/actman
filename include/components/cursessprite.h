#ifndef COMPONENTS_CURSES_SPRITE_H
#define COMPONENTS_CURSES_SPRITE_H

#include <ncurses.h>
#include "components/sprite.h"

struct CursesSprite : public Sprite {
  CursesSprite() = default;
  CursesSprite(const CursesSprite&) = default;
  CursesSprite(const chtype sprite, const bool visible) : Sprite(visible), sprite(sprite) {}
  // Todo: color
  chtype sprite;
};

#endif // COMPONENTS_CURSES_SPRITE_H
