#include <SPI.h>
#include <TFTv2.h>
#include <init_cmd.h>

#define SSID "BRIDE OF ZOLTAR"
#define PASS "xxxx"

TFT Tft=TFT();

void print(String s, int color=WHITE) {
  int len = s.length();
  char charBuf[len];
  s.toCharArray(charBuf, s.length());
  print((const char *)charBuf, color);
}

void print(char const *string, int color=WHITE) {
  Tft.fillScreen();
  Tft.drawString(string, 0, 0, 3, color);
}


void setup()
{
  int success = false;
  TFT_BL_ON;
  Tft.TFTinit(LANDSCAPE);

  // Open serial communications and wait for port to open:
  Serial.begin(9600);
  Serial.setTimeout(5000);

  while(!success) {
    Serial.println("AT+RST");
    delay(1000);
    if (Serial.find("ready")) {
      print("Ready!", GREEN);
      success = true;
    } else {
      print("Failure!", RED);
      delay(3000);
    }
  }



  delay(1000);

  connectWiFi();
}


boolean connectWiFi()
{
  Serial.println("AT+CWMODE=1");
  String cmd="AT+CWJAP=\"";
  cmd+=SSID;
  cmd+="\",\"";
  cmd+=PASS;
  cmd+="\"";
  String output = "Sending ";
  output = output + cmd;
  print(output);
  Serial.println(cmd);
  delay(2000);
  if(Serial.find("OK")){
    print("WiFi Success!", GREEN);
    return true;
  }else{
    print("WiFi Failure!", RED);
    return false;
  }
}

void loop() // run over and over
{
}
