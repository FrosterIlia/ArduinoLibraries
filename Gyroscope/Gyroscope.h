#pragma once
#include <Arduino.h>
#include "I2Cdev.h"
#include "MPU6050.h"

class Gyroscope {
public:
  Gyroscope() {
    init();

  }

  void begin() {

    //initialize mpu 
    mpu.initialize();
    mpu.setFullScaleGyroRange(MPU6050_GYRO_FS_250);

    calibrate();

    _gyro_angle_y = get_angle_accel();
    timer = millis();

    _filterCoeff = 0.95;
  }

  void calibrate() {
  //   Serial.println(mpu.getXAccelOffset());
  //   Serial.println(mpu.getYAccelOffset());
  //   Serial.println(mpu.getZAccelOffset());
  //   Serial.println(mpu.getXGyroOffset());
  //   Serial.println(mpu.getYGyroOffset());
  //   Serial.println(mpu.getZGyroOffset());

  mpu.CalibrateGyro(6);
  mpu.CalibrateAccel(6);

    // mpu.setXAccelOffset(-4650);
    // mpu.setYAccelOffset(1095);
    // mpu.setZAccelOffset(1636);
    // mpu.setXGyroOffset(88);
    // mpu.setYGyroOffset(25);
    // mpu.setZGyroOffset(-6);
  }

  float get_angle() {
    
    return getResultingAngle();

  }

  float get_angle_accelerometer(){
    return get_angle_accel();
  }

  void tick(double dt){
    mpu.getMotion6(&_ax, &_ay, &_az, &_gx, &_gy, &_gz);
    get_angle_gyro(dt);
    
  }

  void setFilterCoeff(float value){
    _filterCoeff = value;
  }

private:

  float getResultingAngle(){
    _gyro_angle_y = _gyro_angle_y * _filterCoeff + get_angle_accel() * (1 - _filterCoeff);
    return _gyro_angle_y;
  }

  float get_angle_accel(){


    float _asx = (float)_ax / 16384.0;       // converting to +-1.0

    float _asy = (float)_ay / 16384.0;       // converting to +-1.0

    float _asz = (float)_az / 16384.0;       // converting to +-1.0

    float accel_angle_x = degrees(atan2(_asy, _asz) + PI);
    float accel_angle_y = degrees(atan2(_asx, _asz) + PI);
    float accel_angle_z = degrees(atan2(_asy, _asx) + PI);

    return accel_angle_y;
  }

  double get_angle_gyro(double dt){
    timerPrev = timer;
    timer = millis();
    
    double _gsx = _gx / 131.0;       

    double _gsy = _gy / 131.0;      

    double _gsz = _gz / 131.0;    
    _gyro_angle_x += _gsx * (timer - timerPrev) / 1000.0;
    _gyro_angle_y += _gsy * (timer - timerPrev) / 1000.0;
    _gyro_angle_z += _gsz * (timer - timerPrev) / 1000.0;

    return _gyro_angle_y;
  }

  MPU6050 mpu;
  float _gyro_angle_x, _gyro_angle_y, _gyro_angle_z;
  float _filterCoeff;
  int16_t _ax, _ay, _az, _gx, _gy, _gz;
  double timerPrev, timer;
  double _angle;
};