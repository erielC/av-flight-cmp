#include "SensorManager.h"
#include <Arduino.h>
#include <Wire.h>
#include <SparkFun_MMC5983MA_Arduino_Library.h> // Magnetometer Library
#include <ASM330LHHSensor.h>                    // Main IMU Library
#include <Adafruit_BMP3XX.h>                    // Barometer Library

// Sensor objects
SFE_MMC5983MA magnetometer;
ASM330LHHSensor mainIMU(&Wire, ASM330LHH_I2C_ADD_L);
Adafruit_BMP3XX barometer;

// Logging helper
void logStatus(const char *device, const char *operation, bool success)
{
  if (success)
  {
    Serial.print("[INFO] ");
    Serial.print(device);
    Serial.print(": ");
    Serial.print(operation);
    Serial.println(" succeeded.");
  }
  else
  {
    Serial.print("[ERROR] ");
    Serial.print(device);
    Serial.print(": ");
    Serial.print(operation);
    Serial.println(" failed.");
  }
}

// I2C Device Connection Check
bool isDeviceConnected(uint8_t address)
{
  Wire.beginTransmission(address);
  return (Wire.endTransmission() == 0); // Returns true if device responds
}

// Magnetometer
bool PowerMagnetometer()
{
  if (!isDeviceConnected(MAGNETOMETER_ADDRESS))
    return logStatus("Magnetometer", "I2C Start Device Check", false), false;

  if (magnetometer.begin())
  {
    magnetometer.softReset();
    logStatus("Magnetometer", "Power Up", true);
    return true;
  }
  return logStatus("Magnetometer", "Power Up", false), false;
}

bool MagnetometerVerifyTemperature()
{
  if (magnetometer.verifyTemperature())
    return logStatus("Magnetometer", "Temperature Check", true), true;

  return logStatus("Magnetometer", "Temperature Check", false), false;
}

bool MagnetometerVerifyConnection()
{
  if (magnetometer.verifyConnection(MAGNETOMETER_ADDRESS))
    return logStatus("Magnetometer", "I2C Connection", true), true;

  return logStatus("Magnetometer", "I2C Connection", false), false;
}

// Main IMU
bool PowerMainIMU()
{
  if (!isDeviceConnected(MAIN_IMU_ADDRESS))
    return logStatus("Main IMU", "I2C Start Device Check", false), false;

  if (mainIMU.begin() == ASM330LHH_OK)
  {
    mainIMU.Enable_X();
    mainIMU.Enable_G();
    logStatus("Main IMU", "Power Up", true);
    return true;
  }
  return logStatus("Main IMU", "Power Up", false), false;
}

bool MainIMUVerifyTemperature()
{
  if (mainIMU.verifyTemperature() == ASM330LHH_OK)
    return logStatus("Main IMU", "Temperature Check", true), true;

  return logStatus("Main IMU", "Temperature Check", false), false;
}

bool MainIMUVerifyConnection()
{
  if (mainIMU.verifyConnection(MAIN_IMU_ADDRESS) == ASM330LHH_OK)
    return logStatus("Main IMU", "I2C Connection", true), true;

  return logStatus("Main IMU", "I2C Connection", false), false;
}

// Barometer
bool PowerBarometer()
{
  if (!isDeviceConnected(BAROMETER_ADDRESS))
    return logStatus("Barometer", "I2C Start Device Check", false), false;

  if (barometer.begin_I2C(BAROMETER_ADDRESS))
  {
    barometer.setTemperatureOversampling(BMP3_OVERSAMPLING_8X);
    barometer.setPressureOversampling(BMP3_OVERSAMPLING_4X);
    barometer.setIIRFilterCoeff(BMP3_IIR_FILTER_COEFF_3);
    barometer.setOutputDataRate(BMP3_ODR_50_HZ);
    logStatus("Barometer", "Power Up", true);
    return true;
  }
  return logStatus("Barometer", "Power Up", false), false;
}

bool BarometerVerifyTemperature()
{
  if (barometer.verifyTemperature())
    return logStatus("Barometer", "Temperature Check", true), true;

  return logStatus("Barometer", "Temperature Check", false), false;
}

bool BarometerVerifyConnection()
{
  if (barometer.verifyConnection(BAROMETER_ADDRESS))
    return logStatus("Barometer", "I2C Connection", true), true;

  return logStatus("Barometer", "I2C Connection", false), false;
}

// Overall Initialization and Check
bool InitializeAndCheckSensors()
{
  bool magnetometer = PowerMagnetometer() && MagnetometerVerifyTemperature() && MagnetometerVerifyConnection();
  bool imu = PowerMainIMU() && MainIMUVerifyTemperature() && MainIMUVerifyConnection();
  bool barometer = PowerBarometer() && BarometerVerifyTemperature() && BarometerVerifyConnection();

  return magnetometer && imu && barometer;
}