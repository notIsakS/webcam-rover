#include <Arduino_PMIC.h>
#include <ArduinoMotorCarrier.h>
#include <Arduino.h>


// Statemachine
enum State { 
  IDLE,
  MOVING
};


// Current state
State currentState = IDLE; 


void setup()
{
  //Serial port initialization
  Serial.begin(115200); 
  while (!Serial);


  //Establishing the communication with the Motor Carrier
  if (!controller.begin()){
    Serial.print("Motor Carrier connected, firmware version ");
    Serial.println(controller.getFWVersion());
  }
  else {
    Serial.println("Couldn't connect! Is the red LED blinking? You may need to update the firmware with FWUpdater sketch");
    while (1);
  }


  // Reboot the motor controller; brings every value back to default
  Serial.println("reboot");
  controller.reboot();
  delay(500);
}

void loop() {
  /* 
  Goals:
  Communicate with Arduino IoT Nano 33 - Done
  Make the rover go in a straight line and circles - tbd
  Get the forklift working with servo motor - tbd
  */
}