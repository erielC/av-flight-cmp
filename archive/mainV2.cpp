#include <Arduino.h>
#include <Wire.h>
#include <Adafruit_I2CDevice.h>
#include <SparkFun_MMC5983MA_Arduino_Library.h> // Magnetometer Library
#include <ASM330LHHSensor.h>                    // Main IMU Library
#include <Adafruit_Sensor.h>                    // Barometer Library
#include "Adafruit_BMP3XX.h"                    // Barometer Library

#define MAGNETOMETER_ADDRESS 0x30 // MMC5983 Magnetometer
#define MAIN_IMU_ADDRESS 0x6A     // ASM330LHH Main IMU
#define BAROMETER_ADDRESS 0x76    // BMP390 Barometer

//////////////////// Function Prototypes ///////////////////
bool InitializeAndCheckSensors(void);

bool PowerMagnetometer(void);
bool MagnetometerVerifyTemperature(void);
bool MagnetometerVerifyConnection(void);

bool PowerMainIMU(void);
bool MainIMUVerifyTemperature(void);
bool MainIMUVerifyConnection(void);
bool isDeviceConnected(uint8_t address); // Main IMU only

bool PowerBarometer(void);
bool BarometerVerifyTemperature(void);
bool BarometerVerifyConnection(void);
////////////////////////////////////////////////////////////

//////////////////// Object (Sensor) Classes ///////////////
SFE_MMC5983MA magnetometer;
ASM330LHHSensor mainIMU(&Wire, ASM330LHH_I2C_ADD_L);
Adafruit_BMP3XX barometer;
////////////////////////////////////////////////////////////

// Enum to represent the state of rocket
enum RocketState
{
  INIT_AND_SYSTEM_CHECK, // Combined state for powering up and checking sensors
  IDLE,                  // Awaiting launch conditions
  BOOST,                 // Rocket boost phase
  COAST,                 // Post-boost, coasting to apogee
  APOGEE,                // Highest Altitude Reached
  DROGUE_DEPLOY,         // Descent phase (includes drogue parachutes)
  MAIN_DEPLOY,           // Descent phase (includes main parachutes)
  LANDED,                // Final state after landing
};

// Enum to represent the status codes
enum StatusCode
{
  SUCCESS = 0,
  FAILURE = 1
};

// Declare the initial state variable
RocketState currentState = INIT_AND_SYSTEM_CHECK;

void setup()
{
  Serial.begin(115200);
  Wire.begin(); // Sets up I2C bus
}

void loop()
{
  switch (currentState)
  {
  case INIT_AND_SYSTEM_CHECK:
    Serial.println("STATE: INIT_AND_CHECK");
    if (InitializeAndCheckSensors())
    {
      Serial.println("All sensors initialized and verified successfully.");
      currentState = IDLE;
    }
    else
    {
      Serial.println("Sensor initialization or verification failed. Halting.");
      // Handle failure: Retry, log error, or enter a safe state
    }
    break;
  case IDLE:
    // Serial.println("STATE: IDLE");
    break;
  default:
    break;
  }
}

bool InitializeAndCheckSensors()
{
  bool allSensorsValid = true;

  // Power up and check Magnetometer
  if (PowerMagnetometer())
  {
    Serial.println("MMC5983 Magnetometer Powered Up.");
    if (!MagnetometerVerifyTemperature())
    {
      Serial.println("Magnetometer Temperature Check Failed.");
      allSensorsValid = false;
    }

    if (!MagnetometerVerifyConnection())
    {
      Serial.println("Magnetometer I2C Connection Check Failed.");
      allSensorsValid = false;
    }
  }
  else
// Enum to represent the state of the rocket
  {
    Serial.println("Magnetometer Initialization Failed.");
    allSensorsValid = false;
  }

  // Power up and check Main IMU
  if (PowerMainIMU())
  {
    Serial.println("ASM330 Main IMU Powered Up.");
    if (!MainIMUVerifyTemperature())
    {
      Serial.println("Main IMU Temperature Check Failed.");
      allSensorsValid = false;
    }

    if (!MainIMUVerifyConnection())
    {
      Serial.println("Main IMU I2C Connection Check Failed.");
      allSensorsValid = false;
    }
  }
  else
  {
    Serial.println("Main IMU Initialization Failed.");
    allSensorsValid = false;
  }

  // Power up and check Barometer
  if (PowerBarometer())
  {
    Serial.println("BMP390 Barometer Powered Up.");
    if (!BarometerVerifyTemperature())
    {
      Serial.println("Barometer Temperature Check Failed.");
      allSensorsValid = false;
    }

    if (!BarometerVerifyConnection())
    {
      Serial.println("Barometer I2C Connection Check Failed.");
      allSensorsValid = false;
    }
  }
  else
  {
    Serial.println("Barometer Initialization Failed.");
    allSensorsValid = false;
  }

  return allSensorsValid;
}

