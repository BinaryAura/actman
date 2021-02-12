#ifndef TRANSFORM_H
#define TRANSFORM_H

class Transform : public Component {
public:
  static const uint32_t COMPONENT_ID = __COUNTER__;
  Transform() : x(x), y(y) {}
protected:
  uint32_t x, y;
}

#endif // TRANSFORM_H
