/*********************************************************************
* FILENAME:  customCharsFor4an.ino
* AUTHOR:    reidrumack@gmail.com <Reid Rumack>
* DATE:      23 June 2016
* MICRO:     ATtiny 85
* PRODUCTs:     
* VERSION:   1.2
**********************************************************************/

#include "TinyWireM.h"
#include "Tiny_Adafruit_LEDBackpack.h"

#define  QUAD_ALPHA_NUM_ADDR 0x71
#define  BAR_GRAPH_ADDR      0x70

#define  CHAR_MAX             8
#define  TOTAL_LIGHTS         23
#define  UPDATE_TIME_SHIFT    25
#define  BAR_DELAY_RANGE      75
#define  DELAY_TIME           750

Tiny_Adafruit_AlphaNum4 alpha4 = Tiny_Adafruit_AlphaNum4();
Tiny_Adafruit_24bargraph bar = Tiny_Adafruit_24bargraph();

const uint16_t CUSTOM_CHARS[8]={0x11A8, 0x1445, 0x0474, 0x08A2,
        0x1181, 0x0A84, 0x0790, 0x1980};
uint8_t         lastMax     = 0;
static uint16_t lastUpdated = 0;

char displaybuffer[4] = {' ', ' ', ' ', ' '};


void setup() {
    alpha4.begin(QUAD_ALPHA_NUM_ADDR);  // pass in the address
    bar.begin(BAR_GRAPH_ADDR);
}



void loop() {
   lastUpdated = changeChars();
   lastMax = changeLevel();
}


uint16_t changeChars(){
    uint8_t char0Index, char1Index, char2Index, char3Index;
    char0Index = random(CHAR_MAX);
    char1Index = random(CHAR_MAX);
    char2Index = random(CHAR_MAX);
    char3Index = random(CHAR_MAX);

    uint16_t currentTime =  millis();
    if((currentTime - lastUpdated) > DELAY_TIME){
        alpha4.writeDigitRaw(0, CUSTOM_CHARS[char0Index]);
        alpha4.writeDigitRaw(1, CUSTOM_CHARS[char1Index]);
        alpha4.writeDigitRaw(2, CUSTOM_CHARS[char2Index]);
        alpha4.writeDigitRaw(3, CUSTOM_CHARS[char3Index]);
        alpha4.writeDisplay();
    }
    return currentTime;

}
uint8_t changeLevel() {
      uint8_t newLevel, randomDelay, barDelay,  prevMax;
      
     newLevel = random(TOTAL_LIGHTS);
     randomDelay = random(BAR_DELAY_RANGE);
      prevMax = lastMax;
      barDelay = randomDelay + UPDATE_TIME_SHIFT;
      
    if(lastMax < newLevel){
        for (uint8_t b = prevMax ; b < newLevel; b++) {
            bar.setBar(TOTAL_LIGHTS-b, LED_GREEN);
            bar.writeDisplay();
            delay(barDelay);
        }
    
    }else{
        for (uint8_t b = prevMax; b > newLevel ; b--) {
            bar.setBar(TOTAL_LIGHTS-b, LED_OFF);
            bar.writeDisplay();
            delay(barDelay);
        }
    }
    return newLevel;
}


