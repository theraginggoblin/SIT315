/* 
    Program for Arduino with pir motion sensor and ir sensor.
    Uses interrupts with pir motion sensor to switch inbuilt led state.
    Also uses interrupts with IR sensor to switch pin GREEN_LED_PIN state.
*/

// #include "PinChangeInterrupt.h"

// constants
int const WHITE_LED_PIN = 9; // attached to interrupt from motionsensor1
int const YELLOW_LED_PIN = 10; // attached to interrupt from motionsensor2
int const BLUE_LED_PIN = 11; // attached to interrupt from motionsensor3
int const GREEN_LED_PIN = 12 ;// attached to interrupt from ir sensor
// also LED_BUILTIN which is on pin 13. attached to interrupt from timer1
int const IR_SENSOR_INPUT_PIN = 2; 
int const MOTION_SENSOR1_PIN = 3;
int const MOTION_SENSOR2_PIN = 4; // requires pcint library for interrupts
int const MOTION_SENSOR3_PIN = 5; // requires pcint library for interrupts
//  int const COOLDOWN = 250; // minimum time in milliseconds between LED state changes
int const TIMER1_TICK_COUNT = 12500; // number of ticks to set for COMPA on timer 1


// globals
// volatile boolean builtinLEDState = LOW; // default led state
// volatile unsigned long builtinLEDStateChangeTime = 0; // time led last changed state in milliseconds since sketch start
// volatile boolean greenLEDState = LOW; // default led state
// volatile unsigned long greenLEDStateChangeTime = 0; // time led last changed state in milliseconds since sketch start

/*
    Switches led pin and prints to serial for debug output
*/
void switchLED(int ledPin) {
    int desired_state = !digitalRead(ledPin);
    digitalWrite(ledPin, desired_state);
    String message = "Led pin " + String(ledPin) + " set to " + String(desired_state);
    Serial.println(message);
}

/*
    This is the timer interrupt function for timer 1
    Switches built in led which is red
*/
ISR(TIMER1_COMPA_vect) {
  OCR1A += TIMER1_TICK_COUNT;
  switchLED(LED_BUILTIN);
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
  OCR1A = TIMER1_TICK_COUNT; // set COMPA Register
  TIMSK1 |= B00000010;  // enable COMPA Interrupt
}

/*

*/
void handleIRSensor() {
    switchLED(GREEN_LED_PIN);
}

/*
    
*/
void handleMotionSensor1() {
    switchLED(WHITE_LED_PIN);
}

/*
    
*/
void handleMotionSensor2() {
    switchLED(YELLOW_LED_PIN);
}

/*
    
*/
void handleMotionSensor3() {
    switchLED(BLUE_LED_PIN);
}

/*
    Performs setup for sketch.
    - Begins serial output
    - Sets input pin for motion sensor
    - Sets built in led pin to output
    - Configures interrupt for motion sensor pin
    - Sets green led pin to output
    - Sets IR sensor pin to input
    - Configures interrupt for ir sensor pin
*/
void setup() {
    Serial.begin(9600);
    Serial.println("Initialising setup.");

    // configure pinmodes for LEDs
    pinMode(WHITE_LED_PIN, OUTPUT);
    pinMode(YELLOW_LED_PIN, OUTPUT);
    pinMode(BLUE_LED_PIN, OUTPUT);
    pinMode(GREEN_LED_PIN, OUTPUT);
    pinMode(LED_BUILTIN, OUTPUT);

    // configure pinmodes for input sensors
    pinMode(IR_SENSOR_INPUT_PIN, INPUT);
    pinMode(MOTION_SENSOR1_PIN, INPUT);
    pinMode(MOTION_SENSOR2_PIN, INPUT_PULLUP); // pullup required for PCINT
    pinMode(MOTION_SENSOR3_PIN, INPUT_PULLUP); // pullup required for PCINT

    // normal interrupts for IR sensor and motion sensor 1
    attachInterrupt(digitalPinToInterrupt(IR_SENSOR_INPUT_PIN), handleIRSensor, RISING);
    attachInterrupt(digitalPinToInterrupt(MOTION_SENSOR1_PIN), handleMotionSensor1, RISING);

    // pcint interrupts
    attachPinChangeInterrupt(digitalPinToPinChangeInterrupt(MOTION_SENSOR2_PIN), handleMotionSensor2, FALLING);
    attachPinChangeInterrupt(digitalPinToPinChangeInterrupt(MOTION_SENSOR3_PIN), handleMotionSensor3, RISING);
    // attachPCINT(digitalPinToPCINT(MOTION_SENSOR2_PIN), handleMotionSensor2, RISING);
    // attachPCINT(digitalPinToPCINT(MOTION_SENSOR3_PIN), handleMotionSensor3, RISING);

    // setup timer 1
    setupTimerOne();
    Serial.println("Setup complete.");
}

/*
*/
void loop() {
    // nada
}
