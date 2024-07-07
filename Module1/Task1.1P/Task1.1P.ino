/* 
  Program for Arduino with pir motion sensor. When motion detected inbuilt LED lights up and outputs to serial
*/

// constants
int const MOTIONSENSORPIN = 3; // PIR motion sensor digital pin
long const SLEEP = 50; // delay for loop in milliseconds
String const ACTIVATEDMESSAGE = "sensor activated"; // output for when sensor is activated

// globals
int sensorState = 0; // default sensor state from motion sensor pin

/*
  Performs setup for sketch.
  - Begins serial output
  - Sets input pin for motion sensor
  - Sets build in led pin to output
*/
void setup() {
    Serial.begin(9600);
    Serial.println("Initialising setup.");
    pinMode(MOTIONSENSORPIN, INPUT);
    pinMode(LED_BUILTIN, OUTPUT);
    Serial.println("Setup complete.");
}

/*
  LOOP that runs every SLEEP milliseconds.
  If motion sensor detects movement state goes to HIGH
  and output to serial
*/
void loop() {
    // read 
    sensorState = digitalRead(MOTIONSENSORPIN);

    if (sensorState == HIGH) {
        digitalWrite(LED_BUILTIN, HIGH);
        Serial.println(ACTIVATEDMESSAGE);
    }
    else {
        digitalWrite(LED_BUILTIN, LOW);
    }
    delay(SLEEP);
}
