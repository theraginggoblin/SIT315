
// define unsigned int constants identifying pins for button and led
const uint8_t BTN_PIN = 2;
const uint8_t LED_PIN = 13;

// globals. set initial set to LOW
uint8_t buttonPrevState = LOW;
uint8_t ledState = LOW;

// contains code that runs on start of Arduino
void setup()
{
  // set button pin as input pullup which connects internal resistor
  pinMode(BTN_PIN, INPUT_PULLUP);
  // set led pin to output
  pinMode(LED_PIN, OUTPUT);
  // initialise serial output at 9600 bits per second
  Serial.begin(9600);
}

// loop is main event loop that runs while arduino is powered, starts after setup
void loop()
{
  // set buttonState as whatever the state of BTN_PIN currently is
  uint8_t buttonState = digitalRead(BTN_PIN);
  
  // to serial output current button state, the previous button state
  // and current led state, then newline
  Serial.print(buttonState);
  Serial.print(buttonPrevState);
  Serial.print(ledState);
  Serial.println("");
  
  
  // if current button state not equal previous button state
  // set led state to not led state
  // then write led state to led pin
  if(buttonState != buttonPrevState)
  {
    ledState = !ledState;
    digitalWrite(LED_PIN, ledState);
  }
  
  buttonPrevState = buttonState;
    
  // sleep for 500 milliseconds
  delay(500);
}