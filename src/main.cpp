
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
uint8_t detected = 0;
uint16_t heldTime = 0;
uint16_t count = 0;

void setup() {

  Serial.begin(9600);

  pinMode(PIN_CLOCK, OUTPUT);
  pinMode(PIN_LATCH, OUTPUT);
  pinMode(PIN_DATA, INPUT);

  pinMode(LED_BUILTIN_RX, OUTPUT);

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

  detected = state > 0;

  // Serial.print(state, BIN);

  if (detected) {

    count++;
    if (count / 15 <= mode) {
      if (count % 15 == 0) {
        digitalWrite(LED_BUILTIN_RX, LOW);
      } else if (count % 15 == 8) {
        digitalWrite(LED_BUILTIN_RX, HIGH);
      }
    }
    if (count > 200)
      count = 0;

    changes = state ^ lastState;
    newHigh = changes & state;
    newLow = changes & lastState;
  
    if ((~state & 4) && (~state & 8)) {
      heldTime++;
      Serial.println(heldTime);
      if (heldTime > 300) {
        heldTime = 0;
        clearAll();
        for(uint8_t i = 0; i < 3; i++) {
          digitalPulse(LED_BUILTIN_RX, 160);
          delay(160);
        }
        digitalWrite(LED_BUILTIN_RX, HIGH);
        mode = (mode + 1) % 4;
      }
    } else {
      if (heldTime > 0)
      heldTime--;
    }
    
    if (mode == 0)
      triggerJoystick();
    else if (mode == 1)
      triggerKeyboard(standardKeys);
    else if (mode == 2)
      triggerKeyboard(overcookedKeys);
    else if (mode == 3)
      triggerMouse();
  
    lastState = state;
    
    // delay(10);
    
    delay(2);
  } else {
    count++;
    if (count == 5)
      digitalWrite(LED_BUILTIN_RX, HIGH);

    else if (count >= 10) {
      digitalWrite(LED_BUILTIN_RX, LOW);
      count = 0;
    }

    delay(100);
  }
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
  digitalPulse(pin, 1);
}

void digitalPulse(uint8_t pin, uint16_t duration) {
  digitalWrite(pin, HIGH);
  delay(duration);
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

void triggerKeyboard(uint8_t keyMap[]) {

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
    Mouse.press(MOUSE_LEFT);
  else if (newHigh & 1)
    Mouse.release(MOUSE_LEFT);

  if (newLow & 2) 
    Mouse.press(MOUSE_RIGHT);
  else if (newHigh & 2)
    Mouse.release(MOUSE_RIGHT);

  if (newLow & 4) 
    Mouse.press(MOUSE_MIDDLE);
  else if (newHigh & 4)
    Mouse.release(MOUSE_MIDDLE);

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
  Mouse.release(MOUSE_ALL);
  Joystick.setXAxis(0);
  Joystick.setYAxis(0);
  for (int i = 0; i < 4; i++) {
    Joystick.releaseButton(buttonMap[i]);
  }
}