/*
ROVER PROJECT MADE BY NotIsakS
This project was created to learn C++ code, 
basic robotics, and using external devices such as a camera.
*/

#include <Arduino_PMIC.h>
#include <ArduinoMotorCarrier.h>
#include <Arduino.h>


void encoderReadings();
float batteryVoltage;

void setup()
{
  //Serial port initialization
  Serial.begin(115200);

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
  Serial.println("reset counters");
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
  // Getting encoder readings
  encoderReadings();
  delay(1000);

  controller.ping();
  delay(50);
}

void encoderReadings(){
  Serial.print("Encoder1 Pos [counts]: ");
  Serial.print(encoder1.getRawCount());
  Serial.print(" Encoder1 vel [count/sec]:");
  Serial.println(encoder1.getCountPerSecond());
  Serial.print("Encoder2 pos [counts]: ");
  Serial.print(encoder2.getRawCount());
  Serial.print(" Encoder2 vel [count/sec]:");
  Serial.println(encoder2.getCountPerSecond());
  Serial.println("");
}