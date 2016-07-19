/*********************************************************************
* FILENAME:  ModeAndVolumeControl.ino
* AUTHOR:    reidrumack@gmail.com <Reid Rumack>
* DATE:      23 June 2016
* Arduino:   Mini Pro
* PRODUCT:   LCD 1602A, 5 way nav switch and Game Switch (lever switch)
* VERSION:   1.3
**********************************************************************/
#include <SoftwareSerial.h>
#include <LiquidCrystal.h>
#include <Adafruit_NeoPixel.h>
#include <Adafruit_Soundboard.h>


// difines the pin that the switch are are on


// amount +/- ADC value alowed for error
#define ERR 25

#define SWITCHS A0

//pins for sound board 
#define TRIGGER      A2
#define SFX_ACT      A1
#define SFX_RST      7
#define LASER_LED    3
#define NEO_PIXLES   2



// adc values of each switch
#define UP_ADC      838 
#define LEFT_ADC    765
#define DOWN_ADC    694
#define RIGHT_ADC   1023
#define NUM_PIXELS  24
#define CENTER_ADC  930
SoftwareSerial ss = SoftwareSerial(6, 5);
Adafruit_Soundboard sfx = Adafruit_Soundboard(&ss, NULL, SFX_RST);
Adafruit_NeoPixel strip = Adafruit_NeoPixel(NUM_PIXELS, NEO_PIXLES, NEO_GRB + NEO_KHZ800);


LiquidCrystal lcd(8, 9, 10, 11, 12, 13);

volatile uint8_t CURRENTMODE = 0;
volatile uint8_t LCDMODE = 0;
volatile uint8_t VOLUME = 16;
volatile boolean trackTriggered = false;
volatile boolean trackPlaying = false;  

uint8_t          NUM_VOLUME = 204;
const uint32_t   GREEN = strip.Color(0, 255, 0);
const uint32_t   RED =strip .Color(255, 0, 0);
static  uint32_t   NEO_COLOR;
static  char* modeString[]= {"   Semi  Auto", "   Full  Auto", "  Light It Up", "    Lean On"};
static  char* modeTrack[] = {"lazer"};


void setup(){

    NEO_COLOR = GREEN;
  
    Serial.begin(115200);
    ss.begin(9600);
    lcd.begin(16, 2);
    pinMode(LASER_LED, OUTPUT);
    strip.begin();
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

        if(analogRead(TRIGGER) > 512){
            trigger();
        }else{
            btnHandler(analogRead(SWITCHS));
       }
}

void btnHandler(uint16_t adcValue){
  if(adcValue >= 100 ){
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
    if((analogRead(SFX_ACT) > 512) &&  (VOLUME < 16)){
        volumeUpRecursive();
        volumeUpRecursive();
        VOLUME++;
        lcd.setCursor(VOLUME - 1,1);
        lcd.write(0xFF);
        delay(450);
    }
}
void volumeUpRecursive(){
    uint16_t vol;
      if (! (vol = sfx.volUp()) ) {
        delay(10);
        volumeUpRecursive();
    }
}


void volumeDown(){
    if((analogRead(SFX_ACT) > 512) && (VOLUME > 0)){
        Serial.println("in!");
        volumeDownRecursive();
        volumeDownRecursive();
        VOLUME--;
        lcd.setCursor(VOLUME,1);
        lcd.write(' ');
        delay(450);
    }
}

void volumeDownRecursive(){
    uint16_t vol;
      if(! (vol=sfx.volDown())) {
        delay(10);
        volumeDownRecursive();
    }
}
void playPause() {
  if (trackPlaying) {
    pauseRecursive();
    trackPlaying = false;
  } else {
    playRecursive();
    trackPlaying = true;
  }
  delay(500);
}

void pauseRecursive() {
  if (trackPlaying && ! sfx.pause()) {
    delay(10);
    pauseRecursive();
  }
}

void playRecursive() {
  if (!trackPlaying && ! sfx.unpause()) {
    delay(10);
    playRecursive();
  }
}

void stopAudio(){
     if ((analogRead(SFX_ACT) < 512) && (!sfx.stop())){
         stopRecursive();  
    }
}
void stopRecursive(){
    if ((analogRead(SFX_ACT) < 512) && (!sfx.stop())) {
        delay(10);
        stopRecursive();
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
     pulseLights(NEO_COLOR);
     fireLaserLed();
     while(analogRead(SFX_ACT) <= 512);
     while(analogRead(A2)> 512);
}
void fullAuto(){
     uint8_t toPlay = 0;
     sfx.playTrack(toPlay);
     pulseLights(NEO_COLOR);
     fireLaserLed();
     while(analogRead(SFX_ACT)<= 512);
}
void triggerHandler(){
    switch(CURRENTMODE){
        case 0:
            semiAuto();
        break;
        case 1:
            fullAuto();  
        break;
        
        case 2:
            if(analogRead(SFX_ACT) > 512){
                sfx.playTrack(1);
                trackPlaying = true;
                trackTriggered = true; 
            } 
            delay(500);
            playPause();
          break;
        case 3:
            if(analogRead(SFX_ACT) > 512){
                sfx.playTrack(2);
                trackPlaying = true;
                trackTriggered = true; 
            } 
            delay(500);
            playPause();
        break;
    }
}

    

void selectMode(){
    CURRENTMODE = LCDMODE;
    stopAudio();
}

void clearLcdTopLine(){
    lcd.setCursor(0,0);
    lcd.print("                ");
    lcd.setCursor(0,0);
}


void fireLaserLed(){
    digitalWrite(LASER_LED, HIGH);   // turn the LED on (HIGH is the voltage level)
    delay(100);
    digitalWrite(LASER_LED, LOW);    // turn the LED off by making the voltage LOW
}

void pulseLights(uint32_t c){   
    for (int i = 0; i <= NUM_PIXELS; i++) { 
        strip.setPixelColor(i, c);
        if(i!=0){
          strip.setPixelColor(i-1, 0); 
        }
        strip.show();
        delay(10);
    }
}
