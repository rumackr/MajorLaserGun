/**********************************************************************
* FILENAME:  ModeAndVolumeControl.ino
* AUTHOR:    reidrumack@gmail.com <Reid Rumack>
* DATE:      23 June 2016
* Arduino:   Mini Pro
* PRODUCT:   LCD 1602A
* VERSION:   1.0
**********************************************************************/
#include <SoftwareSerial.h>

// amount +/- ADC value alowed for error
#define ERR 30
// difines the pin that the switch are are on
#define SWITCHS A0
// adc values of each switch
#define UP_ADC      92
#define LEFT_ADC    317
#define DOWN_ADC    694
#define RIGHT_ADC   997
#define CENTER_ADC  930
#define TRIGGER_ADC 1023


volatile uint8_t CURRENTMODE = 0;
volatile uint8_t LCDMODE = 0;
volatile uint8_t CURRENTMODE = 0;



void setup(){
    Serial.begin(9600);
    while(!Serial);
}

void loop(){
    uint16_t adcValue = analogRead(SWITCHS);
    if(adcValue >= 8 ){
        delay(500);
        displayCycler(adcValue);
    }
}

void displayCycler(uint16_t adcValue){
    if(adcValue > TRIGGER_ADC - 3 ){
        Serial.println("Fire!")
    }else if ((adcValue >= UP_ADC - ERR) && (adcValue <= UP_ADC + ERR)){
        Serial.println("Volume Up");
    }else if ((adcValue >= UP_ADC - ERR) && (adcValue <= UP_ADC + ERR)){
        Serial.println("Volume Down");
    }else if ((adcValue >= LEFT_ADC - ERR) && (adcValue <= LEFT_ADC + ERR)){
        (LCDMODE == 0) ? (LCDMODE = 3) : (LCDMODE--);
        Serial.println(LCDMODE);
    }else if ((adcValue >= RIGHT_ADC - ERR) && (adcValue <= RIGHT_ADC + ERR)){
        (LCDMODE == 3) ? (LCDMODE = 0) : (LCDMODE++);
        Serial.println(LCDMODE);
    }else if ((adcValue >= CENTER_ADC - ERR) && (adcValue <= CENTER_ADC + ERR)){
        CURRENTMODE = LCDMODE;
        Serial.println("Mode Set")
        Serial.println(CURRENTMODE);
    }
}
