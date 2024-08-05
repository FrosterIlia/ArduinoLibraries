#pragma once
#include <Arduino.h>

#define NORMAL 1
#define REVERSE 0

class Pid {
public:
  float kp, ki, kd;  
  float setpoint, input;

  Pid() {
    kp = 0;
    ki = 0;
    kd = 0;
    _dt = 0;
  }


  Pid(float p = 0, float i = 0, float d = 0, float dt = 0) {
    kp = p;
    ki = i;
    kd = d;
    _dt = dt / 1000;
  }

  void set_direction(bool direction){
    _direction = direction;
  }

  void set_dt(uint16_t dt){
    _dt = dt / 1000;
  }

  float get_output(){
    return _output;
  }
  void set_limits(uint16_t min_out, uint16_t max_out){
    _min_out = min_out;
    _max_out = max_out;
  
  }

  void compute() {
    float error = setpoint - input;
    if (!_direction) error = -error;
    _I += error * _dt;
    _D = (error - _prev_error) / _dt;
    if (!_direction) _D = -_D;

    _output = constrain(error * kp + _I * ki + _D * kd, _min_out, _max_out);
    _prev_error = error;
  }


private:
  float _dt;
  float _prev_error;
  float _I, _D;
  float _output;
  bool _direction = 1;
  int16_t _max_out = 32767, _min_out = -32768;
};