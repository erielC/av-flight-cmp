#include <Arduino.h>
#include <Wire.h>
#include "SensorManager.h"

RocketState currentState = INIT_AND_SYSTEM_CHECK;

void setup()
{
  Serial.begin(115200);
  Wire.begin();

  // Perform one-time sensor checks
  if (!InitializeAndCheckSensors())
  {
    Serial.println("[CRITICAL] One or more critical sensors failed. Halting...");
    while (true)
      ; // Halt on failure
  }

  // Transition to IDLE state for liftoff monitoring
  currentState = IDLE;
  Serial.println("[INFO] System ready. Awaiting liftoff.");
}

void loop()
{
  switch (currentState)
  {
  case IDLE:
    // if (CheckLiftoffConditions())
    {
      currentState = BOOST;
      Serial.println("[STATE] Transitioning to BOOST phase.");
    }
    break;

  default:
    break;
  }
}
