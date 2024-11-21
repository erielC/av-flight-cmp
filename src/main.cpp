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
void PowerUpAllSensors(void);
void CheckAllSensors(void);
bool PowerMagnetometer(void);
bool MagnetometerReadTemperature(void);
bool MagnetometerVerifyConnection(void);
bool PowerMainIMU(void);
// bool MainIMUReadTemperature(void);
bool MainIMUVerifyConnection(void);
bool PowerBarometer(void);
////////////////////////////////////////////////////////////

//////////////////// Object (Sensor) Classes ///////////////
SFE_MMC5983MA magnetometer;
ASM330LHHSensor mainIMU(&Wire, ASM330LHH_I2C_ADD_L);
Adafruit_BMP3XX barometer;
////////////////////////////////////////////////////////////

enum RocketState
{
  POWER_ON,      // Initial state after powering on
  SYSTEMS_CHECK, // System check before launch
  IDLE,          // Awaiting launch conditions
  BOOST,         // Rocket boost phase
  COAST,         // Post-boost, coasting to apogee
  APOGEE,        // Highest Altitude Reached
  DROGUE_DEPLOY, // Descent phase (includes drogue parachutes)
  MAIN_DEPLOY,   // Descent phase (includes main parachutes)
  LANDED,        // Final state after landing
};

// Declare the initial state variable
RocketState currentState = POWER_ON;

void setup()
{
  Serial.begin(115200);
  Wire.begin(); // Sets up I2C bus
}

void loop()
{
  switch (currentState)
  {
  case POWER_ON:
    Serial.println("STATE: POWER_ON");
    PowerUpAllSensors();
    break;
  case SYSTEMS_CHECK:
    //Serial.println("STATE: SYSTEMS_CHECK");
    // CheckAllSensors();
    break;
  default:
    break;
  }
}

void PowerUpAllSensors(void)
{
  bool allSensorsValid = true;
  if (PowerMagnetometer())
  {
    Serial.println("MMC5983 Magnetometer Ready");
  }
  else
  {
    Serial.println("MMC5983 Magnetometer failed to initialize");
    allSensorsValid = false;
  }

  if (PowerMainIMU())
  {
    Serial.println("ASM330 Main IMU Ready");
  }
  else
  {
    Serial.println("Main IMU failed to initialize");
    allSensorsValid = false;
  }

  if (PowerBarometer())
  {
    Serial.println("BMP390 Barometer Ready");
  }
  else
  {
    Serial.println("BMP390 Barometer failed to initialize");
    allSensorsValid = false;
  }

  if (allSensorsValid)
  {
    currentState = SYSTEMS_CHECK;
  }
  else
  {
    Serial.println("One or more sensors failed to initialize.");
    // Handle the error, e.g., enter a safe state or retry initialization
  }
}

bool PowerMagnetometer(void)
{
  if (magnetometer.begin() == false)
  {
    Serial.println("MMC5983MA did not respond - check your wiring. Freezing.");
    return false;
  }

  magnetometer.softReset();
  Serial.println("MMC5983MA ON");
  return true;
}

bool PowerMainIMU(void)
{
  if (mainIMU.begin() != 0)
  {
    Serial.println("ASM330LHH did not respond - check your wiring. Freezing.");
    return false;
  }

  mainIMU.Enable_X(); // Enable accelerometer
  mainIMU.Enable_G(); // Enable gyroscope

  Serial.println("ASM330LHH ON");
  return true;
}

bool PowerBarometer(void)
{
  if (barometer.begin_I2C() == false)
  {
    Serial.println("BMP390 did not respond - check your wiring. Freezing.");
    return false;
  }

  //
  barometer.setTemperatureOversampling(BMP3_OVERSAMPLING_8X);
  barometer.setPressureOversampling(BMP3_OVERSAMPLING_4X);
  barometer.setIIRFilterCoeff(BMP3_IIR_FILTER_COEFF_3);
  barometer.setOutputDataRate(BMP3_ODR_50_HZ);

  Serial.println("BMP390 ON");
  return true;
}

bool MagnetometerReadTemperature(void)
{
  if (magnetometer.readTemperature())
  {
    Serial.println("MMC5983 Magnetometer GO");
    return true;
  }
  else
  {
    Serial.println("MMC5983 Magnetometer NO GO TOO HOT/COLD");
    return false;
  }
}

bool MagnetometerVerifyConnection(void)
{
  if (magnetometer.verifyConnection(MAGNETOMETER_ADDRESS))
  {
    Serial.println("MMC5983 Magnetometer Connection Verified");
    return true;
  }
  else
  {
    Serial.println("MMC5983 Magnetometer Connection Failed");
    return false;
  }
}

bool MainIMUReadTemperature(void)
{
  if (mainIMU.readTemperature() == 0)
  {
    Serial.println("ASM330LHH Main IMU GO");
    return true;
  }
  else
  {
    Serial.println("ASM330LHH Main IMU NO GO");
    return false;
  }
}

bool MainIMUVerifyConnection(void)
{
  if (mainIMU.verifyConnection(MAIN_IMU_ADDRESS) == 0)
  {
    Serial.println("ASM330LHH Main IMU Connection Verified");
    return true;
  }
  else
  {
    Serial.println("ASM330LHH Main IMU Connection Failed");
    return false;
  }
}