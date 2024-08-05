#pragma once
#include <Arduino.h>

class Timer{
  public:
    Timer(uint16_t period = 1000){
      _period = period;
      _timer = millis();
    }
  bool isReady(){
    if (millis() - _timer >= _period){
      _timer = millis();
      return true;
    }
    return false;
  }
  void setPeriod(uint16_t newPeriod){
    _period = newPeriod;
  }
  private:
    uint16_t _period;
    uint32_t _timer;
};