//LCD1602
//You should now see your LCD1602 display the flowing characters "SUNFOUNDER" and "hello, world"
//Email:support@sunfounder.com
//Website:www.sunfounder.com
//2015.5.7
#include <LiquidCrystal.h>// include the library code 
/**********************************************************/
char array1[]="hello, world!";
int tim = 250;  //the value of delay time
uint8_t displayMode = 0;
// initialize the library with the numbers of the interface pins
LiquidCrystal lcd(8, 9, 10, 11, 12, 13); // was 4, 6, 10, 11, 12, 13
 

void setup()
{
  pinMode(3, INPUT);
  lcd.begin(16, 2);
}



void loop() {
  if(digitalRead(3)== HIGH){
    modeCycle();
  }
}
void printDisp(){
  
}

void modeCycle(){
  if(displayMode == 2){
    displayMode = 0:
  }else{
    displayMode++;
  }
}
/************************************************************/
