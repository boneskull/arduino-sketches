

/* Started with Basic MQTT example */
#include <aJson.h>
#include <ESP8266WiFi.h>      //ESP library from http://github.com/esp8266/Arduino
#include <MQTT.h>
#include <PubSubClient.h>      // MQTT library from http://github.com/Imroy/pubsubclient
#include <DHT.h>      // DHT library from http://github.com/adafruit/DHT-sensor-library
// Written by ladyada, public domain
#include <Wire.h>

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

void setup() {

  // Setup console
  Serial.begin(115200);
  delay(10);
  dht.begin();
  WiFi.begin(ssid, pass);
  Serial.println("Connecting to WiFi");
  while (WiFi.status() != WL_CONNECTED) {
    delay(250);
  }
  Serial.println("WiFi connected");
  Serial.println("Connecting to MQTT server");
  while(!client.connect("thermostat")) {
    delay(250);
  }
  Serial.println("MQTT connected");
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

    Serial.print("DTH sensor read F ");
    Serial.println(f);
    Serial.print("DTH sensor read H ");
    Serial.println(h);

    temps[tempIdx] = f;

    if (tempIdx + 1 == VALUE) {
      float temp;
      Serial.print("Taking average temp");
      for (int i = 0; i < VALUES; i++) {
        temp += temps[i];
      }
      temp /= VALUES;
      Serial.print("Temperature: ");
      Serial.println(temp);
      Serial.print("Humidity: ");
      Serial.println(h);
      client.publish("digs/home/temperature", String(temp));
      client.publish("digs/home/humidity", String(h));
      tempIdx = 0;
    }
    previousMillis = currentMillis;
    tempIdx++;
  }
}
