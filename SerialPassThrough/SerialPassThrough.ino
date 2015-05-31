/*
  Example mySerial Serial Passthrough Sketch
 by: Jim Lindblom
 SparkFun Electronics
 date: February 26, 2013
 license: Public domain

 This example sketch converts an RN-42 mySerial module to
 communicate at 9600 bps (from 115200), and passes any serial
 data between Serial Monitor and mySerial module.
 */
#include <SoftwareSerial.h>  

SoftwareSerial mySerial(10, 11);

void setup()
{
  Serial.begin(9600);  // Begin the serial monitor at 9600bps

  mySerial.begin(115200);  // The mySerial Mate defaults to 115200bps
  mySerial.write(" ");
//  mySerial.print("$");  // Print three times individually
//  mySerial.print("$");
//  mySerial.print("$");  // Enter command mode
//  mySerial.println("U,9600,N");  // Temporarily Change the baudrate to 9600, no parity
//  // 115200 can be too fast at times for NewSoftSerial to relay the data reliably
//  mySerial.begin(9600);  // Start mySerial serial at 9600
}

void loop()
{
  if(mySerial.available())  // If the mySerial sent any characters
  {
    // Send any characters the mySerial prints to the serial monitor
    Serial.print((char)mySerial.read());  
  }
  if(Serial.available())  // If stuff was typed in the serial monitor
  {
    // Send any characters the Serial monitor prints to the mySerial
    mySerial.print((char)Serial.read());
  }
  // and loop forever and ever!
}

