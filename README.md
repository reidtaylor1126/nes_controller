# NES Controller Adapter

A project for Arduino that converts from standard NES controller to HID input.
Includes Joystick, Keyboard, and Mouse modes.

## Hardware

- Arduino Pro Micro/Leonardo: I'm using a Leonardo in a micro form factor since it was the cheapest option
- 7-Pin NES Controller Port

## Assembly

Connect the VCC and GND pins to the corresponding pins on the board. 

Connect D1 to Pin 4, OUT0 to Pin 5, and CUP to Pin 6 to use the default configuration. Otherwise, change the `PIN_DATA`, `PIN_LATCH`, and `PIN_CLOCK` constants in [main.h](include/main.h) to match your hardware setup.

[![Pinout Diagram](https://psmay.com/wp-content/uploads/2011/10/nes-controller-pinout.png)](https://psmay.com/wp-content/uploads/2011/10/nes-controller-pinout.png)

## Usage

Plug an NES controller into the port. If a controller is not detected, the RX LED will blink slowly.

As mentioned above, the software includes three modes: Joystick, Keyboard, and Mouse. Holding SELECT and START for ~4 seconds will cycle to the next mode in the order listed. The RX LED will blink rapidly 3 times to signal that the mode has changed.

Most mappings are configurable in [main.h](include/main.h).

### Joystick Mode

This mode is fairly self-explanatory. The D-Pad acts as a simple 2-axis stick, and A, B, SELECT, and START are mapped to act as their counterparts on, for example, a Playstation controller.

### Keyboard Mode

This mode maps buttons to keys used commonly by emulators. By default, the D-Pad is mapped to the arrow keys, with A, B, SELECT, and START mapped to X, Z, RSHIFT, and ENTER respectively.

### Mouse Mode

This mode emulates basic mouse functions. The D-Pad moves the mouse cursor. A and B are mapped to left and right click. SELECT is mapped to middle click. While holding START, the D-Pad switches to scroll mode.