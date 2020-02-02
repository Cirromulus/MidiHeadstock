#include <Arduino.h>
#include "MIDIUSB.h"

// First parameter is the event type (0x09 = note on, 0x08 = note off).
// Second parameter is note-on/note-off, combined with the channel.
// Channel can be anything between 0-15. Typically reported to the user as 1-16.
// Third parameter is the note number (48 = middle C).
// Fourth parameter is the velocity (64 = normal, 127 = fastest).

struct Button{
    const uint8_t input;
    const uint8_t led;
    const uint8_t channel;
    bool lastState;
};

constexpr bool default_buttonstate = false;

Button buttons[] =
{
    {0,1,0, default_buttonstate},
    {2,3,1, default_buttonstate},
    {4,5,2, default_buttonstate},
    {6,7,3, default_buttonstate},
};




void sendCTL(uint8_t channel, bool state)
{
    // controlChange(0, 10, 65); // Set the value of controller 10 on channel 0 to 65
    midiEventPacket_t event = {0x0B, channel | 0xB0, 0, state ? 255 : 0};
    MidiUSB.sendMIDI(event);
}

void setup() {
    //Serial.begin(115200);
    for(Button& b : buttons)
    {
        pinMode(b.input, INPUT);
        pinMode(b.led, OUTPUT);
    }
}

// First parameter is the event type (0x0B = control change).
// Second parameter is the event type, combined with the channel.
// Third parameter is the control number number (0-119).
// Fourth parameter is the control value (0-127).
/*
void controlChange(uint8_t channel, uint8_t control, uint8_t value) {
    midiEventPacket_t event = {0x0B, 0xB0 | channel, control, value};
    MidiUSB.sendMIDI(event);
}
*/

void loop() {
    for(Button& b : buttons)
    {
        bool state = digitalRead(b.input);
        if (state != b.lastState)
        {
            sendCTL(b.channel, state);
            digitalWrite(b.led, state);
            b.lastState = state;
        }
    }
    MidiUSB.flush();
}
