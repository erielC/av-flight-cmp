#include <Wire.h>

#include <SparkFun_MMC5983MA_Arduino_Library.h>

SFE_MMC5983MA myMag;

void setup()
{
  Serial.begin(115200);
  Serial.println("MMC5983MA Example");

  Wire.begin();

  if (myMag.begin() == false)
  {
    Serial.println("MMC5983MA did not respond - check your wiring. Freezing.");
    while (true)
      ;
  }

  myMag.softReset();

  Serial.println("MMC5983MA connected");

  int celsius = myMag.getTemperature();
  float fahrenheit = (celsius * 9.0f / 5.0f) + 32.0f;

  Serial.print("Die temperature: ");
  Serial.print(celsius);
  Serial.print("°C or ");
  Serial.print(fahrenheit, 0);
  Serial.println("°F.");
}

void loop()
{
  uint32_t currentX = 0;
  uint32_t currentY = 0;
  uint32_t currentZ = 0;
  double scaledX = 0;
  double scaledY = 0;
  double scaledZ = 0;

  // This reads the X, Y and Z channels consecutively
  // (Useful if you have one or more channels disabled)
  currentX = myMag.getMeasurementX();
  currentY = myMag.getMeasurementY();
  currentZ = myMag.getMeasurementZ();

  // Or, we could read all three simultaneously
  // myMag.getMeasurementXYZ(&currentX, &currentY, &currentZ);

  Serial.print("X axis raw value: ");
  Serial.print(currentX);
  Serial.print("\tY axis raw value: ");
  Serial.print(currentY);
  Serial.print("\tZ axis raw value: ");
  Serial.println(currentZ);

  // The magnetic field values are 18-bit unsigned. The _approximate_ zero (mid) point is 2^17 (131072).
  // Here we scale each field to +/- 1.0 to make it easier to convert to Gauss.
  //
  // Please note: to properly correct and calibrate the X, Y and Z channels, you need to determine true
  // offsets (zero points) and scale factors (gains) for all three channels. Futher details can be found at:
  // https://thecavepearlproject.org/2015/05/22/calibrating-any-compass-or-accelerometer-for-arduino/
  scaledX = (double)currentX - 131072.0;
  scaledX /= 131072.0;
  scaledY = (double)currentY - 131072.0;
  scaledY /= 131072.0;
  scaledZ = (double)currentZ - 131072.0;
  scaledZ /= 131072.0;

  // The magnetometer full scale is +/- 8 Gauss
  // Multiply the scaled values by 8 to convert to Gauss
  Serial.print("X axis field (Gauss): ");
  Serial.print(scaledX * 8, 5); // Print with 5 decimal places

  Serial.print("\tY axis field (Gauss): ");
  Serial.print(scaledY * 8, 5);

  Serial.print("\tZ axis field (Gauss): ");
  Serial.println(scaledZ * 8, 5);

  Serial.println();
  delay(100);
}