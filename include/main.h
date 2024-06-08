#ifndef MAIN_H
#define MAIN_H

#include <stdint.h>
#include <XInput.h>

#define PIN_CLOCK (4)
#define PIN_LATCH (5)
#define PIN_DATA (6)

#define BUTTON_A (0)
#define BUTTON_B (1)
#define BUTTON_SEL (8)
#define BUTTON_STA (9)
#define BUTTON_U (4)
#define BUTTON_D (5)
#define BUTTON_L (6)
#define BUTTON_R (7)

uint8_t buttonMap[4] = {
    BUTTON_A, BUTTON_B, BUTTON_SEL, BUTTON_STA
};

uint8_t keyMap[8] = {
    'x', 'z', KEY_ESC, KEY_RETURN, KEY_UP_ARROW, KEY_DOWN_ARROW, KEY_LEFT_ARROW, KEY_RIGHT_ARROW
};

uint8_t pollButtons();
void digitalPulse(uint8_t pin);
void triggerJoystick(uint8_t state, uint8_t lastState);
void triggerKeyboard(uint8_t state, uint8_t lastState);
// void triggerXInput(uint8_t state, uint8_t lastState);

#endif