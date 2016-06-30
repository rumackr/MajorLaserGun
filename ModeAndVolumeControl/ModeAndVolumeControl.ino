
/**********************************************************************
* FILENAME:  ModeAndVolumeControl.ino
* AUTHOR:    reidrumack@gmail.com <Reid Rumack>
* DATE:      28 June 2016
* Arduino:   Mini Pro
* PRODUCT:   LCD 1602A, 5 way nav switch and Game Switch (lever switch)
* VERSION:   2.1
**********************************************************************/
#include <SoftwareSerial.h>
#include <LiquidCrystal.h>
#include <Adafruit_Soundboard.h>
#include <avr/sleep.h>
#include <avr/interrupt.h>
/********************************************************************
 * PIN Defineintions
 */

// difines the pin that the switch are are on


// amount +/- ADC value alowed for error
#define ERR 30
#define IRQ 2

#define SWITCHS A0

//Reset pins for sound board
#define SFX_RST 7


// adc values of each switch
#define UP_ADC      838
#define LEFT_ADC    765
#define DOWN_ADC    694
#define RIGHT_ADC   997
#define CENTER_ADC  930
#define TRIGGER_ADC 1023


LiquidCrystal lcd(8, 9, 10, 11, 12, 13);
Adafruit_Soundboard sfx = Adafruit_Soundboard(&Serial, NULL, SFX_RST);

volatile unsigned long last_interrupt_time = 0;
volatile uint8_t CURRENTMODE = 0;
volatile uint8_t LCDMODE = 0;
volatile uint8_t VOLUME = 15;

static  char* modeString[]= {"   Semi  Auto", "   Full  Auto", "  Light It Up", "    Lean On"};

void setup(){
    pinMode(2, INPUT);
    Serial.begin(9600);
    lcd.begin(16, 2);
    lcd.clear();
    for(uint8_t i = 0; i < VOLUME;i++){
        lcd.setCursor(i,1);
        lcd.write(0xFF);
    }
    lcd.setCursor(0,0);
    lcd.print(modeString[CURRENTMODE]);
}

void loop(){
    delay(100);
    void audioOff(void) {
    sleepNow();
    }
}

/********************************* Actions ************************************/
void buttonIsr(){
    detachInterrupt(0);
    audioOn();
    uint16_t adcValue = analogRead(SWITCHS);
    unsigned long interrupt_time = millis();
    if((interrupt_time - last_interrupt_time) > 200 ){
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

          //  unsigned long startTime  = millis();
           // while((millis()- startTime) < 250);
   }
   last_interrupt_time = interrupt_time;
   playLazer();
   audioOff();
   sleepNow();
}

void trigger(){
    uint8_t toPlay = 0;
    digitalWrite(13,LOW);   // turn LED off to indicate sleep
    if(LCDMODE != CURRENTMODE){
        LCDMODE = CURRENTMODE;
        clearLcdTopLine();
        lcd.setCursor(0,0);
        lcd.print(modeString[CURRENTMODE]);
    }

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
}

void cycleRight(){
    (LCDMODE == 3) ? (LCDMODE = 0) : (LCDMODE++);
    clearLcdTopLine();
    lcd.setCursor(0,0);
    lcd.print(modeString[LCDMODE]);
}

void cycleLeft(){
    (LCDMODE == 0) ? (LCDMODE = 3) : (LCDMODE--);
    clearLcdTopLine();
    lcd.setCursor(0,0);
    lcd.print(modeString[LCDMODE]);
}

void selectMode(){
    CURRENTMODE = LCDMODE;
}


void clearLcdTopLine(){
    lcd.setCursor(0,0);
    lcd.print("                ");
}





void audioOn(void) {
  pinMode(SFX_RST , INPUT_PULLUP);
  
}

void audioOff(void) {
  digitalWrite(SFX_RST , LOW); // Hold low = XRESET (low power)
  pinMode(     SFX_RST , OUTPUT);
}

void playLazer(){
      sfx.playTrack(0);
      delay(250);
      while(analogRead(A1)<512);
}
/****************************** POWER-SAVING **********************************/
void sleepNow(){
    //enables pin 2s ability to trigger an interrupt
    attachInterrupt(0, buttonIsr, HIGH);
    delay(100);
    
    // Choose our preferred sleep mode:
    set_sleep_mode(SLEEP_MODE_IDLE);
 
    // Set sleep enable (SE) bit:
    sleep_enable();
 
    // Put the device to sleep:
    sleep_mode();
 
    // Upon waking up, sketch continues from this point.
    sleep_disable();
    
}

