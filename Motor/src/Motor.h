#pragma once
#include <Arduino.h>

// Modes
#define FORWARD 1
#define BACKWARDS 2
#define BRAKE 3
#define STOP 4
#define AUTO 0

// Direction
#define REVERSE 0
#define NORMAL 1


// Class for controlling a motor with a 3-wire driver
class Motor{
    public:

    Motor(uint8_t ain1_pin, uint8_t ain2_pin, uint8_t pwm_pin){

        _ain1_pin = ain1_pin;
        _ain2_pin = ain2_pin;
        _pwm_pin = pwm_pin;
        _mode = FORWARD;
        _resolution = 8;
        _filterCoeff = 235;

        pinMode(_ain1_pin, OUTPUT);
        pinMode(_ain2_pin, OUTPUT);
        pinMode(_pwm_pin, OUTPUT);
        
        init();
    }

    Motor(uint8_t ain1_pin, uint8_t ain2_pin, uint8_t pwm_pin, uint8_t mode){

        _ain1_pin = ain1_pin;
        _ain2_pin = ain2_pin;
        _pwm_pin = pwm_pin;
        _mode = mode;
        _resolution = 8;
        _filterCoeff = 235;

        pinMode(_ain1_pin, OUTPUT);
        pinMode(_ain2_pin, OUTPUT);
        pinMode(_pwm_pin, OUTPUT);
        
        init();
    }

    void setSpeed(int16_t speed){
        if (speed > 0){
            _speed = constrain(map(speed, 0, intPow(2, _resolution) - 1, _minSignal, intPow(2, _resolution) - 1), _minSignal, intPow(2, _resolution) - 1);
        }
        else if (speed == 0) _speed = 0;
        else { // if speed is negative (for auto mode)
            _speed = constrain(map(-speed, 0, intPow(2, _resolution) - 1, _minSignal, intPow(2, _resolution) - 1), _minSignal, intPow(2, _resolution) - 1) * -1;
        }

        switch (_mode){
            case FORWARD:
                digitalWrite(_ain1_pin, 1);
                digitalWrite(_ain2_pin, 0);
                analogWrite(_pwm_pin, _speed);
            break;


            case BACKWARDS:
                digitalWrite(_ain1_pin, 0);
                digitalWrite(_ain2_pin, 1);
                analogWrite(_pwm_pin, _speed);
                break;
            
            case BRAKE:
                digitalWrite(_ain1_pin, 1);
                digitalWrite(_ain2_pin, 1);
                analogWrite(_pwm_pin, _speed);
                break;

            case STOP:
                digitalWrite(_ain1_pin, 0);
                digitalWrite(_ain2_pin, 0);
                analogWrite(_pwm_pin, 0);
                break;

            case AUTO:
                if (_direction == REVERSE) _speed = -_speed;
                if (_speed < 0){ // go backward
                    digitalWrite(_ain1_pin, 0);
                    digitalWrite(_ain2_pin, 1);
                    analogWrite(_pwm_pin, -_speed);
                }
                else if (_speed == 0){ // stop
                    digitalWrite(_ain1_pin, 0);
                    digitalWrite(_ain2_pin, 0);
                    analogWrite(_pwm_pin, 0);
                } 
                else{ // go forward
                    digitalWrite(_ain1_pin, 1);
                    digitalWrite(_ain2_pin, 0);
                    analogWrite(_pwm_pin, _speed);   
                }
                break;
        }
    }

    void setMode(uint8_t mode){
        _mode = mode;
        if (_direction == REVERSE){
            if (_mode == FORWARD) _mode = BACKWARDS;
            else if (_mode == BACKWARDS) _mode = FORWARD;
        }
    }

    void setDirection(bool direction){
        _direction = direction;
    }

    void setResolution(uint8_t resolution){
        _resolution = resolution;
    }

    void setMinSignal(uint16_t minSignal){
        _minSignal = minSignal;
    }

    void tick(){
        if (_smoothMode){
            _filteredSpeed += (_desSpeed - _filteredSpeed) / (256 - _filterCoeff);
            setSpeed(_filteredSpeed);
        }
        
    }

    void setSmooth(bool value){
        _smoothMode = value;
    }

    void setSmoothSpeed(int16_t speed){
        _desSpeed = speed;
    }

    bool getCurrentDirection(){
        if (_mode == AUTO){
            if (_speed >= 0) return false;
            return true;
        }
        else if (_mode == FORWARD) return true;
        else if (_mode == BACKWARDS) return false;

        else return true;
    }

    private:

    int16_t _speed;
    uint8_t _ain1_pin, _ain2_pin, _pwm_pin;
    uint8_t _mode;
    uint8_t _direction;
    uint8_t _resolution;
    uint16_t _minSignal;
    float _filteredSpeed;
    uint8_t _filterCoeff;
    int16_t _desSpeed;
    bool _smoothMode;

    int16_t intPow(int16_t value, int16_t power){
        int16_t result = value;
        for (uint16_t i = 1; i < power; i++){
            result *= value;
        }
        return result;
    }
};
