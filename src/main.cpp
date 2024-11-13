#include <Arduino.h>
#include <Wire.h> // Include the Wire library for I2C

enum RocketState
{
  POWER_ON,      // Initial state after powering on
  SYSTEMS_CHECK, // System check before launch
  IDLE,          // Awaiting launch conditions
  BOOST,         // Rocket boost phase
  COAST,         // Post-boost, coasting to apogee
  APOGEE,
  DROGUE_DEPLOY, // Descent phase (includes drogue parachutes)
  MAIN_DEPLOY,   // Descent phase (includes main parachutes)
  LANDED,        // Final state after landing
};

// Declare the initial state variable
RocketState currentState = POWER_ON;

void setup()
{
}

void loop()
{
}
