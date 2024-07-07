/* 
    Program for Arduino with pir motion sensor. Uses interrupts with pir motion sensor to switch inbuilt
    led state.
*/

// constants
int const MOTIONSENSORPIN = 3; // PIR motion sensor digital pin
String const ACTIVATEDMESSAGE = "led activated"; // output for when led is activated
String const DEACTIVATEDMESSAGE = "led deactivated"; // output for when led is deactivated
int const LEDCOOLDOWN = 1000; // minimum time in milliseconds between LED state changes

// globals
volatile int ledState = LOW; // default led state
volatile unsigned long ledStateChangeTime = 0; // time led last changed state in milliseconds since sketch start


/*
    Turns on built in LED and prints output to serial for debugging purposes.
*/
void turnOnLED() {
    digitalWrite(LED_BUILTIN, HIGH);
    Serial.println(ACTIVATEDMESSAGE);
}

/*
    Turns off built in LED and prints output to serial for debugging purposes.
*/
void turnoffLED() {
    digitalWrite(LED_BUILTIN, LOW);
    Serial.println(DEACTIVATEDMESSAGE);
}

/*
    Function called on interrupt
    - Activates built in LED and creates prints to serial if cooldown period passed.
    - Switches LED state between HIGH and LOW
*/
void handleActivation() {
    if (millis() - LEDCOOLDOWN >=  ledStateChangeTime) {
        if (ledState == LOW) {
            turnOnLED();
            ledState = HIGH;
        }
        else {
            turnoffLED();
            ledState = LOW;
        }
    }
    
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
    pinMode(MOTIONSENSORPIN, INPUT);
    pinMode(LED_BUILTIN, OUTPUT);
    ledStateChangeTime = millis(); // initial state change of led - off
    // set motion sensor pin for interrupt - calls handleActivation on RISING
    attachInterrupt(digitalPinToInterrupt(MOTIONSENSORPIN), handleActivation, RISING);
    Serial.println("Setup complete.");
}

/*
*/
void loop() {
    // nada
}
