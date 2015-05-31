
#include <SoftwareSerial.h>

#define R1 1
#define R2 2
#define LF 10

SoftwareSerial tinySerial(3, 4); // RX, TX

void setup()  
{
  // Open serial communications and let us know we are connected
  tinySerial.begin(9600);
  pinMode(R1, OUTPUT);
  pinMode(R2, OUTPUT);
}

char read(String cmd) {
  int in = tinySerial.parseInt();
  while (in != LF) {
    cmd += read(cmd);
  }
  return in;
}

void loop()
{
  String cmd = "";
  if (tinySerial.available()) {
    read(cmd);
    tinySerial.println(cmd);   
  }
}

