/**********************************************************************
* FILENAME:  ModeAndVolumeControl.ino
* AUTHOR:    reidrumack@gmail.com <Reid Rumack>
* DATE:      23 June 2016
* Arduino:   Mini Pro
* PRODUCT:   LCD 1602A
* VERSION:   1.0
**********************************************************************/
#include <SoftwareSerial.h>
#include <LiquidCrystal.h>

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
volatile uint8_t VOLUME = 7;
static  char* modeString[]= {"   Semi  Auto", "   Full  Auto", "Song 1", "Song 2"};    
LiquidCrystal lcd(8, 9, 10, 11, 12, 13);


void setup(){
    Serial.begin(9600);
    lcd.begin(16, 2);
    while(!Serial);
    lcd.clear();
    for(uint8_t i = 0; i < VOLUME;i++){
        lcd.setCursor(i,1);
        lcd.write(0xFF);
    }
    lcd.setCursor(0,0);
    lcd.print(modeString[CURRENTMODE]);
}

void loop(){
    uint16_t adcValue = analogRead(SWITCHS);
    if(adcValue >= 8 ){
        displayCycler(adcValue);
    }
}

void displayCycler(uint16_t adcValue){
    if(adcValue > TRIGGER_ADC - 3 ){
        if(LCDMODE != CURRENTMODE){
            LCDMODE = CURRENTMODE;
            updateDisplay(0);
        }
        delay(250);
    }else if ((adcValue >= UP_ADC - ERR) && (adcValue <= UP_ADC + ERR)){
        if(VOLUME < 16){
            VOLUME++;
        }
        updateDisplay(1);
        Serial.println("Volume Up");
        delay(500);
    }else if ((adcValue >= DOWN_ADC - ERR) && (adcValue <= DOWN_ADC + ERR)){
        if(VOLUME > 0){
            VOLUME--;
        }
        updateDisplay(2);
        Serial.println("Volume Down");
        delay(500);
    }else if ((adcValue >= LEFT_ADC - ERR) && (adcValue <= LEFT_ADC + ERR)){
        (LCDMODE == 0) ? (LCDMODE = 3) : (LCDMODE--);
        updateDisplay(3);
        Serial.println(LCDMODE);
        delay(500);
    }else if ((adcValue >= RIGHT_ADC - ERR) && (adcValue <= RIGHT_ADC + ERR)){
        (LCDMODE == 3) ? (LCDMODE = 0) : (LCDMODE++);
        updateDisplay(3);
        Serial.println(LCDMODE);
        delay(500);
    }else if ((adcValue >= CENTER_ADC - ERR) && (adcValue <= CENTER_ADC + ERR)){
      // mayby put in "mode set"  
        CURRENTMODE = LCDMODE;
        Serial.println("Mode Set");
        delay(500);
    }
}

void updateDisplay(uint8_t switchID){
    switch(switchID){
      // trigger was pulled 
      case 0:
        clearLcdTopLine();   
        lcd.setCursor(0,0);
        lcd.print(modeString[CURRENTMODE]);
        break;
      // volume up
      case 1:
        lcd.setCursor(VOLUME - 1,1);
        lcd.write(0xFF);
        break;
      // volume down
      case 2:
        lcd.setCursor(VOLUME,1);
        lcd.write(' ');
        break;
      case 3:
        clearLcdTopLine();
        lcd.setCursor(0,0);
        lcd.print(modeString[LCDMODE]);
        break;                  
    }
}

void clearLcdTopLine(){
    lcd.setCursor(0,0);
    lcd.print("                ");
}

