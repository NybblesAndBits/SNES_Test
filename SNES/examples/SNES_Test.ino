#include "SNES.h"

// SNES(clock, latch, data);
SNES snes(12, 11, 10);

void setup() {
  Serial.begin(9600);
}

void loop() {
  // refresh() should be called at a consistant rate. Every loop iteration usually works well.
  // After refresh is called you can check which buttons were pressed or released as seen
  // below.
  snes.refresh();

  // Something like
  // if(millis() - lastRefresh > 33) refresh();
  // Could be done to call refresh at about 30Hz

  // Check each button for pressed and releases.
  // These should only be checked after refresh() is called.
  if(snes.wasPressed(SNES_B)) Serial.println("B Pressed");
  if(snes.wasReleased(SNES_B)) Serial.println("B Released");

  if(snes.wasPressed(SNES_A)) Serial.println("A Pressed");
  if(snes.wasReleased(SNES_A)) Serial.println("A Released");

  if(snes.wasPressed(SNES_X)) Serial.println("X Pressed");
  if(snes.wasReleased(SNES_X)) Serial.println("X Released");

  if(snes.wasPressed(SNES_Y)) Serial.println("Y Pressed");
  if(snes.wasReleased(SNES_Y)) Serial.println("Y Released");

  if(snes.wasPressed(SNES_L)) Serial.println("L Pressed");
  if(snes.wasReleased(SNES_L)) Serial.println("L Released");

  if(snes.wasPressed(SNES_R)) Serial.println("R Pressed");
  if(snes.wasReleased(SNES_R)) Serial.println("R Released");

  if(snes.wasPressed(SNES_START)) Serial.println("Start Pressed");
  if(snes.wasReleased(SNES_START)) Serial.println("Start Released");

  if(snes.wasPressed(SNES_SELECT)) Serial.println("Select Pressed");
  if(snes.wasReleased(SNES_SELECT)) Serial.println("Select Released");

  if(snes.wasPressed(SNES_UP)) Serial.println("Up Pressed");
  if(snes.wasReleased(SNES_UP)) Serial.println("Up Released");

  if(snes.wasPressed(SNES_DOWN)) Serial.println("Down Pressed");
  if(snes.wasReleased(SNES_DOWN)) Serial.println("Down Released");

  if(snes.wasPressed(SNES_RIGHT)) Serial.println("Right Pressed");
  if(snes.wasReleased(SNES_RIGHT)) Serial.println("Right Released");

  if(snes.wasPressed(SNES_LEFT)) Serial.println("Left Pressed");
  if(snes.wasReleased(SNES_LEFT)) Serial.println("Left Released");
  
}
