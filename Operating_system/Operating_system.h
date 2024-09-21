#pragma once
#include <Arduino.h>

template < uint16_t _AMOUNT >
class Operating_system {
public:
  Operating_system() {
    for (uint16_t i = 0; i < _AMOUNT; i++){
      states[i] = 1;
    }
  }

  void tick() {
    for (uint8_t i = 0; i < _AMOUNT; i++) {
      if (tasks[i] && millis() - timers[i] >= periods[i] && states[i]) {
        tasks[i]();

        timers[i] = millis();
      }
    }
  }

  void set_period(uint16_t num, uint32_t period) {
    periods[num] = period;
  }

  void attach(uint16_t num, void (*function)(), uint32_t period = 1000) {
    tasks[num] = function;
    periods[num] = period;
  }

  void detach(uint16_t num) {
    tasks[num] = NULL;
  }

  void start(uint16_t num){
    states[num] = 1;
  }

  void stop(uint16_t num){
    states[num] = 0;
  }

  void exec(uint16_t num){
    tasks[num]();
  }

  uint32_t get_left(){
    uint32_t min_time = millis() - timers[0];
    for (uint16_t i = 0; i < _AMOUNT; i++){
      if (millis() - timers[i] < min_time){
        min_time = millis() - timers[i];
      }
    }
    return min_time;
  }

private:
  uint32_t timers[_AMOUNT], periods[_AMOUNT];
  void (*tasks[_AMOUNT])() = {};
  bool states[_AMOUNT];
};