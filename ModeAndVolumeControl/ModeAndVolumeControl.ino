//V1
/**********************************************************************
* FILENAME:  ModeAndVolumeControl.ino
* AUTHOR:    reidrumack@gmail.com <Reid Rumack>
* DATE:      23 June 2016
* Arduino:   Mini Pro
* PRODUCT:   LCD 1602A, 5 way nav switch and Game Switch (lever switch)
* VERSION:   1.1
**********************************************************************/
#include <SoftwareSerial.h>
#include <LiquidCrystal.h>
#include <Adafruit_Soundboard.h>
/********************************************************************
 * PIN Defineintions  
 */



// difines the pin that the switch are are on


// amount +/- ADC value alowed for error
#define ERR 30

#define SWITCHS A0

//pins for sound board 
#define SFX_TX 5
#define SFX_RX 6
#define SFX_RST 7


// adc values of each switch
#define UP_ADC      838 
#define LEFT_ADC    765
#define DOWN_ADC    694
#define RIGHT_ADC   997
#define CENTER_ADC  930
#define TRIGGER_ADC 1023


LiquidCrystal lcd(8, 9, 10, 11, 12, 13);
SoftwareSerial ss = SoftwareSerial(SFX_TX, SFX_RX);
Adafruit_Soundboard sfx = Adafruit_Soundboard(&ss, NULL, SFX_RST);

volatile uint8_t CURRENTMODE = 0;
volatile uint8_t LCDMODE = 0;
volatile uint8_t VOLUME = 15;

static  char* modeString[]= {"   Semi  Auto", "   Full  Auto", "  Light It Up", "    Lean On"};


void setup(){
    pinMode(2, INPUT);
    Serial.begin(9600);
    Serial.print("pwr");
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
    uint16_t adcValue = analogRead(SWITCHS);
    if(adcValue >= 30 ){
        btnHandler(adcValue);
    }
}

void btnHandler(uint16_t adcValue){
    if(adcValue > TRIGGER_ADC - 10 ){
        trigger();
    }else if ((adcValue >= UP_ADC - ERR) && (adcValue <= UP_ADC + ERR)){
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

void trigger(){
    uint8_t toPlay = 0;
    sfx.rest();
    sfx.playTrack(toPlay);
    if(LCDMODE != CURRENTMODE){
        LCDMODE = CURRENTMODE;
        clearLcdTopLine();   
        lcd.setCursor(0,0);
        lcd.print(modeString[CURRENTMODE]);
    }
    delay(250);
}

void volumeUp(){
    if(VOLUME < 16){
        VOLUME++;
        for(uint8_t i; i<6; i++){
          sfx.volUp();
        }
    }
    lcd.setCursor(VOLUME - 1,1);
    lcd.write(0xFF);
    delay(500);
}

void volumeDown(){
    if(VOLUME > 0){
        VOLUME--;
        for(uint8_t i; i<6; i++){
            sfx.volDown();
        }
    }
    lcd.setCursor(VOLUME,1);
    lcd.write(' ');
    delay(500);
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

void selectMode(){
    CURRENTMODE = LCDMODE;
}


void clearLcdTopLine(){
    lcd.setCursor(0,0);
    lcd.print("                ");
}

