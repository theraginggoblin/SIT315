/* 
    Program for Arduino with:
        3 PIR motion sensors
        1 infrared sensor
        Timer1 configured to trigger interrupt
        5 output LEDs (including built in)

    Utilises the library PinChangeInterrupt
    https://github.com/NicoHood/PinChangeInterrupt/tree/master

    Details:
        Infrared sensor: Normal interrupt. Triggers green LED
        Motionsensor1: Normal interrupt. Triggers white LED
        Motionsensor2: Pin change interrupt. Triggers yellow LED
        Motionsensor3: Pin change interrupt. Triggers blue LED
        Timer1: Triggers red (built in) LED every 500ms using interrupt
*/

#include "PinChangeInterrupt.h"

// constants
int const WHITE_LED_PIN = 7; // attached to interrupt from motionsensor1
int const YELLOW_LED_PIN = 8; // attached to interrupt from motionsensor2
int const BLUE_LED_PIN = 11; // attached to interrupt from motionsensor3
int const GREEN_LED_PIN = 12; // attached to interrupt from ir sensor
// also LED_BUILTIN which is on pin 13. attached to interrupt from timer1
int const IR_SENSOR_INPUT_PIN = 2; 
int const MOTION_SENSOR1_PIN = 3;
int const MOTION_SENSOR2_PIN = 4; // requires pcint library for interrupts
int const MOTION_SENSOR3_PIN = 5; // requires pcint library for interrupts
// GLOBAL cooldown is minimum time in milliseconds between LED state changes - global cooldown shared by all sensors
// exists because some using a sensor can trigger multiple interrupts
int const GLOBAL_COOLDOWN = 100;
int const TIMER1_TICK_COUNT = 31250; // number of ticks to set for COMPA on timer 1

// globals
volatile unsigned long lastLEDStateChangeTime = 0; // time led last changed state in milliseconds since sketch start

/*
    Checks cooldown (which is just 100ms now), if outside of cooldown
    switches led pin and prints to serial for debug output
*/
void switchLED(int ledPin) {
    if (millis() - GLOBAL_COOLDOWN >= lastLEDStateChangeTime) {
        lastLEDStateChangeTime = millis();
        int desired_state = !digitalRead(ledPin);
        digitalWrite(ledPin, desired_state);
        String message = "Led pin " + String(ledPin) + " set to " + String(desired_state);
        Serial.println(message);
    }
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

    pwm frequencies
    https://arduinoinfo.mywikis.net/wiki/Arduino-PWM-Frequency

    original settings I played with for 50ms timer - tinkercad animation takes longer than 50ms
    so ended up going for 500ms

    old 50ms calculations
    timer time out max = 64 (prescaler ratio) * 65536 (16bits) / 16000000 (system clock) = 262ms
    desired time out 50ms = 0.05s = 64 (prescaler ratio) * 12500 (tick count) / 16000000  (system clock)

    timer compare registers method
    50ms period = 12500 tick count
    means on setup and interrupt need to increase OCR1A += 12500


    500ms calculation
    timer time out max = 256 (prescaler ratio) * 65536 (16bits) / 16000000 (system clock) = 1048ms
    desired time out 500ms = 0.5s = 256 (prescaler ratio) * 31,250 (tick count) / 16000000  (system clock)

    timer compare registers method
    500ms period = 31250 tick count
    means on setup and interrupt need to increase OCR1A += 31250

    prescalers.
    B00000010 is 64:1
    B00000100 is 256:1
*/
void setupTimerOne() {
  TCCR1A = 0;           // initialise Timer1
  TCCR1B = 0;           // initilise Timer1
  TCCR1B |= B00000100;  // prescalar = 256:1
  OCR1A = TIMER1_TICK_COUNT; // set COMPA Register
  TIMSK1 |= B00000010;  // enable COMPA Interrupt
}

/*
    Function to trigger on interrupt.
    Triggers green LED to switch.
*/
void handleIRSensor() {
    switchLED(GREEN_LED_PIN);
}

/*
    Function to trigger on interrupt.
    Triggers white LED to switch.
*/
void handleMotionSensor1() {
    switchLED(WHITE_LED_PIN);
}

/*
    Function to trigger on interrupt.
    Triggers yellow LED to switch.
*/
void handleMotionSensor2() {
    switchLED(YELLOW_LED_PIN);
}

/*
    Function to trigger on interrupt.
    Triggers blue LED to switch.
*/
void handleMotionSensor3() {
    switchLED(BLUE_LED_PIN);
}

/*
    Performs setup for sketch.
    - Begins serial output
    - Sets pin modes for all LEDs
    - Sets pin modes for input sensors
    - Attachs interrupts to the input sensors
    - Calls setupTimerOne to configure Timer1 with interrupt
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
    attachPinChangeInterrupt(digitalPinToPinChangeInterrupt(MOTION_SENSOR2_PIN), handleMotionSensor2, RISING);
    attachPinChangeInterrupt(digitalPinToPinChangeInterrupt(MOTION_SENSOR3_PIN), handleMotionSensor3, RISING);

    // setup timer 1
    setupTimerOne();
    Serial.println("Setup complete.");
}

/*
    Nada
*/
void loop() {
    // nada
}
