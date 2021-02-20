#ifndef COMPONENTS_TRANSFORM_H
#define COMPONENTS_TRANSFORM_H

#include <math.h>

struct Transform {
  Transform() = default;
  Transform(const Transform&) = default;
  Transform(const float x, const float y, const float z, const float rotation) : x(x), y(y), z(z), rotation(rotation) {}
  float distance(const Transform& other) const {
    return sqrt(this->x*this->x + this->y*this->y + this->z*this->z);
  }

  float distance2d(const Transform& other) const {
    return sqrt(this->x*this->x + this->y*this->y);
  }

  float taxicap_distance(const Transform& other) const {
    return abs(this->x - other.x) + abs(this->y - other.y);
  }

  float x;
  float y;
  float z;
  float rotation;

};

#endif // COMPONENTS_TRANSFORM_H
