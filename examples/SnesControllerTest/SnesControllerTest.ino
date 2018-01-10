#include "SnesController.h"

// Pin definitions
const uint8_t CLOCK = 10;
const uint8_t LATCH = 11;
const uint8_t DATA  = 12;

// Create a SnesController object
SnesController snes(CLOCK, LATCH, DATA);

// Some variables to limit the print
// interval when performing the hold test.
int heldCount = 0;
uint32_t heldInterval = 500;

//=============================================================================
// This function runs once during startup.
void setup() {
	Serial.begin(9600);
}

//=============================================================================
// This function repeats forever.
void loop() {

	// Poll is called to read the state of the controller.
	snes.poll();

	// The isPressed() function can be used to check if
	// a particular button is currently being pressed.
	// As long as the button is being held down, isPressed()
	// will always return true.
	if(snes.isPressed(SnesController::START)) {
		Serial.println("Start is pressed.");
	}

	// The wasPressed() function can be used to detect
	// if a button was just pressed. This function will
	// only return true when a button was not pressed
	// during the last poll() but now is.
	if(snes.wasPressed(SnesController::A)) {
		Serial.println("A was pressed.");
	}

	// The wasReleased() function can be used to detect
	// if a button was just released. This works the
	// same way as the wasPressed() function but only
	// returns turn when a button was pressed during the
	// last poll() but now is not.
	if(snes.wasReleased(SnesController::A)) {
		Serial.println("A was released.");
	}

	// The getHeldTime() function can be used to check
	// how long a button has been held down in ms.
	uint32_t nextInterval = heldCount * heldInterval;
	if(snes.getHeldTime(SnesController::R) > nextInterval) {
		Serial.print("R has been held for ");
		Serial.print(nextInterval)
		Serial.println(" ms.");
		heldCount++;
	}

	// Reset the held count if R was released.
	if(snes.wasReleased(SnesController::R)) {
		heldCount = 0;
	}
}
