#include <Wire.h>
#include <SparkFun_MMC5983MA_Arduino_Library.h>

SFE_MMC5983MA magnetometer;

int interruptPin = 2;

volatile bool newDataAvailable = true;
uint32_t rawValueX = 0;
uint32_t rawValueY = 0;
uint32_t rawValueZ = 0;
double scaledX = 0;
double scaledY = 0;
double scaledZ = 0;
double heading = 0;

void setup()
{
  Serial.begin(115200);
  Serial.println("MMC5983MA Example");

  Wire.begin();

  // Configure the interrupt pin for the "Measurement Done" interrupt
  pinMode(interruptPin, INPUT);
  attachInterrupt(digitalPinToInterrupt(interruptPin), interruptRoutine, RISING);

  if (magnetometer.begin() == false)
  {
    Serial.println("MMC5983MA did not respond - check your wiring. Freezing.");
    while (true)
      ;
  }

  magnetometer.softReset();

  Serial.println("MMC5983MA connected");

  Serial.println("Setting filter bandwith to 100 Hz for continuous operation...");
  magnetometer.setFilterBandwidth(100);
  Serial.print("Reading back filter bandwith: ");
  Serial.println(magnetometer.getFilterBandwith());

  Serial.println("Setting continuous measurement frequency to 10 Hz...");
  magnetometer.setContinuousModeFrequency(10);
  Serial.print("Reading back continuous measurement frequency: ");
  Serial.println(magnetometer.getContinuousModeFrequency());

  Serial.println("Enabling auto set/reset...");
  magnetometer.enableAutomaticSetReset();
  Serial.print("Reading back automatic set/reset: ");
  Serial.println(magnetometer.isAutomaticSetResetEnabled() ? "enabled" : "disabled");

  Serial.println("Enabling continuous mode...");
  magnetometer.enableContinuousMode();
  Serial.print("Reading back continuous mode: ");
  Serial.println(magnetometer.isContinuousModeEnabled() ? "enabled" : "disabled");

  Serial.println("Enabling interrupt...");
  magnetometer.enableInterrupt();
  Serial.print("Reading back interrupt status: ");
  Serial.println(magnetometer.isInterruptEnabled() ? "enabled" : "disabled");

  // Set our interrupt flag, just in case we missed the rising edge
  newDataAvailable = true;
}

void loop()
{
  if (newDataAvailable == true)
  {
    newDataAvailable = false;              // Clear our interrupt flag
    magnetometer.clearMeasDoneInterrupt(); // Clear the MMC5983 interrupt

    // Read all three channels simultaneously
    // Note: we are calling readFieldsXYZ to read the fields, not getMeasurementXYZ
    // The measurement is already complete, we do not need to start a new one
    magnetometer.readFieldsXYZ(&rawValueX, &rawValueY, &rawValueZ);

    // The magnetic field values are 18-bit unsigned. The _approximate_ zero (mid) point is 2^17 (131072).
    // Here we scale each field to +/- 1.0 to make it easier to calculate an approximate heading.
    //
    // Please note: to properly correct and calibrate the X, Y and Z channels, you need to determine true
    // offsets (zero points) and scale factors (gains) for all three channels. Futher details can be found at:
    // https://thecavepearlproject.org/2015/05/22/calibrating-any-compass-or-accelerometer-for-arduino/
    scaledX = (double)rawValueX - 131072.0;
    scaledX /= 131072.0;

    scaledY = (double)rawValueY - 131072.0;
    scaledY /= 131072.0;

    scaledZ = (double)rawValueZ - 131072.0;
    scaledZ /= 131072.0;

    // Magnetic north is oriented with the Y axis
    // Convert the X and Y fields into heading using atan2 (Arc Tangent 2)
    heading = atan2(scaledX, 0 - scaledY);

    // atan2 returns a value between +PI and -PI
    // Convert to degrees
    heading /= PI;
    heading *= 180;
    heading += 180;

    Serial.print("Heading: ");
    Serial.println(heading, 1);
  }
}

void interruptRoutine()
{
  newDataAvailable = true;
}