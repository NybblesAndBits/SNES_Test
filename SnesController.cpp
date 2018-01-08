#include "SnesController.h"
#include "Arduino.h"

//=============================================================================
SnesController::SnesController(uint8_t clock, uint8_t latch, uint8_t data) {
	
	// Store the pins.
	this->clock = clock;
	this->latch = latch;
	this->data  = data;
	
	// Set data direction.
	pinMode(clock, OUTPUT);
	pinMode(latch, OUTPUT);
	pinMode(data, INPUT);
	
	// Default output states.
	digitalWrite(clock, LOW);
	digitalWrite(latch, LOW);
	
	// Default internal variable states.
	state = getRawState();
    change = 0;
	pollTime = 0;
	for (uint8_t i=0; i<BUTTON_COUNT; i++) changeTimes[i] = 0;
}

//=============================================================================
uint16_t SnesController::getRawState() {
	
	uint16_t rawState = 0;
	uint16_t mask = 0x0800;
	
	// Latch the controller button data.
	digitalWrite(latch, HIGH);
	digitalWrite(latch, LOW);
	
	// Clock in all twelve buttons
	for (uint8_t i=0; i<BUTTON_COUNT; i++) {
		
		// Read in a single bit.
		if (digitalRead(data)) rawState |= mask;

		// Shift the mask left
		mask >>= 1;
		
		// Clock
		digitalWrite(clk, HIGH);
		digitalWrite(clk, LOW);
	}
	
	return rawState;	
}

//=============================================================================
void SnesController::poll() {
	
}

//=============================================================================
bool SnesController::isPressed(uint16_t buttonMask) {
	
}

//=============================================================================
bool SnesController::wasPressed(uint16_t buttonMask) {
	
}

//=============================================================================
bool SnesController::wasReleased(uint16_t buttonMask) {
	
}

//=============================================================================
uint32_t getHeldTime(uint16_t buttonMask) {
	
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

