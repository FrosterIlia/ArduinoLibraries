#pragma once
#include <Arduino.h>
#include "Wire.h"

#define DS3231_ADDRESS 0b01101000


class RTC {
public:

  void request_time() {
    Wire.beginTransmission(DS3231_ADDRESS);
    Wire.write(0x00);
    Wire.endTransmission();
    // Serial.println("hello");

    Wire.requestFrom(DS3231_ADDRESS, 3);

    if (Wire.available() >= 3) {
      seconds_raw = Wire.read();
      minutes_raw = Wire.read();
      hours_raw = Wire.read();
      // Serial.println(seconds_raw);
    }

    seconds = (seconds_raw >> 4) * 10 + (seconds_raw & 0b00001111);
    minutes = (minutes_raw >> 4) * 10 + (minutes_raw & 0b00001111);
    hours = (hours_raw >> 4) * 10 + (hours_raw & 0b00001111);

    Serial.println(seconds);
    Serial.println(minutes);
    Serial.println(hours);
    Serial.println("end");
    Serial.println(__TIME__);


  }

  void set_sys_time(){
    Wire.beginTransmission(DS3231_ADDRESS);
    Wire.write(0x00);
    Wire.write((__TIME__[6] - 48) << 4 | (__TIME__[7] - 48));
    Wire.write((__TIME__[3] - 48) << 4 | (__TIME__[4] - 48));
    Wire.write((__TIME__[0] - 48) << 4 | (__TIME__[1] - 48));
    
    Wire.endTransmission();
  }

  uint8_t get_seconds(){
    return seconds;
  }
  uint8_t get_minutes(){
    return minutes;
  }
  uint8_t get_hours(){
    return hours;
  }


private:
  uint8_t seconds, minutes, hours;
  uint8_t seconds_raw, minutes_raw, hours_raw;
};
