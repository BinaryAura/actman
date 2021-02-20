#ifndef INPUT_H
#define INPUT_H

class Input {
public:
  inline static bool is_key_pressed(int32_t keycode) {
    return instance->_is_key_pressed(keycode);
  }
  // Todo: Mouse Button Pressed
  // Todo: get Mouse X
  // Todo:: get Mouse Y
  // Todo:: get Mouse position
  inline static void _set_instance(Input* instance) {
    Input::instance = instance;
  }
protected:
  virtual bool _is_key_pressed(int32_t) = 0;
  // Todo: raw mouse button pressed
  // Todo: raw mouse x
  // Todo: raw mouse y
  // Todo: raw mouse position
private:
  static Input* instance;
};

#endif // INPUT_H
