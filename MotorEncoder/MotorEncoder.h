#pragma once
#include <Arduino.h>

#define HOLES_NUMBER 20

#define AVERAGING_BUFFER 5


class MotorEncoder{
    public:

    MotorEncoder(uint8_t pin){
        init();
        _pin = pin;
        _period = 100;
        _prevTime = millis();
        pinMode(_pin, INPUT);
        
    }

    void attach(void (*function)()){
        attachInterrupt(_pin, function, CHANGE);
    }

    uint8_t getPin(){
        return _pin;
    }

    bool tick(){

        if ( abs(_tickCounter) >= 2 && _timerOn){
            _speed = median(midArifm(((float)_tickCounter / ((float)millis() - (float)_prevTime) * 10000.0)));
            _tickCounter = 0;
            _prevTime = millis();
            return true;
        }
        if (!_timerOn) _speed = 0;

        if (millis() - _prevTime >= _period) _timerOn = false;
        return false;
    }

    void incTick(){
        _tickCounter++;
    }
    
    void decTick(){
        _tickCounter--;
    }

    void setTimerOn(){
        _timerOn = true;
    }

    float getSpeed(){
        return _speed;
    }

    private:

    float median(float newVal) {
        _medianBuf[_medianCount] = newVal;
        if (++_medianCount >= 3) _medianCount = 0;
        return (max(_medianBuf[0], _medianBuf[1]) == max(_medianBuf[1], _medianBuf[2])) ? max(_medianBuf[0], _medianBuf[2]) : max(_medianBuf[1], min(_medianBuf[0], _medianBuf[2]));
    }

    float midArifm(float newVal) {

        _sum += newVal; 
        _counter++;     
        if (_counter == AVERAGING_BUFFER) {    
            _prevResult = _sum / AVERAGING_BUFFER; 
            _sum = 0; 
            _counter = 0; 
        }
        return _prevResult;
    }

    uint8_t _pin;
    volatile int16_t _tickCounter;
    float _speed;
    uint16_t _period;
    uint32_t _prevTime;
    byte _counter = 0;
    float _prevResult = 0;
    float _sum = 0;
    float _medianBuf[3];
    byte _medianCount;
    bool _timerOn = true;
    
};
