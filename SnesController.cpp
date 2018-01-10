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

	uint16_t newState = 0;
	uint16_t mask = 0x0800;
	change = 0;

	// Latch the controller button data.
	digitalWrite(latch, HIGH);
	digitalWrite(latch, LOW);
	pollTime = mills();

	// Clock in all twelve buttons
	for (uint8_t i=0; i<BUTTON_COUNT; i++) {

		// Read in a single bit.
		if (digitalRead(data)) newState |= mask;

		// if newState[bit] != state[bit]
		if((newState ^ state) & mask) {
			if(pollTime - changeTime[SNES_CONTROLLER_MASK2INDEX(mask)] > BOUNCE_TIME) {
				state |= newState & mask;
				change |= mask;
				changeTime[SNES_CONTROLLER_MASK2INDEX(mask)] = pollTime;
			}
		}

		// Shift the mask left
		mask >>= 1;

		// Clock
		digitalWrite(clk, HIGH);
		digitalWrite(clk, LOW);
	}
}

//=============================================================================
bool SnesController::isPressed(uint16_t buttonMask) {
	return (state & buttonMask);
}

//=============================================================================
bool SnesController::wasPressed(uint16_t buttonMask) {
	return (change & buttonMask) && (state & buttonMask);
}

//=============================================================================
bool SnesController::wasReleased(uint16_t buttonMask) {
	return (change & buttonMask) && ((~state) & buttonMask);
}

//=============================================================================
uint32_t getHeldTime(uint16_t buttonMask) {
	if(SNES_CONTROLLER_MASK2INDEX(buttonMask) < BUTTON_COUNT) {
		if(state & mask) {
			return pollTime - changeTime[SNES_CONTROLLER_MASK2INDEX(buttonMask)];
		}
	}
	return 0;
}
