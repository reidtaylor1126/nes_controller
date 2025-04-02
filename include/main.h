#ifndef MAIN_H
#define MAIN_H

#include <Arduino.h>
#include <stdint.h>
#include "Joystick.h"
#include "Keyboard.h"
#include "Mouse.h"

#define PIN_CLOCK (6)
#define PIN_LATCH (5)
#define PIN_DATA (4)

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
    'x',                // A
    'z',                // B
    KEY_RIGHT_SHIFT,    // Select
    KEY_RETURN,         // Start
    KEY_UP_ARROW,       // Up
    KEY_DOWN_ARROW,     // Down
    KEY_LEFT_ARROW,     // Left
    KEY_RIGHT_ARROW     // Right
};

#define MOUSE_SCALE_X (5)
#define MOUSE_SCALE_Y (5)
#define MOUSE_SCALE_SCROLL (-1)

uint8_t pollButtons();
void digitalPulse(uint8_t pin);
void digitalPulse(uint8_t pin, uint16_t duration);
void triggerJoystick();
void triggerKeyboard();
void triggerMouse();
void clearAll();

#endif