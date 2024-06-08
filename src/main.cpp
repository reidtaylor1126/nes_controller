#include <Arduino.h>
#include "Joystick.h"
#include "Keyboard.h"
#include "main.h"

// TODO migrate to XInput

Joystick_ Joystick(3, JOYSTICK_TYPE_JOYSTICK, 
                  10, 0, 
                  true, true, false, 
                  false, false, false,
                  false, false, false, 
                  false, false);

uint8_t state, lastState;
int lastX, lastY;

void setup() {

  pinMode(PIN_CLOCK, OUTPUT);
  pinMode(PIN_LATCH, OUTPUT);
  pinMode(PIN_DATA, INPUT);

  digitalWrite(PIN_CLOCK, LOW);
  digitalWrite(PIN_LATCH, LOW);

  Keyboard.begin();
  Joystick.begin();
  Joystick.setXAxisRange(-1, 1);
  Joystick.setYAxisRange(-1, 1);
}

void loop() {

  state = pollButtons();

  // triggerJoystick(state, lastState);
  triggerKeyboard(state, lastState);

  lastState = state;
  
  delay(10);
}

uint8_t pollButtons() {
  uint8_t state = 255;
  uint8_t mask = 1;

  digitalPulse(PIN_LATCH);

  for (int i = 0; i < 8; i++) {
    if (digitalRead(PIN_DATA) == LOW) {
      state &= ~mask;
    }
    mask = mask << 1;
    digitalPulse(PIN_CLOCK);
  }

  return ~state;
}

void digitalPulse(uint8_t pin) {
  digitalWrite(pin, HIGH);
  digitalWrite(pin, LOW);
}

void triggerJoystick(uint8_t state, uint8_t lastState) {
  uint8_t changes = state ^ lastState;
  uint8_t newHigh = changes & state;
  uint8_t newLow = changes & lastState;

  uint8_t mask = 1;
  int xAxis = 0;
  int yAxis = 0;

  for (int i = 0; i < 8; i++) {
    if (i < 4) {
      if (newHigh & mask) {
        Joystick.pressButton(buttonMap[i]);
      } else if (newLow & mask) {
        Joystick.releaseButton(buttonMap[i]);
      }
    } else {
      uint8_t value = (state & mask) >> i;
      if (i == 4) {
        yAxis -= value;
      } else if (i == 5) {
        yAxis += value;
      } else if (i == 6) {
        xAxis -= value;
      } else if (i == 7) {
        xAxis += value;
      }
    }
    mask = mask << 1;
  }

  if (xAxis != lastX) {
    Joystick.setXAxis(xAxis);
    lastX = xAxis;
  }
  if (yAxis != lastY) {
    Joystick.setYAxis(yAxis);
    lastY = yAxis;
  }
}

void triggerKeyboard(uint8_t state, uint8_t lastState) {
  uint8_t changes = state ^ lastState;
  uint8_t newHigh = changes & state;
  uint8_t newLow = changes & lastState;

  uint8_t mask = 1;
  int xAxis = 0;
  int yAxis = 0;

  for (int i = 0; i < 8; i++) {
    if (newHigh & mask) {
      Keyboard.press(keyMap[i]);
    } else if (newLow & mask) {
      Keyboard.release(keyMap[i]);
    }
    mask = mask << 1;
  }    
}