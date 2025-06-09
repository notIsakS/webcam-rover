#include <Arduino_PMIC.h>
#include <ArduinoMotorCarrier.h>
#include <Arduino.h>

/* States for which the rover can be in */
enum State {
  IDLE,
  MOVING
};

State currentState = IDLE;

void setup()
{
  Serial.begin(115200);
  while (!Serial);
  /* Fix issue with Controller not being recognized, or find other way to make it work.*/
  if (!Controller.begin()) {
    Serial.println("Motor carrier not detected.");
    while (1);
  } 
}

void loop() {
  /* 
  Goals:
  Communicate with Arduino IoT Nano 33 - Done
  Make the rover go in a straight line and circles - tbd
  Get the forklift working with servo motor - tbd
  */
}