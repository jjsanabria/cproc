
// A SIMPLE CLOCK: An 'n' millisecond clock that prints millis() 
// every 'm' tics.
// The purpose is to encapsulate most of the AVR code (ugly stuff) in a class
// and make sketches a bit more readable.
// For more info on timers and interrupts, 
// please check out this great link (and inspiration for this class):
// http://www.engblaze.com/microcontroller-tutorial-avr-and-arduino-timer-interrupts/
// NOTE: Our needs don't need great time accuracy. This clock is NOT very accurate.

#include "jTimer.h"
#define TIC_LENGTH  5 // a 5 ms ticking clock 

jTimer ticker(1, TIC_LENGTH); // @params: use avr hardware timer #1, and set a 'tic_length' timeout,
volatile bool tic = false; // make sure to make volatile

void setup() {
  Serial.begin(9600);
  ticker.start();
} 

uint32_t counter = 0; 

void loop(){
  if(tic){
    tic=false;
    counter++;
    if(counter==200){
      counter = 0;
      //print every 200 tics, hence every 200 ms
      Serial.println( millis() );
      }
  }  
}




ISR(TIMER1_COMPA_vect){
    tic=true; //  5ms tic
}


