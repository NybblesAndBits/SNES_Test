#include "SNES.h"
#include "Arduino.h"

SNES::SNES(int clk, int latch, int data) {
  this->clk = clk;
  this->latch = latch;
  this->data = data;

  pinMode(clk, OUTPUT);
  pinMode(latch, OUTPUT);
  pinMode(data, INPUT);

  for (int i = 0; i < 12; i++) lastTimes[i] = 0;
  rawRefresh();
  currState = getRawState();
  lastState = currState;
}

void SNES::rawRefresh() {
  int mask = 0b100000000000;

  // Latch the inputs
  digitalWrite(latch, HIGH);
  digitalWrite(latch, LOW);

  lastRawState = rawState;
  rawState = 0x0000;

  // Clock in all twelve buttons
  for (int i = 0; i < 12; i++) {
    if (digitalRead(data)) rawState |= mask;

    // Shift the mask left
    mask >>= 1;
    // Clock
    digitalWrite(clk, HIGH);
    digitalWrite(clk, LOW);
  }

  // Invert and trim off leading 4 bits
  rawState = ~rawState;
  rawState &= 0x0FFF;
}

int SNES::getRawState() {
  return rawState;
}

bool SNES::isHeld(uint16_t button) {
  return getRawState() & button;
}

bool SNES::rawChanged() {
  return ((lastRawState ^ rawState) != 0);
}

void SNES::refresh() {
  rawRefresh(); // rawState is set the the current state

  updateFlags = 0; // Clear all of the update flags

  int mask = 0b100000000000;
  bool bitChange;

  lastState = currState; // Store the old state. Never used in this method but used to detect a change in changed()

  for (int i = 0; i < 12; i++) {
    bitChange = (rawState ^ currState) & mask; // If the current state differs from the last state

    if (bitChange && (millis() - lastTimes[i] > SNES_DEBOUNCE)) { // If there was a difference and enough time has passed
      lastTimes[i] = millis();
      updateFlags |= mask; // Flag that this button has been changed
      currState ^= mask; // Flip the state of this button's bit
    }
    mask >>= 1;
  }
}

int SNES::getState() {
  return currState;
}

bool SNES::wasPressed(uint16_t button) {
  // Check if the button is pressed and if it has changed since the last check
  return (updateFlags & button) && (currState & button);
}

bool SNES::wasReleased(uint16_t button) {
  // Check if the button is not pressed and if it has changed since the last check
  return (updateFlags & button) && (~currState & button);
}

bool SNES::changed() {
  // Check if there is any differences between the last and current state
  return (currState ^ lastState);
}

