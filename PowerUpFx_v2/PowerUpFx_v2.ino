/**********************************************************************
* FILENAME:  PowerUpFx_v2                                             
* AUTHOR:    reidrumack@gmail.com <Reid Rumack>                           
* DATE:      16 June 2016
* PROSSESOR: Arduino Mini Pro                                              
* PRODUCT:   Adafruit NeoPixel(60)                                                            
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
  pwrUpFx(); // Blue
  pwrDownFx();
}

static void pwrUpFx(void) {
  delay(25);
  uint8_t count = 0;
  while(count < 10){
      pwrStrip.setPixelColor(count, 0, 0, 1+(count * 5), 0);
      pwrStrip.show();
      count++;
      delay(100);
  }
  delay(500);
}

static void pwrDownFx(void) {
  uint8_t count = 0;
  while(count < 10){
      pwrStrip.setPixelColor(count, 0, 0, 0, 0);
      pwrStrip.show();
      count++;  
  }
}
