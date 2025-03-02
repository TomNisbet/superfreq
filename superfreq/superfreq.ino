#include "ssd1306lite.h"

// Declare the global instance of the display
SSD1306Display display;

const byte FREQ_PIN = 2;
volatile unsigned long ticksRise;
volatile unsigned long ticksFall;
volatile unsigned long ticksLow;
volatile unsigned long ticksHigh;

void isrPinChange() {
    if (digitalRead(2)) { //PIND & 0x04) {
        ticksRise = micros();
        ticksLow = ticksRise - ticksFall;
    } else {
        ticksFall = micros();
        ticksHigh = ticksFall - ticksRise;
    }
}

void setup() {
    delay(50);
    display.initialize();
    display.clear();
    display.text2x(0, 0, "Freq:         Hz");
    display.text2x(2, 0, "High:         ms");
    display.text2x(4, 0, "Low:          ms");
    display.text2x(6, 0, "Duty:          %");

    ticksRise = ticksFall = micros();
    pinMode(FREQ_PIN, INPUT_PULLUP);
    attachInterrupt(digitalPinToInterrupt(FREQ_PIN), isrPinChange, CHANGE);
}


void loop() {
    delay(1000);
    char buffer[20];
    unsigned long myLow = ticksLow;
    unsigned long myHigh = ticksHigh;
    float f;
    int prec;

    f = 1000000.0 / (myLow + myHigh);
    prec = f < 10.0 ? 2 : 0;
    dtostrf(f, 9, prec, buffer);
    display.text2x(0, 5*8, buffer);

    f = myHigh / 1000.0;
    prec = f >= 1000.0 ? 0 : 3; 
    dtostrf(f, 9, prec, buffer);
    display.text2x(2, 5*8, buffer);

    f = myLow / 1000.0;
    prec = f >= 1000.0 ? 0 : 3; 
    dtostrf(f, 9, prec, buffer);
    display.text2x(4, 5*8, buffer);

    dtostrf(myHigh * 100.0 / (myHigh + myLow), 10, 2, buffer);
    display.text2x(6, 5*8, buffer);
}



