// #include <Arduino.h>
// // #include "Joystick.h"
// #include <XInput.h>
// #include "main.h"

// // Joystick_ Joystick(3, JOYSTICK_TYPE_JOYSTICK, 
// //                   10, 0, 
// //                   true, true, false, 
// //                   false, false, false,
// //                   false, false, false, 
// //                   false, false);

// uint8_t state, lastState;
// int lastX, lastY;

// void setup() {

//   pinMode(PIN_CLOCK, OUTPUT);
//   pinMode(PIN_LATCH, OUTPUT);
//   pinMode(PIN_DATA, INPUT);

//   digitalWrite(PIN_CLOCK, LOW);
//   digitalWrite(PIN_LATCH, LOW);

//   Serial.begin(9600);

//   XInput.setAutoSend(false);
//   XInput.begin();

// //   Joystick.begin();
// //   Joystick.setXAxisRange(-1, 1);
// //   Joystick.setYAxisRange(-1, 1);
// }

// void loop() {

//   state = pollButtons();

//   triggerJoystick(state, lastState);

//   lastState = state;
  
//   delay(10);
// }

// uint8_t pollButtons() {
//   uint8_t state = 255;
//   uint8_t mask = 1;

//   digitalPulse(PIN_LATCH);

//   for (int i = 0; i < 8; i++) {
//     if (digitalRead(PIN_DATA) == LOW) {
//       state &= ~mask;
//     }
//     mask = mask << 1;
//     digitalPulse(PIN_CLOCK);
//   }

//   return ~state;
// }

// void digitalPulse(uint8_t pin) {
//   digitalWrite(pin, HIGH);
//   digitalWrite(pin, LOW);
// }

// void triggerJoystick(uint8_t state, uint8_t lastState) {
//   uint8_t changes = state ^ lastState;
//   uint8_t newHigh = changes & state;
//   uint8_t newLow = changes & lastState;

//   // if (newHigh | newLow) {
//   //   Serial.println(state);
//   // }

//   uint8_t mask = 1;

//   for (int i = 0; i < 8; i++) {
//     if (i < 4) {
//       if (newHigh & mask) {
//         // Serial.println(buttonMap[i]);
//         // Joystick.pressButton(buttonMap[i]);
//         XInput.press(buttonMap[i]);
//       } else if (newLow & mask) {
//         // Serial.println(-buttonMap[i]);
//         // Joystick.releaseButton(buttonMap[i]);
//         XInput.release(buttonMap[i]);
//       }
//     } else {
//         if (state & changes & mask) {
//           if (i == 4) {
//             XInput.setJoystickY(JOY_LEFT, -32768);
//           } else if (i == 5) {
//             XInput.setJoystickY(JOY_LEFT, 32768);
//           } else if (i == 6) {
//             XInput.setJoystickX(JOY_LEFT, -32768);
//           } else if (i == 7) {
//             XInput.setJoystickX(JOY_LEFT, 32768);
//           }
//         } else if (lastState & changes & mask) {
//           if (i == 4 || i == 5) {
//             XInput.setJoystickY(JOY_LEFT, 0);
//           } else if (i == 6 || i == 7) {
//             XInput.setJoystickX(JOY_LEFT, 0);
//           }

//         }
//     }
//     mask = mask << 1;
//   }

// }