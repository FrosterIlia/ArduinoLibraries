#include "Operating_system.h"

#define TASKS_NUMBER 3

#define LED 2

Operating_system<TASKS_NUMBER> OS;


void setup() {
  Serial.begin(9600);

  OS.attach(0, task1, 3000);
  OS.attach(1, task2);
  OS.attach(2, task3);

  OS.set_period(0, 1000);
  OS.set_period(1, 2000);
  OS.set_period(2, 100);
}



void loop() {
  OS.tick();
}


void task1() {
  Serial.println("task1");
  digitalWrite(LED, !digitalRead(LED));
}

void task2() {

  Serial.println("task2");
}

void task3(){
  Serial.println(OS.get_left());
}

