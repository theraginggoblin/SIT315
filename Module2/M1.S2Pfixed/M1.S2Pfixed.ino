
// define unsigned int constants identifying pins for button and led
const uint8_t BTN_PIN = 2;
const uint8_t LED_PIN = 13;

void switchLED() {
  int desired_state = !digitalRead(LED_PIN);
  digitalWrite(LED_PIN, desired_state);
  String message = "Led pin " + String(LED_PIN) + " set to " + String(desired_state);
  Serial.println(message);
}

// contains code that runs on start of Arduino
void setup() {
  // set button pin as input pullup which connects internal resistor
  pinMode(BTN_PIN, INPUT_PULLUP);
  // set led pin to output
  pinMode(LED_PIN, OUTPUT);
  // initialise serial output at 9600 bits per second
  Serial.begin(9600);

  // attach interrupt to BTN_PIN which calls switchLED
  attachInterrupt(digitalPinToInterrupt(BTN_PIN), switchLED, RISING);
}

// loop is main event loop that runs while arduino is powered, starts after setup
void loop() {
  // pass
}