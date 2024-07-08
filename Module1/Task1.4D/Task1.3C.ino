/* 
    Program for Arduino with pir motion sensor.
    Uses interrupts with pir motion sensor to switch inbuilt led state.
    Also uses interrupts and Timer1 with ultrasonic sensor to switch pin GREEN_LED_PIN state.
*/

// constants
int const MOTION_SENSOR_PIN = 3; // PIR motion sensor digital pin
int const BUILTIN_LED_COOLDOWN = 1000; // minimum time in milliseconds between LED state changes
int const TIMER1_TICK_COUNT = 12500; // number of ticks to set for COMPA on timer 1
int const GREEN_LED_PIN = 11; // pin for green led
int const GREEN_LED_COOLDOWN = 250; // minimum time in milliseconds between LED state changes
int const DISTANCE_SENSOR_INPUT_PIN = 2; // pin for ultrasonic distance sensor
int const DISTANCE_SENSOR_OUTPUT_PIN = 4; // pin for ultrasonic distance sensor

// globals
volatile boolean builtinLEDState = LOW; // default led state
volatile unsigned long builtinLEDStateChangeTime = 0; // time led last changed state in milliseconds since sketch start
volatile boolean greenLEDState = LOW; // default led state
volatile unsigned long greenLEDStateChangeTime = 0; // time led last changed state in milliseconds since sketch start
volatile unsigned long echoStartTime = 0; // ultrasonic distance sensor echoes


/*

*/
void sendEcho(){
    // Sets the trigger on HIGH state for 10 micro seconds to send a series of pulses
    digitalWrite(DISTANCE_SENSOR_OUTPUT_PIN, HIGH);
    // blocks 10 microseconds from the interrupt, I think we'll live :)
    delayMicroseconds(10);
    // disable the sending again so we can wait for a response
    digitalWrite(DISTANCE_SENSOR_OUTPUT_PIN, LOW);
    // record the send time
    echoStartTime = micros();
}

/*

*/
void handleEchoReceive(){
    // don't do anything if no pulse has been sent
    if(echoStartTime != 0){
        // calculate the distance by measuring how long it took to return the sound
        // The speed of sound is 343 m/s and we need half the time it took (since
        // the sound has to travel towards the object _AND_ back). So a single echo does
        // 1/(343/2) = 0.005831 seconds per meter
        String distance = String((micros() - echoStartTime) / 58);
        Serial.println(distance);
        echoStartTime = 0; // reset start time
    }
}

/*
    Switches led pin to desired state and prints to serial for debug output
*/
void switchLED(int ledPin, int desired_state) {
    String message = "Led pin " + String(ledPin) + "set to " + String(desired_state);
    digitalWrite(ledPin, desired_state);
    Serial.println(message);
}

/*
    Function called on interrupt
    - Activates built in LED and creates prints to serial if cooldown period passed.
    - Switches LED state between HIGH and LOW
*/
void handleBuiltinLED() {
    if (millis() - BUILTIN_LED_COOLDOWN >=  builtinLEDStateChangeTime) {
        if (builtinLEDState == LOW) {
            switchLED(LED_BUILTIN, HIGH);
            builtinLEDState = HIGH;
            builtinLEDStateChangeTime = millis();
        }
        else {
            switchLED(LED_BUILTIN, LOW);
            builtinLEDState = LOW;
            builtinLEDStateChangeTime = millis();
        }
    }
}

/*
*/
void handleGreenLED() {
    if (millis() - GREEN_LED_COOLDOWN >=  greenLEDStateChangeTime) {
        if (greenLEDState == LOW) {
            switchLED(GREEN_LED_PIN, HIGH);
            greenLEDState = HIGH;
            greenLEDStateChangeTime = millis();
        }
        else {
            switchLED(GREEN_LED_PIN, LOW);
            greenLEDState = LOW;
            greenLEDStateChangeTime = millis();
        }
    }
    
}

/*
    This is the timer interrupt function for timer 1
*/
ISR(TIMER1_COMPA_vect) {
  OCR1A += TIMER1_TICK_COUNT;
  handleGreenLED();
}


/*
    Configures timer1
    timer time out max = 64 (prescaler ratio) * 65536 (16bits) / 16000000 (system clock) = 262ms
    desired time out 50ms = 64 (prescaler ratio) * 12500 (tick count) / 16000000  (system clock)

    timer compare registers method
    50ms period = 12500 tick count
    means on setup and interrupt need to increase OCR1A += 12500
*/
void setupTimerOne() {
  TCCR1A = 0;           // initialise Timer1
  TCCR1B = 0;           // initilise Timer1
  TCCR1B |= B00000011;  // prescalar = 64:1
  OCR1A = 12500;        // set COMPA Register
  TIMSK1 |= B00000010;  // enable COMPA Interrupt
}

/*
    Performs setup for sketch.
    - Begins serial output
    - Sets input pin for motion sensor
    - Sets build in led pin to output
    - Configures interrupt for motion sensor pin
*/
void setup() {
    Serial.begin(9600);
    Serial.println("Initialising setup.");

    // configure pinmodes for motion sensor connecting to builtin led
    pinMode(MOTION_SENSOR_PIN, INPUT);
    pinMode(LED_BUILTIN, OUTPUT);
    builtinLEDStateChangeTime = millis();
    // set motion sensor pin for interrupt - calls handleBuiltinLED on RISING
    attachInterrupt(digitalPinToInterrupt(MOTION_SENSOR_PIN), handleBuiltinLED, RISING);

    // setup everything for Timer1 interrupts and its switching of green LED with 
    // ultrasonic sensor
    pinMode(GREEN_LED_PIN, OUTPUT);
    greenLEDStateChangeTime = millis();
    setupTimerOne();
    attachInterrupt(digitalPinToInterrupt(ULTRASONIC_PIN_INPUT), handleEchoReceive, RISING);

    Serial.println("Setup complete.");
}

/*
*/
void loop() {
    // nada
}
