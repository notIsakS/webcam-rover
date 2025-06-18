/*
ROVER PROJECT MADE BY NotIsakS
This project was created to learn C++ code, 
basic robotics, and using external devices such as a camera.
*/
#include <Arduino_LSM6DS3.h>
#include <Arduino_PMIC.h>
#include <ArduinoMotorCarrier.h>
#include <Arduino.h>
#include <cmath>

void encoderReadings();
double travelDistance(double x1_robot, double y1_robot, double x2_circle, double y2_circle);
double x1_robot = 0;
double y1_robot = 0;
double x2_circle = 0;
double y2_circle = 0;

float batteryVoltage;

static float length = 8.5;   //cm
static float w_radius = 4.5; //cm

void setup()
{
  //Serial port initialization
  Serial.begin(115200);
  while(!Serial)
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

  if (!IMU.begin()) {
    Serial.println("Failed to initialize IMU!");

    while (1);
  }

  Serial.print("Gyroscope sample rate = ");
  Serial.print(IMU.gyroscopeSampleRate());
  Serial.println(" Hz");
  Serial.println();
  Serial.println("Gyroscope in degrees/second");
  Serial.println("X\tY\tZ");

  batteryVoltage = battery.getRaw()/236.0; //236 for Nano, 77 for MKR.
  Serial.print("Battery voltage: ");
  Serial.print(batteryVoltage, 3);
  Serial.print("V, Raw ");
  Serial.println(battery.getRaw());

  x1_robot = 287.0;
  x2_circle = 437.0;
  y1_robot = 184.0;
  y2_circle = 319.0;
}

void loop() {
  controller.ping();
  delay(30);

  float gx, gy, gz;
  if (IMU.gyroscopeAvailable()) {
    IMU.readGyroscope(gx, gy, gz);

    Serial.print(gx);
    Serial.print('\t');
    Serial.print(gy);
    Serial.print('\t');
    Serial.println(gz);
  }

  //encoderReadings();
  delay(100);

  //travelDistance(x1_robot, y1_robot, x2_circle, y2_circle);
  delay(100);
}


double travelDistance(double x1, double y1, double x2, double y2){
  double dx = (x2 - x1);
  double dy = (y2 - y1);
  double theta = atan2(dy, dx);
  double r = sqrt(pow(dx/13.6,2) + pow(dy/12.6315,2));

  Serial.print("\r Angle (rad): ");
  Serial.println(theta);
  Serial.print("\r Distance (cm): ");
  Serial.println(r);

  return theta;
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