#ifndef SENSOR_MANAGER_H
#define SENSOR_MANAGER_H

#include <Arduino.h>

// Addresses for sensors
#define MAGNETOMETER_ADDRESS 0x30 // MMC5983 Magnetometer Address 0x30 -> hexadecimal:
#define MAIN_IMU_ADDRESS 0x6A     // ASM330LHH Main IMU
#define BAROMETER_ADDRESS 0x76    // BMP390 Barometer

// Enum to represent the state of the rocket
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

enum CriticalIndex
{
  MAIN_IMU,
  BAROMETER
}; // Indices for critical sensors
enum NonCriticalIndex
{
  MAGNETOMETER
}; // Indices for non-critical sensors

// Function prototypes
bool InitializeAndCheckSensors();

bool PowerMagnetometer();
bool MagnetometerVerifyTemperature();
bool MagnetometerVerifyConnection();

bool PowerMainIMU();
bool MainIMUVerifyTemperature();
bool MainIMUVerifyConnection();

bool PowerBarometer();
bool BarometerVerifyTemperature();
bool BarometerVerifyConnection();

bool isDeviceConnected(uint8_t address);

#endif
