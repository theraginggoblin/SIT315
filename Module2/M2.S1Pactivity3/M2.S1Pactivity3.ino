/*
Program that blinks LED
Includes a potentiometer to change rate at which LED blinks
Defaults to blinking every two seconds but this is pointless as potentiometer reading will be used instead.

I potentially deviated a lot from what the activity asked as the requirements were vague.
It said make the LED blink every 2 seconds - then allow timer frequency to be updated using potentiometer.
The acitivity did not mention how the blinking LED and timer are related.
Could have had the timer doing nothing (but the interrupt for timer was set to change led state?) and led blinked in loop every 2000ms
Anyway....

How this works.

If potentiometer in the most counter clockwise position doesn't change LED state
If potentiometer in the most clockwise position changes state every 4194ms
Just under the halfway mark is blinking every 2 seconds
So faster is counter clockwise and slower is clockwise

*/

const byte LED_PIN = 13;
const byte METER_PIN = A4;

const int BLINK_TWO_SECONDS_TICKS = 31252;
const int SIXTEENBITS = 65536;

volatile unsigned long lastLEDFrequency = 0; 

/*
Setup PINS
Setup timer1 for interrupts, initially to blink every 2 seconds

*/
void setup() {
    pinMode(LED_PIN, OUTPUT);
    pinMode(METER_PIN, INPUT);
    Serial.begin(9600);
    setupTimer();
    startTimer(BLINK_TWO_SECONDS_TICKS);
}


/*
Loops every 1 second
Reads analog pin
Adds 1 to reading so its 1 to 1024 then times it by 64 giving a range of 1024 to 65536
then passes that to timer to update LED state change frequency
*/
void loop() {
    // take a new reading every second
    delay(1000);
    long potentiometerReading = analogRead(METER_PIN);
    Serial.println("potentiometer says: " + String(potentiometerReading));
    
    // num of ticks to reach before switching, 1024 to 65536
    double ticks = ((potentiometerReading + 1) * 64);
    Serial.println("ticks: " + String(ticks));
    startTimer(ticks);
}


/*
Everything required to setup overflow compare timer interrupts on timer1
except setting the overflow ticks
*/
void setupTimer() {
  TCCR1A = 0;           // timer1 register A to 0
  TCCR1B = 0;           // timer1 register B to 0
  TCCR1B |= B00000101;  // prescalar = 1024:1
  TIMSK1 |= B00000010;  // enable COMPA Interrupt for timer 1
}

/*
    Configures timer1

    pwm frequencies
    https://arduinoinfo.mywikis.net/wiki/Arduino-PWM-Frequency

    prescalers.
    B00000010 is 64:1
    B00000100 is 256:1
    B00000101 is 1024:1

    timer compare registers method

    require LED blinking each 2 seconds so interrupts have to be able to fire every 2000 ms
    already know prescalar of 256:1 on timer1 allows max of just over a second so need to change prescalar
  
    calculations.
  
    time out max = 1024 (prescalar ratio) * 65536 (16 bits) /  16000000 (system clock) = 4194ms
    so 1024:1 prescalar is what is required

    led state change in ms = 1024 (prescaler ratio) * <ticks> / 16000000  (system clock)

    tick count maxs out at 65536
    max overflow count is 65536 (4194 ms)
    the potentiometer analog read is 0-1023
    read of 1023 should max out led blink duration (arbitrary decision by me)
    read of 0 should not switch led state (another artitrary decision by me)
    to make the numbers easier to deal with using multiplication and division will add + 1 so range is 1-1024
    65536 / 1024 = 64
    so analog read can be multiplied by 64.
*/
void startTimer(double frequency){
  noInterrupts();
  if (lastLEDFrequency != frequency) {
    Serial.println("changing frequency");
    lastLEDFrequency = frequency;
    TCNT1 = 0; // reset timer back to 0
    OCR1A = frequency; // set COMPA Register - adjust this to adjust frequency - is ticks
  }
  interrupts();
}

/*
Interrupt function for timer1 on A
Does not change LED state if miniumum frequency
*/
ISR(TIMER1_COMPA_vect){
  // if potentiometer is all the way to left don't change state
  if (lastLEDFrequency > 1024) {
    TCNT1 = 0; // reset timer back to 0
    digitalWrite(LED_PIN, digitalRead(LED_PIN) ^ 1);
  }
}
