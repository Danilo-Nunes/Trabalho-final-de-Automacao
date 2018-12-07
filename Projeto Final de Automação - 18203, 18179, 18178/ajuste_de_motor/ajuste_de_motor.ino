#include <Stepper.h>
#define graus 50

Stepper motor(360/graus, 8, 10, 9, 11);

void setup() {
  // put your setup code here, to run once:
  motor.setSpeed(3000);
    motor.step(-10);
}

void loop() {
}
