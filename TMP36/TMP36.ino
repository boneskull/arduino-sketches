#include <LiquidCrystal.h>

//initializes/defines the output pin of the TMP36 temperature sensor
int outputpin = 0;


// initialize the library with the numbers of the interface pins
LiquidCrystal lcd(12, 11, 5, 4, 3, 2);

//this sets the ground pin to LOW and the input voltage pin to high
void setup()
{
  lcd.begin(16, 2);
}

//main loop
void loop()
{
  int rawvoltage = analogRead(outputpin);
  float volts = rawvoltage / 205.0;
  float celsiustemp = 100.0 * volts - 50;
  float fahrenheittemp = celsiustemp * 9.0 / 5.0 + 32.0;
  lcd.clear();
  lcd.print("Fahrenheit: ");
  lcd.print(fahrenheittemp);
  delay(30000);
}

