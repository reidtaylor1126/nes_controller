
#include "main.h"

Joystick_ Joystick(3, JOYSTICK_TYPE_JOYSTICK, 
                  10, 0, 
                  true, true, false, 
                  false, false, false,
                  false, false, false, 
                  false, false);

uint8_t state, lastState, changes, newHigh, newLow;
int lastX, lastY;
uint8_t mode = 0;

uint16_t held_time=0;

void setup() {

  Serial.begin(9600);

  pinMode(PIN_CLOCK, OUTPUT);
  pinMode(PIN_LATCH, OUTPUT);
  pinMode(PIN_DATA, INPUT);

  digitalWrite(PIN_CLOCK, LOW);
  digitalWrite(PIN_LATCH, LOW);

  Keyboard.begin();
  Mouse.begin();
  Joystick.begin();
  Joystick.setXAxisRange(-1, 1);
  Joystick.setYAxisRange(-1, 1);

  Serial.println("Initialized");
}


void loop() {

  state = 0;

  state = pollButtons();

  // Serial.print(state, BIN);

  changes = state ^ lastState;
  newHigh = changes & state;
  newLow = changes & lastState;

  if ((~state & 4) && (~state & 8)) {
    held_time++;
    Serial.println(held_time);
    if (held_time > 300) {
      held_time = 0;
      clearAll();
      mode = (mode + 1) % 3;
    }
  } else {
    if (held_time > 0)
      held_time--;
  }
  
  if (mode == 1)
    triggerKeyboard();
  else if (mode == 0)
    triggerJoystick();
  else if (mode == 2)
    triggerMouse();

  lastState = state;
  
  // delay(10);
  
  delay(2);
}

uint8_t pollButtons() {
  uint8_t internalState = 255;
  uint8_t mask = 1;

  digitalPulse(PIN_LATCH);

  for (int i = 0; i < 8; i++) {
    // Serial.println(i);
    // Serial.println(digitalRead(PIN_DATA));
    if (digitalRead(PIN_DATA) == LOW) {
      internalState &= ~mask;
    }
    mask = mask << 1;
    digitalPulse(PIN_CLOCK);
  }

  // Serial.println((uint8_t)internalState);

  return internalState;
}

void digitalPulse(uint8_t pin) {
  digitalWrite(pin, HIGH);
  delay(1);
  digitalWrite(pin, LOW);
}

void triggerJoystick() {

  uint8_t mask = 1;
  int xAxis = 0;
  int yAxis = 0;

  for (int i = 0; i < 8; i++) {
    if (i < 4) {
      if (newLow & mask) {
        Joystick.pressButton(buttonMap[i]);
      } else if (newHigh & mask) {
        Joystick.releaseButton(buttonMap[i]);
      }
    } else {
      uint8_t value = (state & mask) >> i;
      if (i == 4) {
        yAxis += value;
      } else if (i == 5) {
        yAxis -= value;
      } else if (i == 6) {
        xAxis += value;
      } else if (i == 7) {
        xAxis -= value;
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

void triggerKeyboard() {

  uint8_t mask = 1;

  for (int i = 0; i < 8; i++) {
    if (newLow & mask) {
      Keyboard.press(keyMap[i]);
    } else if (newHigh & mask) {
      Keyboard.release(keyMap[i]);
    }
    mask = mask << 1;
  }    
}

void triggerMouse() {

  uint8_t scroll = ((~state) & 8
) > 0;
  uint8_t xAxis = 0;
  uint8_t yAxis = 0;

  if (newLow & 1) 
    Mouse.press(1);
  else if (newHigh & 1)
    Mouse.release(1);

  if (newLow & 2) 
    Mouse.press(3);
  else if (newHigh & 2)
    Mouse.release(3);

  if (newLow & 4) 
    Mouse.press(2);
  else if (newHigh & 4)
    Mouse.release(2);

  yAxis -= (~state & 16) >> 4;
  yAxis += (~state & 32) >> 5;
  xAxis -= (~state & 64) >> 6;
  xAxis += (~state & 128) >> 7;

  if (scroll)
    Mouse.move(0, 0, yAxis * MOUSE_SCALE_SCROLL);
  else
    Mouse.move(xAxis * MOUSE_SCALE_X, yAxis * MOUSE_SCALE_Y);  

}

void clearAll() {
  Keyboard.releaseAll();
  Mouse.release(0);
  Mouse.release(1);
  Mouse.release(2);
  Mouse.release(3);
  Joystick.setXAxis(0);
  Joystick.setYAxis(0);
  for (int i = 0; i < 4; i++) {
    Joystick.releaseButton(buttonMap[i]);
  }
}