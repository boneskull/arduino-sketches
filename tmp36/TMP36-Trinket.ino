
#include "TrinketFakeUsbSerial.h"
//initializes/defines the output pin of the TMP36 temperature sensor
int outputpin = 1;
long lastMillis = 0;

//this sets the ground pin to LOW and the input voltage pin to high
void setup()
{
  TFUSerial.begin();
}

//main loop
void loop()
{
  TFUSerial.task();
  long currentMillis = millis();
  if (currentMillis - lastMillis >= 3000) {
    int rawvoltage = analogRead(outputpin);
    float volts = rawvoltage / 205.0;
    float celsiustemp = 100.0 * volts - 50;
    float fahrenheittemp = celsiustemp * 9.0 / 5.0 + 32.0;
    TFUSerial.write((uint8_t)fahrenheittemp);
  }

  delay(5);

}

