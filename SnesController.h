#ifndef SNES_CONTROLLER_H
#define SNES_CONTROLLER_H

#include <stdint.h>     // uint16_t

#define SNES_CONTROLLER_MASK2INDEX(mask) (((mask) >> 12) & 0x000F)

class SnesController {
private:
  //===========================================================================
  // The amount of time in milliseconds each button needs to remained in a
  // pressed or released state before it is allowed to change state again.
  // This is used to help mediate button bouncingsg.
  static const uint32_t BOUNCE_TIME = 50;

  //===========================================================================
  // The number of buttons on the SNES controller. This is used for array
  // bounds checking.
  static const uint8_t BUTTON_COUNT = 12;

public:
  //===========================================================================
  // Bit masks for each of the buttons.
  static const uint16_t B      = 0x0800;
  static const uint16_t Y      = 0x1400;
  static const uint16_t SELECT = 0x2200;
  static const uint16_t START  = 0x3100;
  static const uint16_t UP     = 0x4080;
  static const uint16_t DOWN   = 0x5040;
  static const uint16_t LEFT   = 0x6020;
  static const uint16_t RIGHT  = 0x7010;
  static const uint16_t A      = 0x8008;
  static const uint16_t X      = 0x9004;
  static const uint16_t L      = 0xA002;
  static const uint16_t R      = 0xB001;

private:
  //===========================================================================
  // Local copies of the required pins.
  uint8_t clock;
  uint8_t latch;
  uint8_t data;

  //===========================================================================
  // Internal variables for keeping state.
  uint16_t state;
  uint16_t change;
  uint32_t pollTime;
  uint32_t changeTimes[BUTTON_COUNT];

public:
  //===========================================================================
  // Constructor that accepts the pins for clocking, latching, and data input.
  SnesController(uint8_t clock, uint8_t latch, uint8_t data);

  //===========================================================================
  // This function returns a raw 16-bit value read from the controller. This
  // can be used in place of poll() if raw bit manipulation is desired for
  // speed. This does NOT update any internal state variables so other
  // functions (isPressed(), wasPressed(), etc.) are not valid.
  uint16_t getRawState();

  //===========================================================================
  // This function reads in the current state of the SNES controller. It also
  // updates internal timing variables that are used to determine the outcome
  // of wasPressed(), wasReleased(), and getHeldTime().
  void poll();

  //===========================================================================
  // Returns true if the provided button is currently being pressed.
  bool isPressed(uint16_t buttonMask);

  //===========================================================================
  // Returns true if the provided button has just been pressed during the
  // last poll(). This function will not return true again until poll() has
  // detected that the button has been released and pressed again.
  bool wasPressed(uint16_t buttonMask);

  //===========================================================================
  // Returns true if the provided button has just been released during the
  // last poll(). This function will not return true again until poll() has
  // detected that the button has been pressed and released again.
  bool wasReleased(uint16_t buttonMask);

  //===========================================================================
  // Returns the amount of time (in milliseconds) that the provided button
  // has been held down for. If the button is not currently pressed 0 is
  // returned.
  uint32_t getHeldTime(uint16_t buttonMask);
};

#endif // SNES_CONTROLLER_H
