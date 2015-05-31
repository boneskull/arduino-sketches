#include <Mirf.h>

int led = 13;

void setup()
{
  
  Mirf.init();
  
  // name the receiving channel - must match tranmitter setting!
  Mirf.setRADDR((byte *)"foobar");
  
  // just a single byte is transmitted
  Mirf.payload = sizeof(byte);

  // we use channel 90 as it is outside of WLAN bands 
  // or channels used by wireless surveillance cameras 
  Mirf.channel = 90;
  
  // now config the device.... 
  Mirf.config();  
  
  // Set 1MHz data rate - this increases the range slightly
  Mirf.configRegister(RF_SETUP,0x06);
  
  pinMode(led, OUTPUT);

  for(int i = 0; i < 3; i++) {
    digitalWrite(led, HIGH);
    delay(250);
    digitalWrite(led, LOW);
    delay(250);
  }  
  
  digitalWrite(led, HIGH);
  delay(1000);
  digitalWrite(led, LOW);
  delay(3000);
}

void loop()
{

  byte c; 

  // is there any data pending? 
  if( Mirf.dataReady() )
  {
     // well, get it
     Mirf.getData(&c);

    // ... and write it out to the PC
    digitalWrite(led, HIGH);
    delay(250);
    digitalWrite(led, LOW);
  }
}

