/* 
    Program for Arduino with pir motion sensor and ir sensor.
    Uses interrupts with pir motion sensor to switch inbuilt led state.
    Also uses interrupts with IR sensor to switch pin GREEN_LED_PIN state.
*/

// constants
int const MOTION_SENSOR_PIN = 3; // PIR motion sensor digital pin
int const BUILTIN_LED_COOLDOWN = 1000; // minimum time in milliseconds between LED state changes
int const GREEN_LED_PIN = 11; // pin for green led
int const GREEN_LED_COOLDOWN = 250; // minimum time in milliseconds between LED state changes
int const IR_SENSOR_INPUT_PIN = 2; // pin for ir sensor

// globals
volatile boolean builtinLEDState = LOW; // default led state
volatile unsigned long builtinLEDStateChangeTime = 0; // time led last changed state in milliseconds since sketch start
volatile boolean greenLEDState = LOW; // default led state
volatile unsigned long greenLEDStateChangeTime = 0; // time led last changed state in milliseconds since sketch start

/*
    Switches led pin to desired state and prints to serial for debug output
*/
void switchLED(int ledPin, int desired_state) {
    String message = "Led pin " + String(ledPin) + " set to " + String(desired_state);
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
    Function called on interrupt
    - Activates green LED and creates prints to serial if cooldown period passed.
    - Switches LED state between HIGH and LOW
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

    // configure pinmodes for motion sensor connecting to builtin led
    pinMode(MOTION_SENSOR_PIN, INPUT);
    pinMode(LED_BUILTIN, OUTPUT);
    builtinLEDStateChangeTime = millis();
    // set motion sensor pin for interrupt - calls handleBuiltinLED on RISING
    attachInterrupt(digitalPinToInterrupt(MOTION_SENSOR_PIN), handleBuiltinLED, RISING);

    // setup everything for switching of green LED with ir sensor
    pinMode(GREEN_LED_PIN, OUTPUT);
    pinMode(IR_SENSOR_INPUT_PIN, INPUT);
    greenLEDStateChangeTime = millis();
    attachInterrupt(digitalPinToInterrupt(IR_SENSOR_INPUT_PIN), handleGreenLED, RISING);

    Serial.println("Setup complete.");
}

/*
*/
void loop() {
    // nada
}
