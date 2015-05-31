

/* Started with Basic MQTT example */

#include <ESP8266WiFi.h>      //ESP library from http://github.com/esp8266/Arduino
#include <MQTT.h>
#include <PubSubClient.h>      // MQTT library from http://github.com/Imroy/pubsubclient
#include <DHT.h>      // DHT library from http://github.com/adafruit/DHT-sensor-library
// Written by ladyada, public domain
#include <Wire.h>

#include <Adafruit_LEDBackpack.h>
#include <Adafruit_GFX.h>

#define DHTPIN 2     // what pin we're connected to
#define VALUES 4
// Uncomment whatever type you're using!
#define DHTTYPE DHT11   // DHT 11
//#define DHTTYPE DHT22
//#define DHTTYPE DHT21   // DHT 21 (AM2301)

// Initialize DHT sensor for normal 16mhz Arduino
//DHT dht(DHTPIN, DHTTYPE);
// NOTE: For working with a faster chip, like an Arduino Due or Teensy, you
// might need to increase the threshold for cycle counts considered a 1 or 0.
// You can do this by passing a 3rd parameter for this threshold.  It's a bit
// of fiddling to find the right value, but in general the faster the CPU the
// higher the value.  The default for a 16mhz AVR is a value of 6.  For an
// Arduino Due that runs at 84mhz a value of 30 works.
// Example to initialize DHT sensor for ESP8266:
DHT dht(DHTPIN, DHTTYPE, 20);



const char *ssid =   "BRIDE OF ZOLTAR";      // cannot be longer than 32 characters!
const char *pass =   "xxxx";      //
long previousMillis = 0;      // Timer loop from http://www.arduino.cc/en/Tutorial/BlinkWithoutDelay
long interval = 5000;        //
IPAddress server(10, 0, 0, 5);    // Update these with values suitable for your network.
PubSubClient client(server);
Adafruit_7segment matrix = Adafruit_7segment();

void setup() {

  matrix.begin(0x70);
  matrix.clear();
  matrix.writeDisplay();
  // Setup console
  Serial.begin(115200);
  delay(10);
  Serial.println();
  Serial.println();
  dht.begin();
  WiFi.begin(ssid, pass);
  int tries = 0;
  while (WiFi.status() != WL_CONNECTED) {
    matrix.drawColon(true);
    matrix.writeDisplay();
    delay(250);
    matrix.drawColon(false);
    matrix.writeDisplay();
    delay(750);
  }
  Serial.println("WiFi connected");

}

float temps[VALUES];
int tempIdx = 0;

void loop() {
  client.loop();
  unsigned long currentMillis = millis();
  if (currentMillis - previousMillis > interval) {
    // Reading temperature or humidity takes about 250 milliseconds!
    // Sensor readings may also be up to 2 seconds 'old' (its a very slow sensor)
    float h = dht.readHumidity();
    // Read temperature as Celsius
    //float t = dht.readTemperature();
    // Read temperature as Fahrenheit
    float f = dht.readTemperature(true);
    // Check if any reads failed and exit early (to try again).
    if (isnan(h) || isnan(f)) {
      Serial.println("Failed to read from DHT sensor!");
      return;
    }

    Serial.print("DTH sensor read ");
    Serial.println(f);
    temps[tempIdx] = f;


    if (tempIdx == VALUES - 1) {
      float temp;
      Serial.print("Taking average temp");
      for (int i = 0; i < VALUES; i++) {
        temp += temps[i];
      }
      temp /= VALUES;
      Serial.println(temp);
      client.publish("m_bed/temperature", String(temp));
      client.publish("m_bed/humidity", String(h));
      matrix.print((int)temp);
      matrix.writeDisplay();
      tempIdx = 0;
    }
    previousMillis = currentMillis;
    tempIdx++;
  }
}
