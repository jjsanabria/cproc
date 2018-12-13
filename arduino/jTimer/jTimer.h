/*
  Timer.h - Library to manage time and timers
*/
#ifndef jTimer_h
#define jTimer_h
#include "Arduino.h"

class jTimer
{
  public:
    jTimer(uint8_t timerNumber, double intervalLength );
    String error = "Unknown Error";
    void start(void);
    void clearTimer(void);
    void printParams(void);
    //uint16_t tick();
    //void clearTicks();

  private:
    uint16_t _ticks = 0;
    long _timerLoops;
    int16_t _scaleFactor;
    bool _isBetterBoard = false;
    bool _validated = false;
    bool _running=false;
    uint8_t  _clockMegaHertz;
    uint8_t _timerBits;
    uint8_t _timerNumber;
    uint16_t _clockScaleFactors[5] = { 1024, 256, 64, 8, 1 };
    uint8_t _factorsCount = 5;
    bool calculateParams( double intervalLength );
};

#endif