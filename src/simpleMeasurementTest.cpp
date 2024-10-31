#include <Arduino.h>
#include <Wire.h>
#include <SparkFun_MMC5983MA_Arduino_Library.h>

//////////////////////// *** FUNCTIONS *** //////////////////////////////////

float celsiusToFarenheit(float temperature);

////////////////////////////////////////////////////////////////////////////

// Create an instance of the MMC5983MA class as mainMag (main magnetometer)
SFE_MMC5983MA mainMag;

void setup()
{
  // initialize communication between the microcontroller and the serial monitor at 115200 baud rate used for stable comms
  Serial.begin(115200);
  Serial.println("Magnetometer Test");

  // Sets up the I2C bus (communication) and prepares the Arduino to communicate with I2C devices
  Wire.begin();

  // mainMag.begin() intilizes the sensor setting it up for I2C communication if detects false then goes into an infinite while loop if true then skips to rest of initilization process
  if (mainMag.begin() == false)
  {
    Serial.println("MMC5983MA did not respond - check your wiring. Freezing.");

    // The reason that this is an infinite while loop is to basically freeze the program to prevent it from running if the device isnt good to go
    while (true)
      ;
  }

  // Soft reset operation resets the sensor's internal registers and state without needing to power cycle the entire device, ensuring that the sensor is in a known good state for operation.
  mainMag.softReset();
  Serial.println("MMC5983MA is connected and ready");

  // Gets internal temp from integrated thermometer *reason data type is int is sensor temp range is -75C - 125C*
  float temperature = mainMag.getTemperature();
  temperature = celsiusToFarenheit(temperature); // converts celsius to farenheit;

  // Monitoring the internal temperature of the sensor, which can be relevant for temperature compensation in magnetic field measurements
  Serial.print("Die temperature: ");
  Serial.print(temperature);
}

void loop()
{
  // reason uint32_t is used instead of unsigned int is to ensure that the data type is an unsigned 32-bit integer across platforms and library uses it
  uint32_t currentX = 0;
  uint32_t currentY = 0; 
  uint32_t currentZ = 0;

}

float celsiusToFarenheit(float temperature)
{
  temperature = (temperature * 9.0f / 5.0f) + 32.0f;
  return temperature;
}
