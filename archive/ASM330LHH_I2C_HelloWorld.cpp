/**
 ******************************************************************************
 * @file    ASM330LHH_I2C_HelloWorld.ino
 * @brief   Arduino test application for the STEVAL-MKI193V1 adapter board via I2C.
 ******************************************************************************
 */

#include <Wire.h>            // Include the Wire library for I2C
#include <ASM330LHHSensor.h> // Include the sensor library

#define INT_1 A5 // Define INT_1 pin (adjust as needed for your setup)

// Initialize the sensor with the default I2C address
ASM330LHHSensor AccGyr(&Wire, ASM330LHH_I2C_ADD_L);

void setup()
{
  // Set up the onboard LED for status indication
  pinMode(LED_BUILTIN, OUTPUT);

  // Set INT1 of ASM330LHH low to enable I2C communication
  pinMode(INT_1, OUTPUT);
  digitalWrite(INT_1, LOW);

  delay(200); // Small delay for setup

  // Initialize serial communication
  Serial.begin(115200);

  // Initialize I2C bus
  Wire.begin();

  // Initialize and configure the sensor
  AccGyr.begin();
  AccGyr.Enable_X(); // Enable accelerometer
  AccGyr.Enable_G(); // Enable gyroscope
}

void loop()
{
  // Blink the onboard LED
  digitalWrite(LED_BUILTIN, HIGH);
  delay(250);
  digitalWrite(LED_BUILTIN, LOW);
  delay(250);

  // Variables to store sensor data
  int32_t accelerometer[3] = {0, 0, 0};
  int32_t gyroscope[3] = {0, 0, 0};

  // Read accelerometer and gyroscope data
  AccGyr.Get_X_Axes(accelerometer);
  AccGyr.Get_G_Axes(gyroscope);

  // Print sensor data to the serial monitor
  Serial.print("ASM330LHH: | Acc[mg]: ");
  Serial.print(accelerometer[0]);
  Serial.print(" ");
  Serial.print(accelerometer[1]);
  Serial.print(" ");
  Serial.print(accelerometer[2]);
  Serial.print(" | Gyr[mdps]: ");
  Serial.print(gyroscope[0]);
  Serial.print(" ");
  Serial.print(gyroscope[1]);
  Serial.print(" ");
  Serial.print(gyroscope[2]);
  Serial.println(" |");
}
