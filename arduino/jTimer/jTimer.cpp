
#include <avr/io.h>
#include <avr/interrupt.h>
#include "jTimer.h"
#include "Arduino.h"

jTimer::jTimer( uint8_t timerNumber, double intervalLength ){


    if(! _running ){
      #if defined(__AVR_ATmega1280__) || defined(__AVR_ATmega2560__) || defined(__AVR_ATmega32u4__)
       _isBetterBoard =  true;
      #endif

      //let's make sure we have a valid timer number
      if( timerNumber==1 || timerNumber==2 || ( timerNumber>2 && _isBetterBoard && timerNumber<6) ){
        _clockMegaHertz = 16; //both the arduino uno and the mega run at 16 MHz
        _timerNumber = timerNumber;
        _timerBits  = ( timerNumber ==  2 ) ? 8 : 16; //timers 1,3,4,5 are 16 bit. Timer 2 is 8 bit.
        _validated =  calculateParams( intervalLength );
      } else {
        error = "Invalid timer. UNO model has up to timer #2 and MEGA model up to timer #5";
        _validated =  false;
      }
    } else {
      error = "Timer is already running";
    }

} 
//ticker targetTime in milliseconds
bool jTimer::calculateParams( double targetInterval ){

  uint32_t clockSpeed = _clockMegaHertz * 1000;   //cycles per millisecond
  for(int n=0; n<_factorsCount; n++){
    //1. Make sure the target time is not too big for this given scaling factor
    double clockLoopTime = _clockScaleFactors[n] / float(clockSpeed); 
    double maxMeasurableInterval = pow(2, _timerBits ) * clockLoopTime;
     
    if(targetInterval < maxMeasurableInterval ){
       //calculate the amount of loops
       double loops = (targetInterval/clockLoopTime) - 1;
       _timerLoops = long(loops);
       _scaleFactor = _clockScaleFactors[n];
       if(n+1==_factorsCount){ 
          return true;
          }
      } else {
        if(n==0){ 
          error = "Timer length is too big";
          return false;
         } else {
          return true; 
         }
      }
  }
}


