/*********************************************************************
* FILENAME:  ModeAndVolumeControl.ino
* AUTHOR:    reidrumack@gmail.com <Reid Rumack>
* DATE:      23 June 2016
* Arduino:   Mini Pro
* PRODUCT:   LCD 1602A, 5 way nav switch and Game Switch (lever switch)
* VERSION:   1.31
**********************************************************************/
#include <SoftwareSerial.h>
#include <LiquidCrystal.h>
#include <Adafruit_Soundboard.h>
/********************************************************************
 * PIN Defineintions  
 */



// difines the pin that the switch are are on


// amount +/- ADC value alowed for error
#define ERR 25

#define SWITCHS A0

//pins for sound board 
#define SFX_ACT      A1
#define SFX_RST      7



// adc values of each switch
#define UP_ADC      838 
#define LEFT_ADC    765
#define DOWN_ADC    694
#define RIGHT_ADC   1023

#define CENTER_ADC  930
SoftwareSerial ss = SoftwareSerial(6, 5);
Adafruit_Soundboard sfx = Adafruit_Soundboard(&ss, NULL, SFX_RST);


LiquidCrystal lcd(8, 9, 10, 11, 12, 13);

volatile uint8_t CURRENTMODE = 0;
volatile uint8_t LCDMODE = 0;
volatile uint8_t VOLUME = 16;
uint8_t NUM_VOLUME = 204;

static  char* modeString[]= {"   Semi  Auto", "   Full  Auto", "  Light It Up", "    Lean On"};
static  char* modeTrack[] = {"lazer"};


void setup(){
    Serial.begin(115200);
    ss.begin(9600);
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
        if(analogRead(A2) > 512){
            trigger();
        }
        btnHandler(analogRead(SWITCHS));
}

void btnHandler(uint16_t adcValue){
  if(adcValue >= 30 ){
    if ((adcValue >= UP_ADC - ERR) && (adcValue <= UP_ADC + ERR)){
        volumeUp(); 
    }else if ((adcValue >= DOWN_ADC - ERR) && (adcValue <= DOWN_ADC + ERR)){
        volumeDown();
    }else if ((adcValue >= LEFT_ADC - ERR) && (adcValue <= LEFT_ADC + ERR)){
        cycleLeft();
    }else if ((adcValue >= RIGHT_ADC - ERR) && (adcValue <= RIGHT_ADC + ERR)){
        cycleRight();
    }else if ((adcValue >= CENTER_ADC - ERR) && (adcValue <= CENTER_ADC + ERR)){
        selectMode();
    }
  }
}

void trigger(){
    triggerHandler();
    if(LCDMODE != CURRENTMODE){
        LCDMODE = CURRENTMODE;
        clearLcdTopLine();   
        lcd.print(modeString[CURRENTMODE]);
    }
}

void volumeUp(){
    if(VOLUME < 16){
        volumeUpRecursive();
        volumeUpRecursive();
        VOLUME++;
        lcd.setCursor(VOLUME - 1,1);
        lcd.write(0xFF);
    }
    delay(500);
}
void volumeUpRecursive(){
    uint16_t vol;
      if (! (vol=sfx.volUp()) ) {
        delay(50);
        volumeUpRecursive();
    } else { 
        Serial.print("Volume: "); 
        Serial.println(vol);
    }
}


void volumeDown(){
    if(VOLUME > 0){
        volumeDownRecursive();
        volumeDownRecursive();
        VOLUME--;
        lcd.setCursor(VOLUME,1);
        lcd.write(' ');
    }
    delay(500);
}

void volumeDownRecursive(){
    uint16_t vol;
      if (! (vol=sfx.volDown()) ) {
        delay(50);
        volumeDownRecursive();
    } else { 
        Serial.print("Volume: "); 
        Serial.println(vol);
    }
}

void cycleRight(){
    (LCDMODE == 3) ? (LCDMODE = 0) : (LCDMODE++);
    clearLcdTopLine();
    lcd.setCursor(0,0);
    lcd.print(modeString[LCDMODE]);
    delay(500);
}

void cycleLeft(){
    (LCDMODE == 0) ? (LCDMODE = 3) : (LCDMODE--);
    clearLcdTopLine();
    lcd.setCursor(0,0);
    lcd.print(modeString[LCDMODE]);
    delay(500);
}

void semiAuto(){
     uint8_t toPlay = 0;
     sfx.playTrack(toPlay);
     while(analogRead(A1)<= 512);
     while(analogRead(A2)> 512);
}
void fullAuto(){
     uint8_t toPlay = 0;
     sfx.playTrack(toPlay);
     while(analogRead(A1)<= 512);
}
void triggerHandler(){
    switch(CURRENTMODE){
        case 0:
            semiAuto();
        break;
        case 1:
            fullAuto();  
        break;
    }
}
void selectMode(){
    CURRENTMODE = LCDMODE;
}


void clearLcdTopLine(){
    lcd.setCursor(0,0);
    lcd.print("                ");
    lcd.setCursor(0,0);
}