bool PowerMagnetometer()
{
  if (magnetometer.begin())
  {
    magnetometer.softReset();
    Serial.println("MMC5983MA ON");
    return true;
  }
  Serial.println("MMC5983MA did not respond - check your wiring.");
  return false;
}

bool MagnetometerVerifyTemperature(void)
{
  if (magnetometer.verifyTemperature())
  {
    Serial.println("MMC5983 Magnetometer Temperature Verified");
    return true;
  }
  Serial.println("MMC5983 Magnetometer Temperature Check Failed.");
  return false;
}

bool MagnetometerVerifyConnection(void)
{
  if (magnetometer.verifyConnection(MAGNETOMETER_ADDRESS))
  {
    Serial.println("MMC5983 Magnetometer Connection Verified");
    return true;
  }
  Serial.println("MMC5983 Magnetometer Connection Failed.");
  return false;
}

/* SUCCESS and ASM330LHH_OK are both valued at 0 reason were using the ASM330LHH_OK enum is to match the struct in the ASM330 Library
  Functions using this:
  - PowerMainIMU()
  - MainIMUReadTemperature()
  - MainIMUVerifyConnection()
*/
bool PowerMainIMU(void)
{
  // Explicit I2C device check before initializing
  if (!isDeviceConnected(MAIN_IMU_ADDRESS))
  {
    Serial.println("ASM330LHH not found on the I2C bus.");
    return false;
  }

  if (mainIMU.begin() == ASM330LHH_OK)
  {
    mainIMU.Enable_X();
    mainIMU.Enable_G();
    Serial.println("ASM330LHH ON");
    return true;
  }

  Serial.println("ASM330LHH did not respond - check your wiring.");
  return false;
}

bool MainIMUVerifyTemperature(void)
{
  if (mainIMU.verifyTemperature() == ASM330LHH_OK)
  {
    Serial.println("ASM330LHH Main IMU Temperature Verified");
    return true;
  }
  Serial.println("ASM330LHH Main IMU Temperature Check Failed.");
  return false;
}

bool isDeviceConnected(uint8_t address)
{
  Wire.beginTransmission(address);
  return (Wire.endTransmission() == 0); // 0 means the device acknowledged
}

bool MainIMUVerifyConnection(void)
{
  if (isDeviceConnected(MAIN_IMU_ADDRESS) && mainIMU.verifyConnection(MAIN_IMU_ADDRESS) == ASM330LHH_OK)
  {
    Serial.println("ASM330LHH Main IMU Connection Verified");
    return true;
  }
  Serial.println("ASM330LHH Main IMU Connection Failed.");
  return false;
}

bool PowerBarometer(void)
{
  if (barometer.begin_I2C(BAROMETER_ADDRESS))
  {
    barometer.setTemperatureOversampling(BMP3_OVERSAMPLING_8X);
    barometer.setPressureOversampling(BMP3_OVERSAMPLING_4X);
    barometer.setIIRFilterCoeff(BMP3_IIR_FILTER_COEFF_3);
    barometer.setOutputDataRate(BMP3_ODR_50_HZ);
    Serial.println("BMP390 ON");
    return true;
  }
  Serial.println("BMP390 did not respond - check your wiring.");
  return false;
}

bool BarometerVerifyTemperature(void)
{
  if (barometer.verifyTemperature())
  {
    Serial.println("BMP390 Barometer Temperature Verified");
    return true;
  }
  Serial.println("BMP390 Barometer Temperature Check Failed.");
  return false;
}

bool BarometerVerifyConnection(void)
{
  if (barometer.verifyConnection())
  {
    Serial.println("Device is ready to use.");
    return true;
  }
  Serial.println("Connection failed! Check wiring or I2C address.");
  return false;
}