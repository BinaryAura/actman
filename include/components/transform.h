#ifndef COMPONENTS_TRANSFORM_H
#define COMPONENTS_TRANSFORM_H

struct Transform {
  Transform() = default;
  Transform(const Transform&) = default;
  Transform(const float x, const float y, const float rotation) : x(x), y(y), rotation(rotation) {}

  float x;
  float y;
  float rotation;
};

#endif // COMPONENTS_TRANSFORM_H
