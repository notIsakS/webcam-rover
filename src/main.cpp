/*

ROVER PROJECT MADE BY NotIsakS
This project is created by the inspiration 

*/


#include <Arduino_PMIC.h>
#include <ArduinoMotorCarrier.h>
#include <Arduino.h>

void printStatus(float voltage, int count1, float vel1, int count2, float vel2, int duty);

float batteryVoltage;

void setup()
{
  //Serial port initialization
  Serial.begin(115200); 
  //while (!Serial);

  //Establishing the communication with the Motor Carrier
  if (controller.begin()){
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
  
  // Reset the encoders internal counter to zero
  Serial.println("rest counters");
  encoder1.resetCounter(0);
  encoder2.resetCounter(0);

  // PWM for motors 0 MIN, 100 MAX
  M1.setDuty(0); // Negative for CW direction
  M2.setDuty(0); // Positive for CW direction

  if (!PMIC.enableBoostMode()) {
    Serial.println("Error enabling Boost Mode");
  }

  batteryVoltage = battery.getRaw()/236.0; //236 for Nano, 77 for MKR.
  Serial.print("Battery voltage: ");
  Serial.print(batteryVoltage, 3);
  Serial.print("V, Raw ");
  Serial.println(battery.getRaw());
}

void loop() {
/* 
  Goals:
  Communicate with Arduino IoT Nano 33 - Done
  Make the rover go in a straight line and circles - tbd
  Get the forklift working with servo motor - tbd
*/

  Serial.print("Encoder1 Pos [counts]: ");
  Serial.print(encoder1.getRawCount());
  Serial.print(" Encoder1 vel [count/sec]:");
  Serial.println(encoder1.getCountPerSecond());
  Serial.print("Encoder2 pos [counts]: ");
  Serial.print(encoder2.getRawCount());
  Serial.print(" Encoder2 vel [count/sec]:");
  Serial.println(encoder2.getCountPerSecond());
  Serial.println("");
  delay(5000);

  /*
  if (Serial.available() > 0)
  {
    int dutyCycle = Serial.parseInt();
    int safeDuty = constrain(dutyCycle, 0, 100);

    Serial.print("Received duty cycle: ");
    Serial.println(safeDuty);
    
    M1.setDuty(-safeDuty);
    M2.setDuty(safeDuty);

    
    // Controlling PWM signal by COM port
    switch (dutyCycle) 
    {
      case 0:
        M1.setDuty(-safeDuty);
        M2.setDuty(safeDuty);
      case 10:
        M1.setDuty(-safeDuty);
        M2.setDuty(safeDuty);
      case 25:
        M1.setDuty(-safeDuty);
        M2.setDuty(safeDuty);
      case 50:
        M1.setDuty(-safeDuty);
        M2.setDuty(safeDuty);
      case 75:
        M1.setDuty(-safeDuty);
        M2.setDuty(safeDuty);
      case 100:
        M1.setDuty(-safeDuty);
        M2.setDuty(safeDuty);
      default:
      Serial.println("Unknown value, try: 0, 10, 25, 50, 75, 100");
    }
  }
  */

  controller.ping();
  delay(50);
}


void printStatus(float voltage, int count1, float vel1, int count2, float vel2, int duty){
  Serial.write(27);
  Serial.print("[2J");
  Serial.write(27);
  Serial.print("[H");

  Serial.print("Battery Voltage: ");
  Serial.print(voltage, 3);
  Serial.println(" V");

  Serial.print("Encoder1: ");
  Serial.print(count1);
  Serial.print(" counts | ");
  Serial.print(vel1);
  Serial.println(" counts/s");

  Serial.print("Encoder2: ");
  Serial.print(count2);
  Serial.print(" counts | ");
  Serial.print(vel2);
  Serial.print(" counts/s");

  Serial.print("Duty Cycle: ");
  Serial.print(duty);
  Serial.println(" %");
}