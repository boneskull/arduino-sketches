#include <SoftwareSerial.h>

#define SSID "BRIDE OF ZOLTAR"      //your wifi ssid here
#define PASS "xxxx"   //your wifi wep key here

#define LOCATIONID "5746545" // location id
#define DST_IP "188.226.224.148" //api.openweathermap.org
#define HOST "api.openweathermap.org"

SoftwareSerial dbgSerial(2, 3); // RX, TX

TFT tft = TFT();

char myChar;

void print(String s, int poX=0, int poY=0, int size=2, int fgcolor=WHITE, Font* font=NULL) {
  int len = s.length();
  char buf[len];
  s.toCharArray(buf, len);
  tft.print((const char*)buf, poX, poY, size, fgColor, *font);
}

void setup()
{
  TFT_BL_ON;
  tft.TFTinit(LANDSCAPE);

  // Open serial communications for WiFi module:
  Serial.begin(115200);
  // Set time to wait for response strings to be found
  Serial.setTimeout(5000);

  //Open software serial for debugging - must use serial to usb (FTDI) adapter
  //dbgSerial.begin(19200); //can't be faster than 19200 for softserial
  dbgSerial.begin(19200);   //38400 softserial works fine for me

  dbgSerial.println("Weather Report");

  //test if the module is ready
  Serial.println("AT");
  delay(10000);

  if(Serial.find("OK"))
  {
    dbgSerial.println("Module is ready");
    delay(1000);
    //connect to the wifi
    boolean connected=false;
    for(int i=0;i<10;i++)
    {
      if(connectWiFi())
      {
        connected = true;
        break;
      }
    }
    if (!connected)
    {
      //die
      while(1);
    }

    delay(5000);
    //set the single connection mode
    Serial.println("AT+CIPMUX=0");
  }
  else
  {
    dbgSerial.println("Module didn't respond.");
    delay(100);

    //serial loop mode for diag
    while(1){
      while (dbgSerial.available()) {
        myChar = dbgSerial.read();
        Serial.print(myChar);
      }

      while (Serial.available()) {
        myChar = Serial.read();
        delay(25);
        dbgSerial.print(myChar);
      }
    }
  }
}

void loop()
{
  String cmd = "AT+CIPSTART=\"TCP\",\"";
  cmd += DST_IP;
  cmd += "\",80";
  Serial.println(cmd);
  dbgSerial.println(cmd);
  if(Serial.find("Error")) return;
  cmd = "GET /data/2.5/weather?id=";
  cmd += LOCATIONID;
  cmd += " HTTP/1.0\r\nHost: ";
  cmd += HOST;
  cmd += "\r\n\r\n";

  Serial.print("AT+CIPSEND=");
  Serial.println(cmd.length());
  if(Serial.find(">"))
  {
    dbgSerial.print(">");
  }
  else
  {
    Serial.println("AT+CIPCLOSE");
    dbgSerial.println("connect timeout");
    delay(1000);
    return;
  }

  Serial.print(cmd);
  int n = 1; // char counter
  unsigned int i = 0;
  StaticJsonBuffer<500> jsonBuffer;
  char json[500]="{";
  while (!Serial.find("\"main\":{")){} // find the part we are interested in.
  while (i<60000) {
    if(Serial.available()) {
      char c = Serial.read();
      json[n]=c;
      if(c=='}') break;
      n++;
      i=0;
    }
    i++;
  }
  dbgSerial.println(json);

  JSONObject& root = jsonBuffer.parseObject(json);
  double temp = root["temp"];
  double pressure = root["pressure"];
  double humidity = root["humidity"];
  temp -= 273.15; // from kelvin to degree f
  temp *= 1.8;
  temp += 32;
  tft.fillScreen();
//    void drawString(char const *string,uint16_t poX, uint16_t poY, uint8_t size=1, uint16_t fgcolor=WHITE, Font* font=NULL);
  tft.print("Temp: "
  tft.setCursor(2, 25);
  tft.setTextColor(BLUE);
  tft.setTextSize(2);
  tft.print("Temp: ");
  tft.print((int)temp);
  tft.print(".");
  tft.print((int)((temp-(int)temp)*10));
  tft.println(" F");
  tft.setCursor(2, 55);
  tft.setTextColor(GREEN);
  tft.setTextSize(2);
  tft.print("Press: ");
  tft.print((int)pressure);
  tft.setCursor(2, 85);
  tft.setTextColor(YELLOW);
  tft.setTextSize(2);
  tft.print("Humidity: ");
  tft.print((int)humidity);
  tft.println("%");
  dbgSerial.println(temp);
  dbgSerial.println(pressure);
  dbgSerial.println(humidity);
  dbgSerial.println("====");
  delay(600000);
}


boolean connectWiFi()
{
  Serial.println("AT+CWMODE=1");
  String cmd="AT+CWJAP=\"";
  cmd+=SSID;
  cmd+="\",\"";
  cmd+=PASS;
  cmd+="\"";
  dbgSerial.println(cmd);
  Serial.println(cmd);
  delay(2000);
  if(Serial.find("OK"))
  {
    dbgSerial.println("OK, Connected to WiFi.");
    return true;
  }
  else
  {
    dbgSerial.println("Can not connect to the WiFi.");
    return false;
  }
}