void jTimer::start(){

  if(_validated && !_running){

    cli();

    if( _timerNumber==1 ){


        TCCR1A = 0;     
        TCCR1B = 0;     
        OCR1A = _timerLoops;
        TCCR1B |= (1 << WGM12);
        if(_scaleFactor==256 || _scaleFactor==1024 ){ TCCR1B |= (1 << CS12); } else {  TCCR1B |= (0 << CS12); }
        if( _scaleFactor==8 || _scaleFactor==64 ){ TCCR1B |= (1 << CS11);  } else { TCCR1B |= (0 << CS11); }
        if( _scaleFactor==8 || _scaleFactor==256 ){ TCCR1B |= (0 << CS10); } else {  TCCR1B |= (1 << CS10); }
        TIMSK1 |= (1 << OCIE1A); 


    } else if( _timerNumber==2 ){


        TCCR2A = 0;     
        TCCR2B = 0;     
        OCR2A = _timerLoops;
        TCCR2B |= (1 << WGM12);
        if(_scaleFactor==256 || _scaleFactor==1024 ){ TCCR2B |= (1 << CS12); } else {  TCCR2B |= (0 << CS12); }
        if( _scaleFactor==8 || _scaleFactor==64 ){ TCCR2B |= (1 << CS11);  } else { TCCR2B |= (0 << CS11); }
        if( _scaleFactor==8 || _scaleFactor==256 ){ TCCR2B |= (0 << CS10); } else {  TCCR2B |= (1 << CS10); }
        TIMSK2 |= (1 << OCIE2A); 


    } else if( _timerNumber==3 ){
      #if defined(__AVR_ATmega1280__) || defined(__AVR_ATmega2560__)
        TCCR3A = 0;     // set entire TCCR1A register to 0
        TCCR3B = 0;     // same for TCCR1B
        OCR3A = _timerLoops;
        TCCR3B |= (1 << WGM12);
        if(_scaleFactor==256 || _scaleFactor==1024 ){ TCCR3B |= (1 << CS12); } else {  TCCR3B |= (0 << CS12); }
        if( _scaleFactor==8 || _scaleFactor==64 ){ TCCR3B |= (1 << CS11);  } else { TCCR3B |= (0 << CS11); }
        if( _scaleFactor==8 || _scaleFactor==256 ){ TCCR3B |= (0 << CS10); } else {  TCCR3B |= (1 << CS10); }
        TIMSK3 |= (1 << OCIE3A); 
      #endif
    } else if( _timerNumber==4 ){
      #if defined(__AVR_ATmega1280__) || defined(__AVR_ATmega2560__) 
        TCCR4A = 0;     // set entire TCCR1A register to 0
        TCCR4B = 0;     // same for TCCR1B
        OCR4A = _timerLoops;
        TCCR4B |= (1 << WGM12);
        if(_scaleFactor==256 || _scaleFactor==1024 ){ TCCR4B |= (1 << CS12); } else {  TCCR4B |= (0 << CS12); }
        if( _scaleFactor==8 || _scaleFactor==64 ){ TCCR4B |= (1 << CS11);  } else { TCCR4B |= (0 << CS11); }
        if( _scaleFactor==8 || _scaleFactor==256 ){ TCCR4B |= (0 << CS10); } else {  TCCR4B |= (1 << CS10); }
        TIMSK4 |= (1 << OCIE4A); 
      #endif
    } else if( _timerNumber==5 ){
      #if defined(__AVR_ATmega1280__) || defined(__AVR_ATmega2560__)
        TCCR5A = 0;     // set entire TCCR1A register to 0
        TCCR5B = 0;     // same for TCCR1B
        OCR5A = _timerLoops;
        TCCR5B |= (1 << WGM12);
        if(_scaleFactor==256 || _scaleFactor==1024 ){ TCCR5B |= (1 << CS12); } else {  TCCR5B |= (0 << CS12); }
        if( _scaleFactor==8 || _scaleFactor==64 ){ TCCR5B |= (1 << CS11);  } else { TCCR5B |= (0 << CS11); }
        if( _scaleFactor==8 || _scaleFactor==256 ){ TCCR5B |= (0 << CS10); } else {  TCCR5B |= (1 << CS10); }
        TIMSK5 |= (1 << OCIE5A); 
      #endif
    } 

    sei();
    _running =  true;
  } else {
    error = "Timer is either already running or did not validate.";
  }
}

void jTimer::clearTimer(){
    cli(); 
    switch(_timerNumber){

      case 1:
        TCCR1A = 0;     // set entire TCCR1A register to 0
        TCCR1B = 0;     // same for TCCR1B
      break;

      case 2:
        TCCR2A = 0;     // set entire TCCR1A register to 0
        TCCR2B = 0;     // same for TCCR1B
      break;

      case 3:
        #if defined(__AVR_ATmega1280__) || defined(__AVR_ATmega2560__)
        TCCR3A = 0;     // set entire TCCR1A register to 0
        TCCR3B = 0;     // same for TCCR1B
        #endif

      break;

      case 4:
        #if defined(__AVR_ATmega1280__) || defined(__AVR_ATmega2560__) || defined(__AVR_ATmega32u4__)
        TCCR4A = 0;     // set entire TCCR1A register to 0
        TCCR4B = 0;     // same for TCCR1B
        #endif
      break;

      case 5:
        #if defined(__AVR_ATmega1280__) || defined(__AVR_ATmega2560__)
        TCCR5A = 0;     // set entire TCCR1A register to 0
        TCCR5B = 0;     // same for TCCR1B
        #endif
      break;
    }
    sei();
    _running =  false;
}

 


void jTimer::printParams(){
  if(_validated){
    Serial.print("Timer Loops: ");
    Serial.println(_timerLoops);
    Serial.print("Clock Scale Factor: ");
    Serial.println(_scaleFactor);
  } else {
    Serial.println(error);
  }
}




