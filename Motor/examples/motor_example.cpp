#include "Arduino.h"
#include "Motor.h"


#define AIN1 GPIO_NUM_5
#define AIN2 GPIO_NUM_18
#define PWM1 GPIO_NUM_19



Motor motor1(AIN1, AIN2, PWM1);


void setup(){
    Serial.begin(9600);

    motor1.setResolution(10);
    motor1.setMinSignal(395);
    motor1.setSmooth(1);

}

int16_t speed = 0;


void loop(){

    motor1.tick();

    if (Serial.available() > 1){
        char key = Serial.read();
        switch (key){
            case 's':
                speed = Serial.parseInt();
                motor1.setSmoothSpeed(speed);
                Serial.println(speed);

            break;

            case 'b':
                motor1.setMode(BRAKE);
                motor1.setSpeed(speed);
                Serial.parseInt();
                Serial.println("mode brake");

                break;

            case 'o':
                motor1.setMode(STOP);
                motor1.setSpeed(speed);
                Serial.parseInt();
                Serial.println("mode stop");

                break;

            case 'f':
                motor1.setMode(FORWARD);
                motor1.setSpeed(speed);
                Serial.parseInt();
                Serial.println("mode forward");

                break;
            case 'a':
                motor1.setMode(AUTO);
                motor1.setSpeed(speed);
                Serial.parseInt();
                Serial.println("mode auto");

                break;

            case 'z':
                motor1.setMode(BACKWARDS);
                motor1.setSpeed(speed);
                Serial.parseInt();
                Serial.println("mode backwards");

                break;
            
            case 'd':
                if (Serial.parseInt()) motor1.setDirection(REVERSE);
                else motor1.setDirection(NORMAL);
                Serial.println("direction changed");
                break;
        }
    }
}