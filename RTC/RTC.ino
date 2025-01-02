#include "RTC.h"
#include "Wire.h"

RTC clock;

void setup() {
  Serial.begin(9600);

  Wire.begin();

  // clock.set_sys_time();


}
uint32_t timer;
void loop() {
  if (millis() - timer >= 1000){
    timer = millis();
    clock.request_time();
  }
}
