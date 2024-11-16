#include <Wire.h>
#include <Adafruit_Sensor.h>
#include "Adafruit_BMP3XX.h"

#define BMP_SCK 13
#define BMP_MISO 12
#define BMP_MOSI 11
#define BMP_CS 10

#define SEALEVELPRESSURE_HPA (1013.25)

Adafruit_BMP3XX barometer;

void setup()
{
  Serial.begin(115200);
  while (!Serial)
    ;
  Serial.println("Adafruit BMP388 / BMP390 test");

  if (!barometer.begin_I2C())
  { // hardware I2C mode, can pass in address & alt Wire
    // if (! bmp.begin_SPI(BMP_CS)) {  // hardware SPI mode
    // if (! bmp.begin_SPI(BMP_CS, BMP_SCK, BMP_MISO, BMP_MOSI)) {  // software SPI mode
    Serial.println("Could not find a valid BMP3 sensor, check wiring!");
    while (1)
      ;
  }

  // Set up oversampling and filter initialization
  barometer.setTemperatureOversampling(BMP3_OVERSAMPLING_8X);
  barometer.setPressureOversampling(BMP3_OVERSAMPLING_4X);
  barometer.setIIRFilterCoeff(BMP3_IIR_FILTER_COEFF_3);
  barometer.setOutputDataRate(BMP3_ODR_100_HZ);
}

void loop()
{
  if (!barometer.performReading())
  {
    Serial.println("Failed to perform reading :(");
    return;
  }
  Serial.print("Temperature = ");
  Serial.print(barometer.temperature);
  Serial.println(" *C");

  Serial.print("Pressure = ");
  Serial.print(barometer.pressure / 100.0);
  Serial.println(" hPa");

  Serial.print("Approx. Altitude = ");
  Serial.print(barometer.readAltitude(SEALEVELPRESSURE_HPA));
  Serial.println(" m");

  Serial.println();
  delay(2000);
}