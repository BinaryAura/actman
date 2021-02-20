#ifndef COMPONENTS_AI_H
#define COMPONENTS_AI_H

class Application;

struct AI {
  AI() = default;
  AI(const AI&) = default;

  AI(void (* const action)(Application*, Entity)) : action(action) {}
  void (*action)(Application *, Entity);
};

#endif // COMPONENTS_AI_H
