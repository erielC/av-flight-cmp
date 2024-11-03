// simple measurement

#include <Arduino.h>
#include <Wire.h>
#include <SparkFun_MMC5983MA_Arduino_Library.h>

//////////////////////// *** FUNCTIONS *** //////////////////////////////////

void initial_magnetometer_temp_check();
void monitor_magnetometer_temp();
void verify_magnetometer_connection();

////////////////////////////////////////////////////////////////////////////

//////////////////////// *** VARIABLES *** //////////////////////////////////

// reason uint32_t is used instead of unsigned int is to ensure that the data type is an unsigned 32-bit integer across platforms and library uses it

// Variable to store the last time the measurements were taken
uint32_t lastMeasurementTime = 0;

// Measurement interval in milliseconds (10 seconds)
const uint32_t measurementInterval = 10000;

////////////////////////////////////////////////////////////////////////////

// Create an instance of the MMC5983MA class as magnetometer
SFE_MMC5983MA magnetometer;
const uint8_t MAGNETOMETER_ADDRESS = 0x30; // magnetometers I2C address

void setup()
{
  // initialize communication between the microcontroller and the serial monitor at 115200 baud rate used for stable comms
  Serial.begin(115200);
  Serial.println("Magnetometer Test");

  // Sets up the I2C bus (communication) and prepares the Arduino to communicate with I2C devices
  Wire.begin();

  // mmagnetometer.begin() intilizes the sensor setting it up for I2C communication if detects false then goes into an infinite while loop if true then skips to rest of initilization process
  if (magnetometer.begin() == false)
  {
    Serial.println("MMC5983MA did not respond - check your wiring. Freezing.");

    // The reason that this is an infinite while loop is to basically freeze the program to prevent it from running if the device isnt good to go
    while (true)
      ;
  }

  // Soft reset operation resets the sensor's internal registers and state without needing to power cycle the entire device, ensuring that the sensor is in a known good state for operation.
  magnetometer.softReset();
  Serial.println("MMC5983MA is connected and ready");

  initial_magnetometer_temp_check();
  verify_magnetometer_connection();
}

void loop()
{
  // Check if 10 seconds have passed
  if (millis() - lastMeasurementTime >= measurementInterval)
  {
    // Update the last measurement time
    lastMeasurementTime = millis();

    // Perform measurements
    monitor_magnetometer_temp();
    verify_magnetometer_connection();
  }

  uint32_t currentX = 0;
  uint32_t currentY = 0;
  uint32_t currentZ = 0;
  double scaledX = 0;
  double scaledY = 0;
  double scaledZ = 0;

  currentX = magnetometer.getMeasurementX();
  currentY = magnetometer.getMeasurementY();
  currentZ = magnetometer.getMeasurementZ();

  Serial.print("X axis raw value: ");
  Serial.print(currentX);
  Serial.print("\tY axis raw value: ");
  Serial.print(currentY);
  Serial.print("\tZ axis raw value: ");
  Serial.println(currentZ);

  // Converting raw data
  scaledX = (double)currentX - 131072.0;
  scaledX /= 131072.0;
  scaledY = (double)currentY - 131072.0;
  scaledY /= 131072.0;
  scaledZ = (double)currentZ - 131072.0;
  scaledZ /= 131072.0;

  // The magnetometer full scale is +/- 8 Gauss
  // Multiply the scaled values by 8 to convert to Gauss
  Serial.print("X axis field (Gauss): ");
  Serial.print(scaledX * 8, 5); // Print with 5 decimal places

  Serial.print("\tY axis field (Gauss): ");
  Serial.print(scaledY * 8, 5);

  Serial.print("\tZ axis field (Gauss): ");
  Serial.println(scaledZ * 8, 5);

  Serial.println();
  delay(100);
}

void initial_magnetometer_temp_check()
{
  // Gets internal temp from integrated thermometer *reason data type is int is sensor temp range is -75C - 125C*
  float celsius = magnetometer.getTemperature();

  // Monitoring the internal temperature of the sensor, which can be relevant for temperature compensation in magnetic field measurements
  Serial.println("Die temperature: ");
  Serial.print("C: ");
  Serial.println(celsius);
}

void monitor_magnetometer_temp()
{
  float celsius = magnetometer.getTemperature();

  // Check for close-to-limit temperatures
  if (celsius < -35 || celsius > 100)
  {
    // FIXME add function that can turn off sensor and log all data it has recorded
    // POTENTIAL SOLUTION: make a function that calls it instead of a return statement ex. void handle_over_temperature();
  }
  else if (celsius < -10 || celsius > 90)
  {
    Serial.print("Close to limit C: ");
    Serial.println(celsius);
  }
  // Check for nominal operating temperature range
  else if (celsius >= 10 && celsius <= 60)
  {
    Serial.print("Nominal Operating Temperature C: ");
    Serial.println(celsius);
  }
}

void verify_magnetometer_connection()
{
  // Checking if the I2C connection for the magnetometer address is correct/working
  Wire.beginTransmission(MAGNETOMETER_ADDRESS);

  if (Wire.endTransmission() == 0)
  {
    Serial.println("Magnetometer connected successfully");
    // Connection is good; proceed with further operations if necessary
  }
  else
  {
    Serial.println("Failed to connect to the magnetometer");
    // Connection failed; handle the error (e.g., retry or enter a safe state)
  }
}