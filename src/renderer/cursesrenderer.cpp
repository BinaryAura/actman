#include "renderer/cursesrenderer.h"
#include "components/cursessprite.h"

using RawCursesWindow = WINDOW;

void CursesRenderer::on_update(Window* window, Transform& transform, Sprite& sprite) const {
  RawCursesWindow* cwindow = (RawCursesWindow*)window;
  CursesSprite& csprite = (CursesSprite&)sprite;

  if(((CursesSprite&)sprite).visible) {
      mvwaddch((RawCursesWindow*)window, transform.y, transform.x, ((CursesSprite&)sprite).sprite);
  }
}
