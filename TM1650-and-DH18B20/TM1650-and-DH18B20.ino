#include <OneWire.h>
#include <DallasTemperature.h>
#include <Wire.h>
#include <TM1650.h>
#include <math.h>

#define ONE_WIRE_BUS 2

OneWire oneWire(ONE_WIRE_BUS);
DallasTemperature sensors(&oneWire);
DeviceAddress insideThermometer;
TM1650 d;
unsigned long lastMillis = 0;


// function to print a device address
void printAddress(DeviceAddress deviceAddress)
{
  for (uint8_t i = 0; i < 8; i++)
  {
    if (deviceAddress[i] < 16) Serial.print("0");
    Serial.print(deviceAddress[i], HEX);
  }
}

void displayTemp(DeviceAddress deviceAddress) {
  float preF = sensors.getTempF(deviceAddress);
  Serial.print("Temperature (F) before rounding: ");
  Serial.println(preF);
  float f = roundf(preF * 100) / 100;
  String fStr = String(f);
  String space = String(' ');
  Serial.print("Temperature (F): ");
  Serial.println(fStr);
  char fChar[5]; 
  fStr.toCharArray(fChar, sizeof(fChar));
  d.displayString(fChar);
}

void setup() 
{
  
  Serial.begin(9600); //Start serial communication at 9600 for debug statements
  Serial.println("Thermostat Initializing");

  // locate devices on the bus
  Serial.print("Locating devices...");
  sensors.begin();
  Serial.print("Found ");
  Serial.print(sensors.getDeviceCount(), DEC);
  Serial.println(" devices.");
 
  // report parasite power requirements
  Serial.print("Parasite power is: "); 
  if (sensors.isParasitePowerMode()) {
    Serial.println("ON");
  }
  else {
    Serial.println("OFF");  
  }
  if (!sensors.getAddress(insideThermometer, 0)) {
    Serial.println("Unable to find address for Device 0"); 
  }
  
  Serial.print("Device 0 Address: ");
  printAddress(insideThermometer);
  Serial.println();

  // set the resolution to 9 bit (Each Dallas/Maxim device is capable of several different resolutions)
  sensors.setResolution(insideThermometer, 9);
 
  Serial.print("Device 0 Resolution: ");
  Serial.print(sensors.getResolution(insideThermometer), DEC); 
  Serial.println();

  Wire.begin(); //Join the bus as master

  d.init(); 
  d.displayOn();

}

void loop() 
{
  unsigned long currentMillis = millis();
  if (currentMillis >= (lastMillis + 5000)) {
    sensors.requestTemperatures();
    displayTemp(insideThermometer);
    lastMillis = currentMillis;
  }
}



