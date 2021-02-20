#ifndef COMPONENTS_TURN_H
#define COMPONENTS_TURN_H

struct Turn {
  Turn() = default;
  Turn(const Turn&) = default;
  Turn(const uint32_t number) : number(number) {}

  uint32_t number;
};

#endif // COMPONENTS_TURN_H
