/**********************************************************************
  FILENAME:  ledPrograms
  AUTHOR:    rumackr@msoe.edu <Reid Rumack>
  DATE:      16 June 2016
  PROSSESOR: Arduino Mini Pro
  PRODUCT:   Adafruit NeoPixel(60)
**********************************************************************/

#include <Adafruit_NeoPixel.h>

//power up effect
#define PWRUP 2
#define PWRLEDs 10

Adafruit_NeoPixel pwrStrip = Adafruit_NeoPixel(PWRLEDs, PWRUP, NEO_RGBW + NEO_KHZ800);

void setup() {
    pwrStrip.begin();
}

void loop() {
    pwrUpFx();
    pwrDownFx();
}

static void pwrUpFx(void) {
    delay(15);
    for (uint8_t i = 0; i < 10; i++) {
        pwrStrip.setPixelColor(i, 0, 0, 50, 0);
        pwrStrip.show();
        delay(100);
    }
    delay(500);
}

static void pwrDownFx(void) {
    for (uint8_t i = 0; i < 10; i++) {
        pwrStrip.setPixelColor(i, 0, 0, 0, 0);
        pwrStrip.show();
    }
}
