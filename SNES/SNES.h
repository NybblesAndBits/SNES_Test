#ifndef SNES_H
#define SNES_H

#include <stdint.h>

#define SNES_DEBOUNCE 40
#define SNES_B      0b100000000000
#define SNES_Y      0b010000000000
#define SNES_SELECT 0b001000000000
#define SNES_START  0b000100000000
#define SNES_UP     0b000010000000
#define SNES_DOWN   0b000001000000
#define SNES_LEFT   0b000000100000
#define SNES_RIGHT  0b000000010000
#define SNES_A      0b000000001000
#define SNES_X      0b000000000100
#define SNES_L      0b000000000010
#define SNES_R      0b000000000001

class SNES {

  private:  
    // Pins
    int clk;
    int latch;
    int data;

    // Members
    int rawState;
    int lastRawState;

    int lastState;
    int currState;
    long lastTimes[12];
    int updateFlags;

  public:
    // Constructors
    SNES(int clk, int latch, int data);

    //==Methods============================
    // For collecting the raw state of the controller
    void rawRefresh();
    int getRawState();
    bool isHeld(uint16_t button);
    bool rawChanged();

    // For collecting the debounced state of the controller
    void refresh();
    int getState();
    bool wasPressed(uint16_t button);
    bool wasReleased(uint16_t button);
    bool changed();
};

#endif // SNES_H
