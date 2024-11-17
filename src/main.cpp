#include <Arduino.h>
#include <Wire.h>
#include <Adafruit_I2CDevice.h>
#include <SparkFun_MMC5983MA_Arduino_Library.h> // Magnetometer Library
#include <ASM330LHHSensor.h>                    // Main IMU Library

#define MAGNETOMETER_ADDRESS 0x30 // Magnetometer
#define MAIN_IMU_ADDRESS 0x6A     // ASM330LHH Main IMU

//////////////////// Function Prototypes ///////////////////
void PowerUpAllSensors(void);
bool PowerMagnetometer(void);
bool PowerMainIMU(void);
////////////////////////////////////////////////////////////

//////////////////// Object (Sensor) Classes ///////////////
SFE_MMC5983MA magnetometer;
ASM330LHHSensor mainIMU(&Wire, ASM330LHH_I2C_ADD_L);
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
  PowerUpAllSensors();
  if (currentState == SYSTEMS_CHECK)
  {
    Serial.println("STATE MACHINE: SYSTEM_CHECK");
  }
}

void loop()
{
}

void PowerUpAllSensors(void)
{
  bool allSensorsValid = true;
  // if (PowerMagnetometer())
  // {
  //   Serial.println("Magnetometer ready");
  // }
  // else
  // {
  //   Serial.println("Magnetometer failed to initialize");
  // }

  if (PowerMainIMU())
  {
    Serial.println("ASM330 Main IMU Ready");
  }
  else
  {
    Serial.println("Main IMU failed to initialize");
    allSensorsValid = false;
  }

  if (allSensorsValid)
  {
    currentState = SYSTEMS_CHECK;
  }
}

bool PowerMagnetometer(void)
{
  if (magnetometer.begin() == false)
  {
    Serial.println("MMC5983MA did not respond - check your wiring. Freezing.");
    while (true)
      ;
  }

  magnetometer.softReset();
  if (magnetometer.verifyConnection(MAGNETOMETER_ADDRESS))
  {
    Serial.println("MMC5983MA GO");
    return true;
  }
  else
  {
    Serial.println("MMC5983MA NO GO");
    return false;
  }
}

bool PowerMainIMU(void)
{
  if (mainIMU.begin() != 0)
  {
    Serial.println("ASM330LHH did not respond - check your wiring. Freezing.");
    while (true)
      ;
  }

  mainIMU.Enable_X(); // Enable accelerometer
  mainIMU.Enable_G(); // Enable gyroscope

  Serial.println("ASM330LHH GO");
  return true;
}