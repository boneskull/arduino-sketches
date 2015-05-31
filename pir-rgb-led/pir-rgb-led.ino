
#define PAUSE 2000
#define LED_PAUSE 5
#define PIR_PIN 7
#define RED_PIN 9
#define GREEN_PIN 10
#define BLUE_PIN 11

//the time we give the sensor to calibrate (10-60 secs according to the datasheet)
int calibrationTime = 30;

//the time when the sensor outputs a low impulse
long unsigned int lowIn;

unsigned long prevMs = 0;
unsigned long prevLedMs = 0;

boolean lockLow = true;
boolean takeLowTime;
boolean cycling = false;

unsigned int decColor;
unsigned int incColor;
unsigned int rgbColor[3];

boolean motion = false;

void cycle() {

  if (decColor == 3) {
    decColor = 0;
  }

  if (incColor == 3) {
    incColor = 0;
  }

  rgbColor[decColor]--;
  rgbColor[incColor]++;

  setColorRgb(rgbColor[0], rgbColor[1], rgbColor[2]);

  if (rgbColor[decColor] == 0) {
    decColor++;
  }

  if (rgbColor[incColor] == 255) {
    incColor++;
  }

}

void off() {
  setColorRgb(0, 0, 0);

  // Start off with red.
  rgbColor[0] = 255;
  rgbColor[1] = 0;
  rgbColor[2] = 0;

  decColor = 0;
  incColor = 1;
}

void setColorRgb(unsigned int red, unsigned int green, unsigned int blue) {
  analogWrite(RED_PIN, red);
  analogWrite(GREEN_PIN, green);
  analogWrite(BLUE_PIN, blue);
}


/////////////////////////////
//SETUP
void setup() {

  Serial.begin(9600);
  pinMode(PIR_PIN, INPUT);
  pinMode(RED_PIN, OUTPUT);
  pinMode(GREEN_PIN, OUTPUT);
  pinMode(BLUE_PIN, OUTPUT);

  off();
  digitalWrite(PIR_PIN, LOW);

  //give the sensor some time to calibrate
  Serial.print("calibrating sensor ");
  for (int i = 0; i < calibrationTime; i++) {
    Serial.print(".");
    delay(1000);
  }
  Serial.println(" done");
  Serial.println("SENSOR ACTIVE");
}

////////////////////////////
//LOOP
void loop() {
  unsigned long ms = millis();
  if (motion && ms - prevLedMs >= LED_PAUSE) {
    prevLedMs = ms;
    cycle();
  } else if (!motion) {
    off();
  }
  if (ms - prevMs >= PAUSE) {
    prevMs = ms;
    if (digitalRead(PIR_PIN) == HIGH) {
      motion = true;
      if (lockLow) {
        //makes sure we wait for a transition to LOW before any further output is made:
        lockLow = false;
        Serial.println("---");
        Serial.print("motion detected at ");
        Serial.print(millis() / 1000);
        Serial.println(" sec");

      }
      takeLowTime = true;
    }

    if (digitalRead(PIR_PIN) == LOW) {
      if (takeLowTime) {
        lowIn = millis();          //save the time of the transition from high to LOW
        takeLowTime = false;       //make sure this is only done at the start of a LOW phase
      }
      //if the sensor is low for more than the given pause,
      //we assume that no more motion is going to happen
      if (!lockLow && millis() - lowIn > PAUSE) {
        //makes sure this block of code is only executed again after
        //a new motion sequence has been detected
        lockLow = true;
        motion = false;
        Serial.print("motion ended at ");      //output
        Serial.print((millis() - PAUSE) / 1000);
        Serial.println(" sec");
      }
    }
  }
}
