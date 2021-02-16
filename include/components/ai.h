#ifndef COMPONENTS_AI_H
#define COMPONENTS_AI_H

class Scene;

struct AI {
  AI() = default;
  AI(const AI&) = default;

  AI(void (* const action)(Scene&, Entity)) : action(action) {}
  void (*action)(Scene &, Entity);
};

#endif // COMPONENTS_AI_H
