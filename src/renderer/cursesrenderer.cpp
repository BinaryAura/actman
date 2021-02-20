#include <iostream>

#include "log.h"
#include "renderer/cursesrenderer.h"
#include "components/cursessprite.h"

using RawCursesWindow = WINDOW;

void CursesRenderer::clear(Window* window) const {
  Log::get_core_logger()->trace("Clear Window");
  werase((RawCursesWindow*)window->get_native_window());
}

void CursesRenderer::on_update(Window* window, Transform& transform, Sprite& sprite) const {
  if(sprite.visible) {
    mvwaddch((RawCursesWindow*)window->get_native_window(), transform.y, transform.x, ((CursesSprite&)sprite).sprite);
  }
}
