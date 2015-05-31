
#include <Mirf.h>
#include <MirfHardwareSpi85Driver.h>
#include <SPI85.h>

int led = 13;

void flash(int x) {
  for (int i = 0; i < x; i++) {
    digitalWrite(led, HIGH);
    delay(250); 
    digitalWrite(led, LOW);
    delay(250);
  }
};

void error() {
  digitalWrite(led, HIGH);
  delay(1000);
  digitalWrite(led, LOW); 
}

void setup() {
  Mirf.spi = &MirfHardwareSpi85;
  Mirf.init();
  
  Mirf.setRADDR((byte *)"foobar");
  
  Mirf.payload = sizeof(byte);
  Mirf.channel = 90;
  Mirf.config();
  
  Mirf.configRegister(RF_SETUP, 0x06);
  
  pinMode(led, OUTPUT);

  flash(3);
  delay(3000);
}

void loop() {
  uint8_t nrfStatus;

  nrfStatus = Mirf.getStatus();
  nrfStatus = Mirf.getStatus();

  if((byte)nrfStatus == 14) {
    byte v = 3;
    
    Mirf.send(&v);
    
    while (Mirf.isSending()) {
      flash(1);
    }

  } else {
    error();
  }
}
