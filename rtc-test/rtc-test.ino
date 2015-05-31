#include <Wire.h>
#include "RTClib.h"
RTC_DS1307 RTC;
void setup () {
  Serial.begin(57600); Wire.begin(); RTC.begin();
  if (! RTC.isrunning()) {
    Serial.println("RTC is NOT running!");
    // following line sets the RTC to the date & time this sketch
    was compiled
    // uncomment it & upload to set the time, date and start run the
    ￼￼￼￼￼￼￼￼RTC!
    //RTC.adjust(DateTime(__DATE__, __TIME__));
  }
}
￼Page 4 of 11 pages
￼
void loop () {
  DateTime now = RTC.now();
  Serial.print(now.year(), DEC); Serial.print('/'); Serial.print(now.month(), DEC); Serial.print('/'); Serial.print(now.day(), DEC); Serial.print(' '); Serial.print(now.hour(), DEC); Serial.print(':'); Serial.print(now.minute(), DEC); Serial.print(':'); Serial.print(now.second(), DEC); Serial.println(); Serial.println();
  delay(2000);
}
