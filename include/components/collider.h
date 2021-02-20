#ifndef COMPONENTS_COLLIDER_H
#define COMPONENTS_COLLIDER_H

struct Collider {
  Collider() = default;
  Collider(const Collider&) = default;
  Collider(const bool solid) : solid(solid) {}
  bool solid;
};

#endif // COMPONENTS_COLLIDER_H
