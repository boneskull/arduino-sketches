#include <Servo.h> // Include the Servo library
#include <NewPing.h> // Include the NewPing library

#define SERVO_DATA_PIN 3 // Create a variable for the Servo Data pin
#define ECHO_PIN 5 // Create a variable for the Ultrasonic Echo pin
#define TRIG_PIN 6 // Create a variable for the Ultrasonic Trigger pin
#define MAX_DISTANCE 400 // Create a variable for the Max Distance (in cm) that the Ultrasonic Sensor looks for
#define testDistance 20 // Create a variable for the amount (in degrees) that the Servo will travel to test the distance 
#define testDelay 100 // A variable that sets the delay for certain processes in the testSurroundings function
#define WIGGLE_ROOM 25 // percentage chagne in sensor reading to cause it to move

NewPing sonar(TRIG_PIN, ECHO_PIN, MAX_DISTANCE); // Create a Sonar object for the Ultrasonic Sensor
Servo sonicservo; // Create a Servo object called sonicservo for the servo

int pos = 90;
int currentReading;
int lastReading = MAX_DISTANCE;

void setup() { // Start the setup function
  Serial.begin(115200); // Initialize a serial connection with the computer
  sonicservo.attach(SERVO_DATA_PIN); // Attach the sonicservo object to the servo data pin on the Arduino
  sonicservo.write(pos); // Center the servo
} // End the setup function

void loop() { // Start the loop function
  delay(30); // Pause the program for 30 ms to wait to receive the next position
  currentReading = sonar.ping() / US_ROUNDTRIP_CM;
  float pct = (abs(lastReading - currentReading)) / currentReading * 100;
  if (pct >= WIGGLE_ROOM) {
    Serial.print("Significant diff: ");
    Serial.print(pct);
    Serial.println("%");
    findClosestObject();
  } else {
    resetToCurrent();
  }
}

void resetToCurrent() {
  lastReading = currentReading;
  if (pos < 0) {
    pos = 0;
  } else if (pos > 180) {
    pos = 180;
  } else {
    sonicservo.write(pos);
  }
}

void findClosestObject() {

  sonicservo.write(pos - testDistance); // Set the servo to it's current position minus the distance that has been set for it to test
  delay(testDelay); // Let the servo compute the line above
  int leftReading = sonar.ping() / US_ROUNDTRIP_CM; // Take an ultrasonic reading of it's current position
  delay(testDelay / 2); // Wait for the ultrasonic sensor
  sonicservo.write(pos + testDistance); // Set the servo to it's current position plus the distance that has been set for it to test
  delay(testDelay); //Let the servo compute the line above
  int rightReading = sonar.ping() / US_ROUNDTRIP_CM; // Take an ultrasonic reading of it's current position
  delay(testDelay / 2); // Wait for the ultrasonic sensor

  int minReading = min(rightReading, leftReading);
  Serial.print("Min distance (left/right): ");
  Serial.println(minReading);
  if (minReading < currentReading) {
    if (minReading == rightReading) {
      Serial.println("Right is closest; staying put");
      pos += testDistance;
      lastReading = rightReading;
    } else {
      Serial.println("Left is closest; moving back");
      pos -= testDistance;
      lastReading = leftReading;
      sonicservo.write(pos);
    }
  } else {
    Serial.println("Current is still closest; resetting");
    resetToCurrent();
  }
}

