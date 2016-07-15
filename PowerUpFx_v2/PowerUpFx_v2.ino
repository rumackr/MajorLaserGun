/**********************************************************************
* FILENAME: PowerUpFx_v2
* AUTHOR:   reidrumack@gmail.com <Reid Rumack>
* DATE:     16 June 2016
* Arduino:  Mini Pro
* PRODUCT:  Adafruit NeoPixel(60)
**********************************************************************/

#include <Adafruit_NeoPixel.h>

//power up effect
#define PWRUP 2
#define PWRLEDs 10


Adafruit_NeoPixel pwrStrip = Adafruit_NeoPixel(
        PWRLEDs, PWRUP, NEO_GRB + NEO_KHZ800);
void setup() {
    pwrStrip.begin();
}

void loop() {
    pwrUpFx(); // Blue
    pwrDownFx();
}

static void pwrUpFx(void) {
    delay(25);
    for (uint8_t i = 0; i < 10; i++) {
        pwrStrip.setPixelColor(i, 0, 0, 1+(i * 5), 0);
        pwrStrip.show();
                delay(25);

    }
    delay(500);
}

static void pwrDownFx(void) {
    for (uint8_t i = 0; i < 10; i++) {
        pwrStrip.setPixelColor(i, 0, 0, 0, 0);
        pwrStrip.show();
        delay(25);
    }
}

//static void randomBlink(){
//  
//    for( 
//}

