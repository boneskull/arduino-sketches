
// Date and time functions using a DS1307 RTC connected via I2C and Wire lib

#include <Wire.h>
#include <RTClib.h>
#include <Adafruit_GFX.h>
#include <Adafruit_LEDBackpack.h>

Adafruit_7segment matrix = Adafruit_7segment();
RTC_DS1307 RTC;
DateTime lastTime;
boolean colon = false;
unsigned long int prevMillis = 0;

void displayTime(DateTime dateTime) {
  int hour = dateTime.hour();
  int min = dateTime.minute();
  Serial.print(hour);
  Serial.print(':');
  Serial.println(min);

  int time = ((int)(hour / 10) * 1000) + ((int)(hour % 10) * 100) + ((int)(min / 10) * 10) + ((int)min % 10);
  matrix.println(time);
  matrix.writeDisplay();
}

void setup () {
  Serial.begin(57600);
  Wire.begin();
  RTC.begin();
//    RTC.adjust(DateTime(__DATE__, __TIME__));  
  if (! RTC.isrunning()) {
    Serial.println("RTC is NOT running!"); 
  }
  else {
    matrix.begin(0x70);
    matrix.clear();
    matrix.writeDisplay();

    lastTime = RTC.now();
    displayTime(lastTime);
  }

}

void loop () {
  unsigned long int currentMillis = millis();
  if (currentMillis - prevMillis >= 500) {
    DateTime now = RTC.now();

    if (lastTime.unixtime() < now.unixtime()) {
      colon = !colon;
      matrix.drawColon(colon);
      matrix.writeDisplay();
    }
    if (currentMillis - prevMillis >= 1000) {
      displayTime(now);
      prevMillis = currentMillis;
    }
    lastTime = now;
    
  }
}
