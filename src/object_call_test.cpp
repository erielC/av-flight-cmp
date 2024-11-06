// gauss measurement via obejct call

#include <Arduino.h>
#include <Wire.h>
#include <SparkFun_MMC5983MA_Arduino_Library.h>

//////////////////////// *** OBJECT  FUNCTIONS *** //////////////////////////////////

/* @erielC
  magnetometer object:

  magnetometer.initialTempCheck();
  magnetometer.monitorTemp();
  magnetometer.verifyConnection(MAGNETOMETER_ADDRESS);
  magnetometer.getGaussMeasurement();
  
*/

////////////////////////////////////////////////////////////////////////////////////

//////////////////////// *** VARIABLES *** //////////////////////////////////

// reason uint32_t is used instead of unsigned int is to ensure that the data type is an unsigned 32-bit integer across platforms and library uses it

// Variable to store the last time the measurements were taken
uint32_t lastMeasurementTime = 0;

// Measurement interval in milliseconds (10 seconds)
const uint32_t measurementInterval = 10000;

////////////////////////////////////////////////////////////////////////////

// Create an instance of the MMC5983MA class as magnetometer
SFE_MMC5983MA magnetometer;

// the reason a unsigned 8 bit integer is I2C addresses usually at most 7 bits long (saving memory) @erielC
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

  magnetometer.initialTempCheck();
  magnetometer.verifyConnection(MAGNETOMETER_ADDRESS);
}

void loop()
{
  // Check if 10 seconds have passed
  if (millis() - lastMeasurementTime >= measurementInterval)
  {
    // Update the last measurement time
    lastMeasurementTime = millis();

    // Perform measurements
    magnetometer.monitorTemp();
    magnetometer.verifyConnection(MAGNETOMETER_ADDRESS);
  }

  magnetometer.getGaussMeasurement();

  delay(100);
}