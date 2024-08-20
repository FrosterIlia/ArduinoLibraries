#pragma once
#include <Arduino.h>

class EasyButton {
  public:
    EasyButton(byte pin) {
      _mainTimer = millis();
      _clickFlag = false;
      _holdFlag = false;
      _holdTimer = millis();
      _pin = pin;
      _clickDebounce = 10;
      _holdTimeout = 1000;
      pinMode(_pin, INPUT_PULLUP);
    }

    bool tick() {
      if (!digitalRead(_pin) && _clickFlag == false) {
        _clickFlag = true;
        _mainTimer = millis();
      }
      if (digitalRead(_pin) && _clickFlag == true && millis() - _mainTimer >= _clickDebounce) {
        _clickFlag = false;
      }
      if (!digitalRead(_pin) && _holdFlag == false){
        _holdTimer = millis();
        _holdFlag = true;
      }
      if (digitalRead(_pin) && _holdFlag == true) {
        _holdFlag = false;
      }
    }

    bool isClick() {
      if (digitalRead(_pin) && _clickFlag == true && millis() - _mainTimer >= _clickDebounce && millis() - _mainTimer <= _holdTimeout) {
        _clickFlag = false;
        return true;
      }
      return false;
    }
    bool get_state() {
      return !digitalRead(_pin);
    }

    bool isHolded() {

      if (!digitalRead(_pin) && _holdFlag == true && millis() - _holdTimer >= _holdTimeout) {
        _holdTimer = millis();
        _holdFlag = false;
        return true;
      }
      return false;
    }
    bool isHold(){
      if (_clickFlag == true && millis() - _holdTimer >= _holdTimeout){
        return true;
      }
      return false;
    }
    bool set_pin(byte pin){
      _pin = pin;
    }
    byte get_pin(){
      return _pin;
    }
  private:
    uint32_t _mainTimer;
    uint32_t _holdTimer;
    bool _clickFlag;
    bool _holdFlag;
    byte _pin;
    uint16_t _clickDebounce;
    uint16_t _holdTimeout;
};
