#ifndef BUTTON_H
#define BUTTON_H
#endif

#include <Arduino.h>

class Button {
protected:
  uint8_t pin;
  uint8_t state;

public:
  Button(uint8_t _pin) {
    pin = _pin;
    pinMode(pin, INPUT);
  }

  void update() {
    state = digitalRead(pin);
  }

  uint8_t getState() { return state; }
  uint8_t getPin() { return pin; }
};
