#include <Arduino_PMIC.h>
#include <ArduinoMotorCarrier.h>
#include <Arduino.h>


// Statemachine
enum State { 
  IDLE,
  MOVING
};


// Current state
// Will be made possible to write "STOP", or "RUN"
// To controll if the statemachine is working
// Sets PWM to 0
State currentState = MOVING;

float batteryVoltage;

void setup()
{
  //Serial port initialization
  Serial.begin(115200); 
  //while (!Serial);


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

  M3.setDuty(0);

  if (!PMIC.enableBoostMode()) {
    Serial.println("Error enabling Boost Mode");
  }
}

void loop() {
  batteryVoltage = battery.getRaw()/236.0; //236 for Nano, 77 for MKR.
  Serial.print("Battery voltage: ");
  Serial.print(batteryVoltage, 3);  
  Serial.print("V, Raw ");
  Serial.println(battery.getRaw());
  delay(5000); //wait for a few seconds  
  /* 
  Goals:
  Communicate with Arduino IoT Nano 33 - Done
  Make the rover go in a straight line and circles - tbd
  Get the forklift working with servo motor - tbd
  */

  for (int i = 45; i < 120; i += 1){
    servo3.setAngle(i);
    Serial.print("Servos position");
    Serial.println(i);
    delay(20);
  }
  controller.ping();
}