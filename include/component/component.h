#ifndef COMPONENT_H
#define COMPONENT_H

class Component {
public:
  static const uint32_t COMPONENT_ID = __COUNTER__;
  Component() {
    this->id = n_components++;
  }

  virtual update() = 0;
protected:
  static uint32_t n_components = 0;
  uint32_t id;
}

#endif // COMPONENT_H
