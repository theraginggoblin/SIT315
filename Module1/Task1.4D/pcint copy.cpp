// copied and pasted code to copy over to tinkercad as it doesn't have the library supported

/*
Copyright (c) 2014-2021 NicoHood
See the readme for credit to other people.

Permission is hereby granted, free of charge, to any person obtaining a copy
of this software and associated documentation files (the "Software"), to deal
in the Software without restriction, including without limitation the rights
to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
copies of the Software, and to permit persons to whom the Software is
furnished to do so, subject to the following conditions:

The above copyright notice and this permission notice shall be included in
all copies or substantial portions of the Software.

THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN
THE SOFTWARE.
*/

// include guard
#pragma once

//================================================================================
// General Settings
//================================================================================

/* Settings to de/activate ports/pins
This will save you flash and ram because the arrays
are managed dynamically with the definitions below.
Make sure you still have all needed ports activated.
Each deactivated port saves 3 bytes of ram.
If you deactivate the whole port,
you dont need to deactivate the pins.
Same for the port if you deactivate all 8 pins.
You dont have to deactivate pins/ports that dont exist.
That is done by the macros. */

#ifndef PCINT_ENABLE_MANUAL

#define PCINT_ENABLE_PORT0
#define PCINT_ENABLE_PORT1
#define PCINT_ENABLE_PORT2
#define PCINT_ENABLE_PORT3

#define PCINT_ENABLE_PCINT0
#define PCINT_ENABLE_PCINT1
#define PCINT_ENABLE_PCINT2
#define PCINT_ENABLE_PCINT3
#define PCINT_ENABLE_PCINT4
#define PCINT_ENABLE_PCINT5
#define PCINT_ENABLE_PCINT6
#define PCINT_ENABLE_PCINT7
#define PCINT_ENABLE_PCINT8
#define PCINT_ENABLE_PCINT9
#define PCINT_ENABLE_PCINT10
#define PCINT_ENABLE_PCINT11
#define PCINT_ENABLE_PCINT12
#define PCINT_ENABLE_PCINT13
#define PCINT_ENABLE_PCINT14
#define PCINT_ENABLE_PCINT15
#define PCINT_ENABLE_PCINT16
#define PCINT_ENABLE_PCINT17
#define PCINT_ENABLE_PCINT18
#define PCINT_ENABLE_PCINT19
#define PCINT_ENABLE_PCINT20
#define PCINT_ENABLE_PCINT21
#define PCINT_ENABLE_PCINT22
#define PCINT_ENABLE_PCINT23
#define PCINT_ENABLE_PCINT24
#define PCINT_ENABLE_PCINT25
#define PCINT_ENABLE_PCINT26
#define PCINT_ENABLE_PCINT27
#define PCINT_ENABLE_PCINT28
#define PCINT_ENABLE_PCINT29
#define PCINT_ENABLE_PCINT30
#define PCINT_ENABLE_PCINT31

#endif

#ifdef ARDUINO
// use API with function pointers (better optimized with .a linkage)
#define PCINT_API

// is the library compiled via .a file?
// see readme for more information
#define PCINT_ALINKAGE

// force compile all enabled port ISRs (with .a linkage)
//#define PCINT_COMPILE_ENABLED_ISR

#endif

//================================================================================
// Suggested Settings
//================================================================================

// Arduino Uno (328)
#if defined(__AVR_ATmega328__) || defined(__AVR_ATmega328P__) || defined(__AVR_ATmega168__) || defined(__AVR_ATmega88__)
/* Reordering interrupt callbacks priority
Port0 has SPI on higher pins, ordering is fine
Port1 has I2C on higher pins, ordering is fine
Port2 has USART and Pin Interrupt on lower pins,
move the priority down
Its more likely the user will use pin 4-7
*/
#if !defined(PCINT_CALLBACK_PORT2)
#define PCINT_CALLBACK_PORT2 \
PCINT_CALLBACK(4, 20); \
PCINT_CALLBACK(5, 21); \
PCINT_CALLBACK(6, 22); \
PCINT_CALLBACK(7, 23); \
PCINT_CALLBACK(0, 16); /* USART RX */ \
PCINT_CALLBACK(1, 17); /* USART TX */ \
PCINT_CALLBACK(2, 18); /* Pin Interrupt 0 */ \
PCINT_CALLBACK(3, 19); /* Pin Interrupt 1 */
#endif

// deactivate crystal and reset pins by default
#if defined(PCINT_ENABLE_PCINT6)
#undef PCINT_ENABLE_PCINT6 // crystal
#endif
#if defined(PCINT_ENABLE_PCINT7)
#undef PCINT_ENABLE_PCINT7 // crystal
#endif
#if defined(PCINT_ENABLE_PCINT14)
#undef PCINT_ENABLE_PCINT14 // reset
#endif
#endif

// Arduino Mega (2560)
#if defined(ARDUINO_AVR_MEGA2560) || defined(ARDUINO_AVR_MEGA)
/* Port1 is structured a bit more complicated
Also only 3 pins are connected on standard boards
Seeeduino Mega has these pins optional!
Disabling Port1 gives more speed and uses less flash
Pins: 0(RX0), 14(TX3), 15(RX3) */
#if defined(PCINT_ENABLE_PORT1)
#undef PCINT_ENABLE_PORT1 // better performence
#endif

/* Reordering interrupt callbacks priority
Port2 has SPI on lower pins, move the priority down
Its more likely the user will use pin 10-13
Port1 by default deactivated, ordering is fine
Port2 only has ADCs, ordering is fine
*/
#if !defined(PCINT_CALLBACK_PORT0)
#define PCINT_CALLBACK_PORT0 \
PCINT_CALLBACK(4, 4); \
PCINT_CALLBACK(5, 5); \
PCINT_CALLBACK(6, 6); \
PCINT_CALLBACK(7, 7); \
PCINT_CALLBACK(0, 0); /* SPI SS */ \
PCINT_CALLBACK(1, 1); /* SPI SCK */ \
PCINT_CALLBACK(2, 2); /* SPI MISO */ \
PCINT_CALLBACK(3, 3); /* SPI MOSI */
#endif
#endif

// Arduino Leonardo/Micro (32u4)
#if defined(__AVR_ATmega32U4__) || defined(__AVR_ATmega16U4__)
/* Reordering interrupt callbacks priority
Port0 has SPI on lower pins, move the priority down
Its more likely the user will use pin 8-11 */
#if !defined(PCINT_CALLBACK_PORT0)
#define PCINT_CALLBACK_PORT0 \
PCINT_CALLBACK(4, 4); \
PCINT_CALLBACK(5, 5); \
PCINT_CALLBACK(6, 6); \
PCINT_CALLBACK(7, 7); \
PCINT_CALLBACK(0, 0); /* SPI SS / RX LED */ \
PCINT_CALLBACK(1, 1); /* SPI SCK */ \
PCINT_CALLBACK(2, 2); /* SPI MISO */ \
PCINT_CALLBACK(3, 3); /* SPI MOSI */
#endif

// RX LED on normal leonardo/micro
#if defined(PCINT_ENABLE_PCINT0) && (defined(ARDUINO_AVR_LEONARDO) || defined(ARDUINO_AVR_MICRO))
#undef PCINT_ENABLE_PCINT0
#endif
#endif

// Hoodloader2 (u2 Series)
#if defined(__AVR_AT90USB82__) || defined(__AVR_AT90USB162__) || defined(__AVR_ATmega32U2__) || defined(__AVR_ATmega16U2__) || defined(__AVR_ATmega8U2__)
#if defined(ARDUINO_HOODLOADER2)
// on HoodLoader2 Arduino boards only PB1-7 (port0) is broken out, save this flash
#if defined(PCINT_ENABLE_PORT1)
#undef PCINT_ENABLE_PORT1
#endif

// SS (PB0) is not connected on normal Arduino boards
#if defined(PCINT_ENABLE_PCINT0)
#undef PCINT_ENABLE_PCINT0
#endif

/* Reordering interrupt callbacks priority
Port0 has SPI on lower pins, move the priority down
Its more likely the user will use PB4-7
Pretend the User has not soldered the 4 Pinheader
so only do this for non Arduino boards. */
#else
#if !defined(PCINT_CALLBACK_PORT0)
#define PCINT_CALLBACK_PORT0 \
PCINT_CALLBACK(4, 4); \
PCINT_CALLBACK(5, 5); \
PCINT_CALLBACK(6, 6); \
PCINT_CALLBACK(7, 7); \
PCINT_CALLBACK(0, 0); /* SPI SS */ \
PCINT_CALLBACK(1, 1); /* SPI SCK */ \
PCINT_CALLBACK(2, 2); /* SPI MISO */ \
PCINT_CALLBACK(3, 3); /* SPI MOSI */
#endif
#endif
#endif

/* Attiny 25/45/85 only has a very few pins
activate all by default
The order is also okay. */

#if defined(__AVR_ATtiny24__) || defined(__AVR_ATtiny44__) || defined(__AVR_ATtiny84__)
// Port1 is connected to reset, crystal and Pin Interrupt 0
// Deactivate it by default
#if defined(PCINT_ENABLE_PORT1)
#undef PCINT_ENABLE_PORT1
#endif
#endif

/*
Copyright (c) 2014-2021 NicoHood
See the readme for credit to other people.

Permission is hereby granted, free of charge, to any person obtaining a copy
of this software and associated documentation files (the "Software"), to deal
in the Software without restriction, including without limitation the rights
to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
copies of the Software, and to permit persons to whom the Software is
furnished to do so, subject to the following conditions:

The above copyright notice and this permission notice shall be included in
all copies or substantial portions of the Software.

THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN
THE SOFTWARE.
*/

// include guard
// #pragma once

//================================================================================
// Board Definitions
//================================================================================

// Microcontroller specific definitions
// Avr Variants are defined here: https://svn.savannah.gnu.org/viewvc/avr-libc/trunk/avr-libc/include/avr/io.h?view=markup

#if defined(__AVR_ATmega328__) || defined(__AVR_ATmega328A__) || defined(__AVR_ATmega328PA__) || defined(__AVR_ATmega328P__) || defined(__AVR_ATmega328PB__) \
   || defined(__AVR_ATmega168__) || defined(__AVR_ATmega168A__) || defined(__AVR_ATmega168PA__) || defined(__AVR_ATmega168P__) || defined(__AVR_ATmega168PB__) \
   || defined(__AVR_ATmega88__) || defined(__AVR_ATmega88A__) || defined(__AVR_ATmega88PA__) || defined(__AVR_ATmega88P__) || defined(__AVR_ATmega88PB__) \
   || defined(__AVR_ATmega48__) || defined(__AVR_ATmega48A__) || defined(__AVR_ATmega48PA__) || defined(__AVR_ATmega48P__) || defined(__AVR_ATmega48PB__)
// Arduino Uno
#define PCINT_INPUT_PORT0 PINB
#define PCINT_INPUT_PORT1 PINC
#define PCINT_INPUT_PORT2 PIND

#if defined(__AVR_ATmega328PB__)
#define PCINT_INPUT_PORT3 PINE
#endif

#elif defined(__AVR_ATmega162__)

#define PCINT_INPUT_PORT0 PINA
#define PCINT_INPUT_PORT1 PINC

#elif defined(__AVR_ATmega2560__) || defined(__AVR_ATmega1280__) || defined(__AVR_ATmega640__)
// Arduino Mega/Mega2560
#define PCINT_INPUT_PORT0 PINB
#define PCINT_INPUT_PORT2 PINK

// special Port1 case, pins are on 2 HW Pin Ports (E,J)
#if defined(PCINT_ENABLE_PCINT16) // PortE
#if defined(PCINT_ENABLE_PCINT17) || defined(PCINT_ENABLE_PCINT18) \
   || defined(PCINT_ENABLE_PCINT19) || defined(PCINT_ENABLE_PCINT20) \
|| defined(PCINT_ENABLE_PCINT21) || defined(PCINT_ENABLE_PCINT22) \
|| defined(PCINT_ENABLE_PCINT23) // PortJ
// PortE and PortJ selected
#define PCINT_INPUT_PORT1 ((PINE & 0x01) | (PINJ << 1))
#else
// PortE only selected
#define PCINT_INPUT_PORT1 PINE
#endif
#else
// PortJ only selected
// we still have to do the shift because the attach
// function is not designed for this optimization
#define PCINT_INPUT_PORT1 (PINJ << 1)
#endif

#elif defined(__AVR_ATmega32U4__) || defined(__AVR_ATmega16U4__)
// Arduino Leonardo/Micro
#define PCINT_INPUT_PORT0 PINB

#elif defined(__AVR_AT90USB82__) || defined(__AVR_AT90USB162__) || defined(__AVR_ATmega32U2__) || defined(__AVR_ATmega16U2__) || defined(__AVR_ATmega8U2__)
// u2 Series/HoodLoader2
// u2 Series has crappy pin mappings for port 1
#define PCINT_INPUT_PORT0 PINB
#define PCINT_INPUT_PORT1 (((PINC >> 6) & (1 << 0)) | ((PINC >> 4) & (1 << 1)) | ((PINC >> 2) & (1 << 2)) | ((PINC << 1) & (1 << 3)) | ((PIND >> 1) & (1 << 4)))

#elif defined(__AVR_ATtiny25__) || defined(__AVR_ATtiny45__) || defined(__AVR_ATtiny85__)
// Attiny x5
#define PCINT_INPUT_PORT0 PINB

#elif defined(__AVR_ATtiny13__)
// Attiny 13A
#define PCINT_INPUT_PORT0 PINB
// This is just a workaround for the missing definition in the following core: https://sourceforge.net/projects/ard-core13/
// It should work fine with: https://github.com/MCUdude/MicroCore
#ifndef portInputRegister
#define portInputRegister(P) ( (volatile uint8_t *)(PINB) )
#endif

#elif defined(__AVR_ATtiny24__) || defined(__AVR_ATtiny44__) || defined(__AVR_ATtiny84__)
// Attiny x4
#define PCINT_INPUT_PORT0 PINA
#define PCINT_INPUT_PORT1 PINB

#elif defined(__AVR_ATmega1284P__) || defined(__AVR_ATmega644P__) || defined(__AVR_ATmega644__)
// 1284p and 644p, special 4 port case
#define PCINT_INPUT_PORT0 PINA
#define PCINT_INPUT_PORT1 PINB
#define PCINT_INPUT_PORT2 PINC
#define PCINT_INPUT_PORT3 PIND

#elif defined(__AVR_ATtinyX41__) || defined(__AVR_ATtiny441__) || defined(__AVR_ATtiny841__)
// Attiny x41
#define PCINT_INPUT_PORT0 PINA
#define PCINT_INPUT_PORT1 PINB

// "iotn841.h" is missing those definitions, so we add them here
#define PCINT0 0
#define PCINT1 1
#define PCINT2 2
#define PCINT3 3
#define PCINT4 4
#define PCINT5 5
#define PCINT6 6
#define PCINT7 7

#define PCINT8 0
#define PCINT9 1
#define PCINT10 2
#define PCINT11 3

#elif defined(__AVR_ATtiny2313__)
#define PCINT_INPUT_PORT0 PINB

#elif defined(__AVR_ATtiny2313A__) || defined(__AVR_ATtiny4313__)
// All 8 pins
#define PCINT_INPUT_PORT0 PINB
// PinA has 3 PCINTs on the reset and clock lines, we do not use this port
// PIND has 7 pins available, but the pin ordering is so messed up,
// that it does not work with the current library structure.

#elif defined(__AVR_ATtiny261__) || defined(__AVR_ATtiny461__) || defined(__AVR_ATtiny861__)
// PORTB has Reset, clock and SPI while PORTA has I2C and Analog Pins. We just enable all pins.
#define PCINT_INPUT_PORT0 PINA
#define PCINT_INPUT_PORT1 PINB

#else // Microcontroller not supported
#error PinChangeInterrupt library does not support this MCU.
#endif

//================================================================================
// Add missing definitions
//================================================================================

// add fakes if ports are not used
#ifndef PCINT_INPUT_PORT0
#define PCINT_INPUT_PORT0 0
#else
#define PCINT_INPUT_PORT0_USED
#endif
#ifndef PCINT_INPUT_PORT1
#define PCINT_INPUT_PORT1 0
#else
#define PCINT_INPUT_PORT1_USED
#endif
#ifndef PCINT_INPUT_PORT2
#define PCINT_INPUT_PORT2 0
#else
#define PCINT_INPUT_PORT2_USED
#endif
#ifndef PCINT_INPUT_PORT3
#define PCINT_INPUT_PORT3 0
#else
#define PCINT_INPUT_PORT3_USED
#endif

/*
Copyright (c) 2014-2021 NicoHood
See the readme for credit to other people.

Permission is hereby granted, free of charge, to any person obtaining a copy
of this software and associated documentation files (the "Software"), to deal
in the Software without restriction, including without limitation the rights
to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
copies of the Software, and to permit persons to whom the Software is
furnished to do so, subject to the following conditions:

The above copyright notice and this permission notice shall be included in
all copies or substantial portions of the Software.

THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN
THE SOFTWARE.
*/

// include guard
// #pragma once

/*
The strategy in this file:

0. Makefile optimization:
   To disable PCINT/PORTs via makefile use -DPCINT_DISABLE_PORT0 etc.
   This will undef a previously defined PCINT_ENABLE_PORT0.

1. Reduce the user settings to the enabled pins.
   If the whole port is deactivated, also disable all pins on this port.

2. Define the hardware available pins/ports.

3. Compare the hardware definition with the enabled pin definitions.
   If the pin is available and enabled, create a makro to use the pin.

4. Count all used pins (for each port).

5. If there are no pins used on a port, do not use the port

6. Finally we have a clear defintion of the used pins/ports like this:
PCINT_USE_PCINT0 (true/false)
PCINT_USE_PORT0 (true/false)

Other definitions that can be used:
PCINT_HAS_PORT0 (true/false)
PCINT_HAS_PCINT0 (true/false)
PCINT_NUM_PINS_PORT0 (0-8)
PCINT_NUM_USED_PINS_PORT0 (0-8)
EXTERNAL_NUM_PINCHANGEINTERRUPT (0-24)
EXTERNAL_NUM_USED_PINCHANGEINTERRUPT (0-24)
PCINT_NUM_PORTS (0-3)
PCINT_NUM_USED_PORTS (0-3)
*/

//================================================================================
// Disabled Pins
//================================================================================
/*
for (int port = 0; port < 4; port++) {
  Serial.print("#if defined(PCINT_ENABLE_PORT");
  Serial.print(port);
  Serial.print(") && defined(PCINT_DISABLE_PORT");
  Serial.print(port);
  Serial.println(")");
  Serial.print("#undef PCINT_ENABLE_PORT");
  Serial.println(port);
  Serial.println("#endif");
  Serial.println();
}

for (int i = 0; i < 32; i++) {
  Serial.print("#if defined(PCINT_ENABLE_PCINT");
  Serial.print(i);
  Serial.print(") && defined(PCINT_DISABLE_PCINT");
  Serial.print(i);
  Serial.println(")");
  Serial.print("#undef PCINT_ENABLE_PCINT");
  Serial.println(i);
  Serial.println("#endif");
  Serial.println();
}
*/
#if defined(PCINT_ENABLE_PORT0) && defined(PCINT_DISABLE_PORT0)
#undef PCINT_ENABLE_PORT0
#endif

#if defined(PCINT_ENABLE_PORT1) && defined(PCINT_DISABLE_PORT1)
#undef PCINT_ENABLE_PORT1
#endif

#if defined(PCINT_ENABLE_PORT2) && defined(PCINT_DISABLE_PORT2)
#undef PCINT_ENABLE_PORT2
#endif

#if defined(PCINT_ENABLE_PORT3) && defined(PCINT_DISABLE_PORT3)
#undef PCINT_ENABLE_PORT3
#endif

#if defined(PCINT_ENABLE_PCINT0) && defined(PCINT_DISABLE_PCINT0)
#undef PCINT_ENABLE_PCINT0
#endif

#if defined(PCINT_ENABLE_PCINT1) && defined(PCINT_DISABLE_PCINT1)
#undef PCINT_ENABLE_PCINT1
#endif

#if defined(PCINT_ENABLE_PCINT2) && defined(PCINT_DISABLE_PCINT2)
#undef PCINT_ENABLE_PCINT2
#endif

#if defined(PCINT_ENABLE_PCINT3) && defined(PCINT_DISABLE_PCINT3)
#undef PCINT_ENABLE_PCINT3
#endif

#if defined(PCINT_ENABLE_PCINT4) && defined(PCINT_DISABLE_PCINT4)
#undef PCINT_ENABLE_PCINT4
#endif

#if defined(PCINT_ENABLE_PCINT5) && defined(PCINT_DISABLE_PCINT5)
#undef PCINT_ENABLE_PCINT5
#endif

#if defined(PCINT_ENABLE_PCINT6) && defined(PCINT_DISABLE_PCINT6)
#undef PCINT_ENABLE_PCINT6
#endif

#if defined(PCINT_ENABLE_PCINT7) && defined(PCINT_DISABLE_PCINT7)
#undef PCINT_ENABLE_PCINT7
#endif

#if defined(PCINT_ENABLE_PCINT8) && defined(PCINT_DISABLE_PCINT8)
#undef PCINT_ENABLE_PCINT8
#endif

#if defined(PCINT_ENABLE_PCINT9) && defined(PCINT_DISABLE_PCINT9)
#undef PCINT_ENABLE_PCINT9
#endif

#if defined(PCINT_ENABLE_PCINT10) && defined(PCINT_DISABLE_PCINT10)
#undef PCINT_ENABLE_PCINT10
#endif

#if defined(PCINT_ENABLE_PCINT11) && defined(PCINT_DISABLE_PCINT11)
#undef PCINT_ENABLE_PCINT11
#endif

#if defined(PCINT_ENABLE_PCINT12) && defined(PCINT_DISABLE_PCINT12)
#undef PCINT_ENABLE_PCINT12
#endif

#if defined(PCINT_ENABLE_PCINT13) && defined(PCINT_DISABLE_PCINT13)
#undef PCINT_ENABLE_PCINT13
#endif

#if defined(PCINT_ENABLE_PCINT14) && defined(PCINT_DISABLE_PCINT14)
#undef PCINT_ENABLE_PCINT14
#endif

#if defined(PCINT_ENABLE_PCINT15) && defined(PCINT_DISABLE_PCINT15)
#undef PCINT_ENABLE_PCINT15
#endif

#if defined(PCINT_ENABLE_PCINT16) && defined(PCINT_DISABLE_PCINT16)
#undef PCINT_ENABLE_PCINT16
#endif

#if defined(PCINT_ENABLE_PCINT17) && defined(PCINT_DISABLE_PCINT17)
#undef PCINT_ENABLE_PCINT17
#endif

#if defined(PCINT_ENABLE_PCINT18) && defined(PCINT_DISABLE_PCINT18)
#undef PCINT_ENABLE_PCINT18
#endif

#if defined(PCINT_ENABLE_PCINT19) && defined(PCINT_DISABLE_PCINT19)
#undef PCINT_ENABLE_PCINT19
#endif

#if defined(PCINT_ENABLE_PCINT20) && defined(PCINT_DISABLE_PCINT20)
#undef PCINT_ENABLE_PCINT20
#endif

#if defined(PCINT_ENABLE_PCINT21) && defined(PCINT_DISABLE_PCINT21)
#undef PCINT_ENABLE_PCINT21
#endif

#if defined(PCINT_ENABLE_PCINT22) && defined(PCINT_DISABLE_PCINT22)
#undef PCINT_ENABLE_PCINT22
#endif

#if defined(PCINT_ENABLE_PCINT23) && defined(PCINT_DISABLE_PCINT23)
#undef PCINT_ENABLE_PCINT23
#endif

#if defined(PCINT_ENABLE_PCINT24) && defined(PCINT_DISABLE_PCINT24)
#undef PCINT_ENABLE_PCINT24
#endif

#if defined(PCINT_ENABLE_PCINT25) && defined(PCINT_DISABLE_PCINT25)
#undef PCINT_ENABLE_PCINT25
#endif

#if defined(PCINT_ENABLE_PCINT26) && defined(PCINT_DISABLE_PCINT26)
#undef PCINT_ENABLE_PCINT26
#endif

#if defined(PCINT_ENABLE_PCINT27) && defined(PCINT_DISABLE_PCINT27)
#undef PCINT_ENABLE_PCINT27
#endif

#if defined(PCINT_ENABLE_PCINT28) && defined(PCINT_DISABLE_PCINT28)
#undef PCINT_ENABLE_PCINT28
#endif

#if defined(PCINT_ENABLE_PCINT29) && defined(PCINT_DISABLE_PCINT29)
#undef PCINT_ENABLE_PCINT29
#endif

#if defined(PCINT_ENABLE_PCINT30) && defined(PCINT_DISABLE_PCINT30)
#undef PCINT_ENABLE_PCINT30
#endif

#if defined(PCINT_ENABLE_PCINT31) && defined(PCINT_DISABLE_PCINT31)
#undef PCINT_ENABLE_PCINT31
#endif


//================================================================================
// Enabled Pins
//================================================================================

/* Disable all pins on a port, if port is deactivated
We could then check every pin -> port definition
But that'd be a mess and doesnt help
Users who deactivate stuff should know
what the are doing.
So we use the enabled pins for all next definitions*/
/*
for (int port = 0; port < 4; port++) {
Serial.print("#if !defined(PCINT_ENABLE_PORT");
Serial.print(port);
Serial.println(")");
for (int i = 0; i < 8; i++) {
Serial.print("#if defined(PCINT_ENABLE_PCINT");
Serial.print(port * 8 + i);
Serial.println(")");
Serial.print("#undef PCINT_ENABLE_PCINT");
Serial.println(port * 8 + i);
Serial.println("#endif");
}
Serial.println("#endif");
Serial.println();
}
*/
#if !defined(PCINT_ENABLE_PORT0)
#if defined(PCINT_ENABLE_PCINT0)
#undef PCINT_ENABLE_PCINT0
#endif
#if defined(PCINT_ENABLE_PCINT1)
#undef PCINT_ENABLE_PCINT1
#endif
#if defined(PCINT_ENABLE_PCINT2)
#undef PCINT_ENABLE_PCINT2
#endif
#if defined(PCINT_ENABLE_PCINT3)
#undef PCINT_ENABLE_PCINT3
#endif
#if defined(PCINT_ENABLE_PCINT4)
#undef PCINT_ENABLE_PCINT4
#endif
#if defined(PCINT_ENABLE_PCINT5)
#undef PCINT_ENABLE_PCINT5
#endif
#if defined(PCINT_ENABLE_PCINT6)
#undef PCINT_ENABLE_PCINT6
#endif
#if defined(PCINT_ENABLE_PCINT7)
#undef PCINT_ENABLE_PCINT7
#endif
#endif

#if !defined(PCINT_ENABLE_PORT1)
#if defined(PCINT_ENABLE_PCINT8)
#undef PCINT_ENABLE_PCINT8
#endif
#if defined(PCINT_ENABLE_PCINT9)
#undef PCINT_ENABLE_PCINT9
#endif
#if defined(PCINT_ENABLE_PCINT10)
#undef PCINT_ENABLE_PCINT10
#endif
#if defined(PCINT_ENABLE_PCINT11)
#undef PCINT_ENABLE_PCINT11
#endif
#if defined(PCINT_ENABLE_PCINT12)
#undef PCINT_ENABLE_PCINT12
#endif
#if defined(PCINT_ENABLE_PCINT13)
#undef PCINT_ENABLE_PCINT13
#endif
#if defined(PCINT_ENABLE_PCINT14)
#undef PCINT_ENABLE_PCINT14
#endif
#if defined(PCINT_ENABLE_PCINT15)
#undef PCINT_ENABLE_PCINT15
#endif
#endif

#if !defined(PCINT_ENABLE_PORT2)
#if defined(PCINT_ENABLE_PCINT16)
#undef PCINT_ENABLE_PCINT16
#endif
#if defined(PCINT_ENABLE_PCINT17)
#undef PCINT_ENABLE_PCINT17
#endif
#if defined(PCINT_ENABLE_PCINT18)
#undef PCINT_ENABLE_PCINT18
#endif
#if defined(PCINT_ENABLE_PCINT19)
#undef PCINT_ENABLE_PCINT19
#endif
#if defined(PCINT_ENABLE_PCINT20)
#undef PCINT_ENABLE_PCINT20
#endif
#if defined(PCINT_ENABLE_PCINT21)
#undef PCINT_ENABLE_PCINT21
#endif
#if defined(PCINT_ENABLE_PCINT22)
#undef PCINT_ENABLE_PCINT22
#endif
#if defined(PCINT_ENABLE_PCINT23)
#undef PCINT_ENABLE_PCINT23
#endif
#endif

#if !defined(PCINT_ENABLE_PORT3)
#if defined(PCINT_ENABLE_PCINT24)
#undef PCINT_ENABLE_PCINT24
#endif
#if defined(PCINT_ENABLE_PCINT25)
#undef PCINT_ENABLE_PCINT25
#endif
#if defined(PCINT_ENABLE_PCINT26)
#undef PCINT_ENABLE_PCINT26
#endif
#if defined(PCINT_ENABLE_PCINT27)
#undef PCINT_ENABLE_PCINT27
#endif
#if defined(PCINT_ENABLE_PCINT28)
#undef PCINT_ENABLE_PCINT28
#endif
#if defined(PCINT_ENABLE_PCINT29)
#undef PCINT_ENABLE_PCINT29
#endif
#if defined(PCINT_ENABLE_PCINT30)
#undef PCINT_ENABLE_PCINT30
#endif
#if defined(PCINT_ENABLE_PCINT31)
#undef PCINT_ENABLE_PCINT31
#endif
#endif


//================================================================================
// Hardware Definitions
//================================================================================

#if defined(PCINT0_vect)
#define PCINT_HAS_PORT0 true
#else
#define PCINT_HAS_PORT0 false
#endif
#if defined(PCINT1_vect)
#define PCINT_HAS_PORT1 true
#else
#define PCINT_HAS_PORT1 false
#endif
#if defined(PCINT2_vect)
#define PCINT_HAS_PORT2 true
#else
#define PCINT_HAS_PORT2 false
#endif
#if defined(PCINT3_vect)
#define PCINT_HAS_PORT3 true
#else
#define PCINT_HAS_PORT3 false
#endif

// number of available ports
#define PCINT_NUM_PORTS ( \
PCINT_HAS_PORT0 + \
PCINT_HAS_PORT1 + \
PCINT_HAS_PORT2 + \
PCINT_HAS_PORT3)

/*
for (int i = 0; i < 32; i++) {
Serial.print("#ifdef PCINT");
Serial.println(i);
Serial.print("#define PCINT_HAS_PCINT");
Serial.print(i);
Serial.println(" true");
Serial.println("#else");
Serial.print("#define PCINT_HAS_PCINT");
Serial.print(i);
Serial.println(" false");
Serial.println("#endif");
}
*/
#ifdef PCINT0
#define PCINT_HAS_PCINT0 true
#else
#define PCINT_HAS_PCINT0 false
#endif
#ifdef PCINT1
#define PCINT_HAS_PCINT1 true
#else
#define PCINT_HAS_PCINT1 false
#endif
#ifdef PCINT2
#define PCINT_HAS_PCINT2 true
#else
#define PCINT_HAS_PCINT2 false
#endif
#ifdef PCINT3
#define PCINT_HAS_PCINT3 true
#else
#define PCINT_HAS_PCINT3 false
#endif
#ifdef PCINT4
#define PCINT_HAS_PCINT4 true
#else
#define PCINT_HAS_PCINT4 false
#endif
#ifdef PCINT5
#define PCINT_HAS_PCINT5 true
#else
#define PCINT_HAS_PCINT5 false
#endif
#ifdef PCINT6
#define PCINT_HAS_PCINT6 true
#else
#define PCINT_HAS_PCINT6 false
#endif
#ifdef PCINT7
#define PCINT_HAS_PCINT7 true
#else
#define PCINT_HAS_PCINT7 false
#endif
#ifdef PCINT8
#define PCINT_HAS_PCINT8 true
#else
#define PCINT_HAS_PCINT8 false
#endif
#ifdef PCINT9
#define PCINT_HAS_PCINT9 true
#else
#define PCINT_HAS_PCINT9 false
#endif
#ifdef PCINT10
#define PCINT_HAS_PCINT10 true
#else
#define PCINT_HAS_PCINT10 false
#endif
#ifdef PCINT11
#define PCINT_HAS_PCINT11 true
#else
#define PCINT_HAS_PCINT11 false
#endif
#ifdef PCINT12
#define PCINT_HAS_PCINT12 true
#else
#define PCINT_HAS_PCINT12 false
#endif
#ifdef PCINT13
#define PCINT_HAS_PCINT13 true
#else
#define PCINT_HAS_PCINT13 false
#endif
#ifdef PCINT14
#define PCINT_HAS_PCINT14 true
#else
#define PCINT_HAS_PCINT14 false
#endif
#ifdef PCINT15
#define PCINT_HAS_PCINT15 true
#else
#define PCINT_HAS_PCINT15 false
#endif
#ifdef PCINT16
#define PCINT_HAS_PCINT16 true
#else
#define PCINT_HAS_PCINT16 false
#endif
#ifdef PCINT17
#define PCINT_HAS_PCINT17 true
#else
#define PCINT_HAS_PCINT17 false
#endif
#ifdef PCINT18
#define PCINT_HAS_PCINT18 true
#else
#define PCINT_HAS_PCINT18 false
#endif
#ifdef PCINT19
#define PCINT_HAS_PCINT19 true
#else
#define PCINT_HAS_PCINT19 false
#endif
#ifdef PCINT20
#define PCINT_HAS_PCINT20 true
#else
#define PCINT_HAS_PCINT20 false
#endif
#ifdef PCINT21
#define PCINT_HAS_PCINT21 true
#else
#define PCINT_HAS_PCINT21 false
#endif
#ifdef PCINT22
#define PCINT_HAS_PCINT22 true
#else
#define PCINT_HAS_PCINT22 false
#endif
#ifdef PCINT23
#define PCINT_HAS_PCINT23 true
#else
#define PCINT_HAS_PCINT23 false
#endif
#ifdef PCINT24
#define PCINT_HAS_PCINT24 true
#else
#define PCINT_HAS_PCINT24 false
#endif
#ifdef PCINT25
#define PCINT_HAS_PCINT25 true
#else
#define PCINT_HAS_PCINT25 false
#endif
#ifdef PCINT26
#define PCINT_HAS_PCINT26 true
#else
#define PCINT_HAS_PCINT26 false
#endif
#ifdef PCINT27
#define PCINT_HAS_PCINT27 true
#else
#define PCINT_HAS_PCINT27 false
#endif
#ifdef PCINT28
#define PCINT_HAS_PCINT28 true
#else
#define PCINT_HAS_PCINT28 false
#endif
#ifdef PCINT29
#define PCINT_HAS_PCINT29 true
#else
#define PCINT_HAS_PCINT29 false
#endif
#ifdef PCINT30
#define PCINT_HAS_PCINT30 true
#else
#define PCINT_HAS_PCINT30 false
#endif
#ifdef PCINT31
#define PCINT_HAS_PCINT31 true
#else
#define PCINT_HAS_PCINT31 false
#endif


// count numbers of available pins on each port
/*
for (int port = 0; port < 4; port++) {
Serial.print("#define PCINT_NUM_PINS_PORT");
Serial.print(port);
Serial.println(" ( \\");
for (int i = 0; i < 8; i++) {
Serial.print("PCINT_HAS_PCINT");
Serial.print(port * 8 + i);
if (i != 7)
Serial.println(" + \\");
}
Serial.println(")");
Serial.println();
}
*/
#define PCINT_NUM_PINS_PORT0 ( \
PCINT_HAS_PCINT0 + \
PCINT_HAS_PCINT1 + \
PCINT_HAS_PCINT2 + \
PCINT_HAS_PCINT3 + \
PCINT_HAS_PCINT4 + \
PCINT_HAS_PCINT5 + \
PCINT_HAS_PCINT6 + \
PCINT_HAS_PCINT7)

#define PCINT_NUM_PINS_PORT1 ( \
PCINT_HAS_PCINT8 + \
PCINT_HAS_PCINT9 + \
PCINT_HAS_PCINT10 + \
PCINT_HAS_PCINT11 + \
PCINT_HAS_PCINT12 + \
PCINT_HAS_PCINT13 + \
PCINT_HAS_PCINT14 + \
PCINT_HAS_PCINT15)

#define PCINT_NUM_PINS_PORT2 ( \
PCINT_HAS_PCINT16 + \
PCINT_HAS_PCINT17 + \
PCINT_HAS_PCINT18 + \
PCINT_HAS_PCINT19 + \
PCINT_HAS_PCINT20 + \
PCINT_HAS_PCINT21 + \
PCINT_HAS_PCINT22 + \
PCINT_HAS_PCINT23)

#define PCINT_NUM_PINS_PORT3 ( \
PCINT_HAS_PCINT24 + \
PCINT_HAS_PCINT25 + \
PCINT_HAS_PCINT26 + \
PCINT_HAS_PCINT27 + \
PCINT_HAS_PCINT28 + \
PCINT_HAS_PCINT29 + \
PCINT_HAS_PCINT30 + \
PCINT_HAS_PCINT31)


// number of available hardware pins
#define EXTERNAL_NUM_PINCHANGEINTERRUPT ( \
PCINT_NUM_PINS_PORT0 + \
PCINT_NUM_PINS_PORT1 + \
PCINT_NUM_PINS_PORT2 + \
PCINT_NUM_PINS_PORT3)


//================================================================================
// Used Pins
//================================================================================

// check if pins are physically available and enabled
/*
for (int i = 0; i < 32; i++) {
Serial.print("#if (PCINT_HAS_PCINT");
Serial.print(i);
Serial.print(" == true) && defined(PCINT_ENABLE_PCINT");
Serial.print(i);
Serial.println(")");
Serial.print("#define PCINT_USE_PCINT");
Serial.print(i);
Serial.println(" true");
Serial.println("#else");
Serial.print("#define PCINT_USE_PCINT");
Serial.print(i);
Serial.println(" false");
Serial.println("#endif");
}
*/
#if (PCINT_HAS_PCINT0 == true) && defined(PCINT_ENABLE_PCINT0)
#define PCINT_USE_PCINT0 true
#else
#define PCINT_USE_PCINT0 false
#endif
#if (PCINT_HAS_PCINT1 == true) && defined(PCINT_ENABLE_PCINT1)
#define PCINT_USE_PCINT1 true
#else
#define PCINT_USE_PCINT1 false
#endif
#if (PCINT_HAS_PCINT2 == true) && defined(PCINT_ENABLE_PCINT2)
#define PCINT_USE_PCINT2 true
#else
#define PCINT_USE_PCINT2 false
#endif
#if (PCINT_HAS_PCINT3 == true) && defined(PCINT_ENABLE_PCINT3)
#define PCINT_USE_PCINT3 true
#else
#define PCINT_USE_PCINT3 false
#endif
#if (PCINT_HAS_PCINT4 == true) && defined(PCINT_ENABLE_PCINT4)
#define PCINT_USE_PCINT4 true
#else
#define PCINT_USE_PCINT4 false
#endif
#if (PCINT_HAS_PCINT5 == true) && defined(PCINT_ENABLE_PCINT5)
#define PCINT_USE_PCINT5 true
#else
#define PCINT_USE_PCINT5 false
#endif
#if (PCINT_HAS_PCINT6 == true) && defined(PCINT_ENABLE_PCINT6)
#define PCINT_USE_PCINT6 true
#else
#define PCINT_USE_PCINT6 false
#endif
#if (PCINT_HAS_PCINT7 == true) && defined(PCINT_ENABLE_PCINT7)
#define PCINT_USE_PCINT7 true
#else
#define PCINT_USE_PCINT7 false
#endif
#if (PCINT_HAS_PCINT8 == true) && defined(PCINT_ENABLE_PCINT8)
#define PCINT_USE_PCINT8 true
#else
#define PCINT_USE_PCINT8 false
#endif
#if (PCINT_HAS_PCINT9 == true) && defined(PCINT_ENABLE_PCINT9)
#define PCINT_USE_PCINT9 true
#else
#define PCINT_USE_PCINT9 false
#endif
#if (PCINT_HAS_PCINT10 == true) && defined(PCINT_ENABLE_PCINT10)
#define PCINT_USE_PCINT10 true
#else
#define PCINT_USE_PCINT10 false
#endif
#if (PCINT_HAS_PCINT11 == true) && defined(PCINT_ENABLE_PCINT11)
#define PCINT_USE_PCINT11 true
#else
#define PCINT_USE_PCINT11 false
#endif
#if (PCINT_HAS_PCINT12 == true) && defined(PCINT_ENABLE_PCINT12)
#define PCINT_USE_PCINT12 true
#else
#define PCINT_USE_PCINT12 false
#endif
#if (PCINT_HAS_PCINT13 == true) && defined(PCINT_ENABLE_PCINT13)
#define PCINT_USE_PCINT13 true
#else
#define PCINT_USE_PCINT13 false
#endif
#if (PCINT_HAS_PCINT14 == true) && defined(PCINT_ENABLE_PCINT14)
#define PCINT_USE_PCINT14 true
#else
#define PCINT_USE_PCINT14 false
#endif
#if (PCINT_HAS_PCINT15 == true) && defined(PCINT_ENABLE_PCINT15)
#define PCINT_USE_PCINT15 true
#else
#define PCINT_USE_PCINT15 false
#endif
#if (PCINT_HAS_PCINT16 == true) && defined(PCINT_ENABLE_PCINT16)
#define PCINT_USE_PCINT16 true
#else
#define PCINT_USE_PCINT16 false
#endif
#if (PCINT_HAS_PCINT17 == true) && defined(PCINT_ENABLE_PCINT17)
#define PCINT_USE_PCINT17 true
#else
#define PCINT_USE_PCINT17 false
#endif
#if (PCINT_HAS_PCINT18 == true) && defined(PCINT_ENABLE_PCINT18)
#define PCINT_USE_PCINT18 true
#else
#define PCINT_USE_PCINT18 false
#endif
#if (PCINT_HAS_PCINT19 == true) && defined(PCINT_ENABLE_PCINT19)
#define PCINT_USE_PCINT19 true
#else
#define PCINT_USE_PCINT19 false
#endif
#if (PCINT_HAS_PCINT20 == true) && defined(PCINT_ENABLE_PCINT20)
#define PCINT_USE_PCINT20 true
#else
#define PCINT_USE_PCINT20 false
#endif
#if (PCINT_HAS_PCINT21 == true) && defined(PCINT_ENABLE_PCINT21)
#define PCINT_USE_PCINT21 true
#else
#define PCINT_USE_PCINT21 false
#endif
#if (PCINT_HAS_PCINT22 == true) && defined(PCINT_ENABLE_PCINT22)
#define PCINT_USE_PCINT22 true
#else
#define PCINT_USE_PCINT22 false
#endif
#if (PCINT_HAS_PCINT23 == true) && defined(PCINT_ENABLE_PCINT23)
#define PCINT_USE_PCINT23 true
#else
#define PCINT_USE_PCINT23 false
#endif
#if (PCINT_HAS_PCINT24 == true) && defined(PCINT_ENABLE_PCINT24)
#define PCINT_USE_PCINT24 true
#else
#define PCINT_USE_PCINT24 false
#endif
#if (PCINT_HAS_PCINT25 == true) && defined(PCINT_ENABLE_PCINT25)
#define PCINT_USE_PCINT25 true
#else
#define PCINT_USE_PCINT25 false
#endif
#if (PCINT_HAS_PCINT26 == true) && defined(PCINT_ENABLE_PCINT26)
#define PCINT_USE_PCINT26 true
#else
#define PCINT_USE_PCINT26 false
#endif
#if (PCINT_HAS_PCINT27 == true) && defined(PCINT_ENABLE_PCINT27)
#define PCINT_USE_PCINT27 true
#else
#define PCINT_USE_PCINT27 false
#endif
#if (PCINT_HAS_PCINT28 == true) && defined(PCINT_ENABLE_PCINT28)
#define PCINT_USE_PCINT28 true
#else
#define PCINT_USE_PCINT28 false
#endif
#if (PCINT_HAS_PCINT29 == true) && defined(PCINT_ENABLE_PCINT29)
#define PCINT_USE_PCINT29 true
#else
#define PCINT_USE_PCINT29 false
#endif
#if (PCINT_HAS_PCINT30 == true) && defined(PCINT_ENABLE_PCINT30)
#define PCINT_USE_PCINT30 true
#else
#define PCINT_USE_PCINT30 false
#endif
#if (PCINT_HAS_PCINT31 == true) && defined(PCINT_ENABLE_PCINT31)
#define PCINT_USE_PCINT31 true
#else
#define PCINT_USE_PCINT31 false
#endif


//================================================================================
// Number Used Pins
//================================================================================

// count numbers of used pins on each port
/*
for (int port = 0; port < 4; port++) {
Serial.print("#define PCINT_NUM_USED_PINS_PORT");
Serial.print(port);
Serial.println(" ( \\");
for (int i = 0; i < 8; i++) {
Serial.print("PCINT_USE_PCINT");
Serial.print(port * 8 + i);
if (i != 7)
Serial.println(" + \\");
}
Serial.println(")");
Serial.println();
}
*/
#define PCINT_NUM_USED_PINS_PORT0 ( \
PCINT_USE_PCINT0 + \
PCINT_USE_PCINT1 + \
PCINT_USE_PCINT2 + \
PCINT_USE_PCINT3 + \
PCINT_USE_PCINT4 + \
PCINT_USE_PCINT5 + \
PCINT_USE_PCINT6 + \
PCINT_USE_PCINT7)

#define PCINT_NUM_USED_PINS_PORT1 ( \
PCINT_USE_PCINT8 + \
PCINT_USE_PCINT9 + \
PCINT_USE_PCINT10 + \
PCINT_USE_PCINT11 + \
PCINT_USE_PCINT12 + \
PCINT_USE_PCINT13 + \
PCINT_USE_PCINT14 + \
PCINT_USE_PCINT15)

#define PCINT_NUM_USED_PINS_PORT2 ( \
PCINT_USE_PCINT16 + \
PCINT_USE_PCINT17 + \
PCINT_USE_PCINT18 + \
PCINT_USE_PCINT19 + \
PCINT_USE_PCINT20 + \
PCINT_USE_PCINT21 + \
PCINT_USE_PCINT22 + \
PCINT_USE_PCINT23)

#define PCINT_NUM_USED_PINS_PORT3 ( \
PCINT_USE_PCINT24 + \
PCINT_USE_PCINT25 + \
PCINT_USE_PCINT26 + \
PCINT_USE_PCINT27 + \
PCINT_USE_PCINT28 + \
PCINT_USE_PCINT29 + \
PCINT_USE_PCINT30 + \
PCINT_USE_PCINT31)


// number of used hardware pins
#define EXTERNAL_NUM_USED_PINCHANGEINTERRUPT ( \
PCINT_NUM_USED_PINS_PORT0 + \
PCINT_NUM_USED_PINS_PORT1 + \
PCINT_NUM_USED_PINS_PORT2 + \
PCINT_NUM_USED_PINS_PORT3)

//================================================================================
// Used Ports
//================================================================================

// check if ports are used
#if PCINT_NUM_USED_PINS_PORT0
#define PCINT_USE_PORT0 true
#else
#define PCINT_USE_PORT0 false
#endif
#if PCINT_NUM_USED_PINS_PORT1
#define PCINT_USE_PORT1 true
#else
#define PCINT_USE_PORT1 false
#endif
#if PCINT_NUM_USED_PINS_PORT2
#define PCINT_USE_PORT2 true
#else
#define PCINT_USE_PORT2 false
#endif
#if PCINT_NUM_USED_PINS_PORT3
#define PCINT_USE_PORT3 true
#else
#define PCINT_USE_PORT3 false
#endif

// number of used ports
#define PCINT_NUM_USED_PORTS ( \
PCINT_USE_PORT0 + \
PCINT_USE_PORT1 + \
PCINT_USE_PORT2 + \
PCINT_USE_PORT3)

/*
Copyright (c) 2014-2021 NicoHood
See the readme for credit to other people.

Permission is hereby granted, free of charge, to any person obtaining a copy
of this software and associated documentation files (the "Software"), to deal
in the Software without restriction, including without limitation the rights
to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
copies of the Software, and to permit persons to whom the Software is
furnished to do so, subject to the following conditions:

The above copyright notice and this permission notice shall be included in
all copies or substantial portions of the Software.

THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN
THE SOFTWARE.
*/

// Include Guard
// #pragma once

// Software Version
#define PCINT_VERSION 129

#include <avr/io.h>
#include <avr/interrupt.h>

//#ifdef ARDUINO
//#include "Arduino.h"

#ifndef ARDUINO_ARCH_AVR
#error This library can only be used with AVR
#endif

//#else

#ifndef LOW
#define LOW 0x0
#endif
#ifndef CHANGE
#define CHANGE 0x1
#endif
#ifndef FALLING
#define FALLING 0x2
#endif
#ifndef RISING
#define RISING 0x3
#endif

//#endif

//================================================================================
// General Helper Definitions and Mappings
//================================================================================

// Settings and Board definitions are seperated to get an better overview.
// The order and position of the inclusion is important!
// #include "PinChangeInterruptSettings.h"
// #include "PinChangeInterruptBoards.h"
// #include "PinChangeInterruptPins.h"

//#if !PCINT_NUM_USED_PORTS
//#error Please enable at least one PCINT port and pin!
//#endif

// manually include cpp files to save flash if only 1 ISR is present
// it includes all ISR files but only the 1 available ISR will/can be compiled
#if (PCINT_NUM_USED_PORTS == 1)
#ifndef PCINT_COMPILE_ENABLED_ISR
#define PCINT_COMPILE_ENABLED_ISR
#endif
#endif

//================================================================================
// Makro Definitions
//================================================================================

// generates the callback for easier reordering in Settings
#define PCINT_MACRO_BRACKETS ()
#define PCINT_MACRO_TRUE == true)
#define PCINT_CALLBACK(bit, pcint) \
if (PCINT_USE_PCINT ## pcint PCINT_MACRO_TRUE \
if (trigger & (1 << bit)) \
PinChangeInterruptEventPCINT ## pcint PCINT_MACRO_BRACKETS

// definition used by the user to create custom LowLevel PCINT Events
#define PinChangeInterruptEvent_Wrapper(n) PinChangeInterruptEventPCINT ## n
#define PinChangeInterruptEvent(n) PinChangeInterruptEvent_Wrapper(n)

// missing 1.0.6 definition workaround
#ifndef NOT_AN_INTERRUPT
#define NOT_AN_INTERRUPT -1
#endif

// convert a normal pin to its PCINT number (0 - max 23), used by the user
// calculates the pin by the Arduino definitions
#if defined(PCIE0)
#define digitalPinToPinChangeInterrupt(p) (digitalPinToPCICR(p) ? ((8 * (digitalPinToPCICRbit(p) - PCIE0)) + digitalPinToPCMSKbit(p)) : NOT_AN_INTERRUPT)
#elif defined(PCIE)
#define digitalPinToPinChangeInterrupt(p) (digitalPinToPCICR(p) ? ((8 * (digitalPinToPCICRbit(p) - PCIE)) + digitalPinToPCMSKbit(p)) : NOT_AN_INTERRUPT)
#else
#error MCU has no such a register
#endif

// alias for shorter writing
#define PCINTEvent(n) PinChangeInterruptEvent_Wrapper(n)
#define digitalPinToPCINT digitalPinToPinChangeInterrupt
#define attachPCINT attachPinChangeInterrupt
#define enablePCINT enablePinChangeInterrupt
#define detachPCINT detachPinChangeInterrupt
#define disablePCINT disablePinChangeInterrupt
#define getPCINTTrigger getPinChangeInterruptTrigger

//================================================================================
// Function Prototypes + Variables
//================================================================================

// typedef for our callback function pointers
typedef void(*callback)(void);

// useless function for weak implemented/not used functions, extern c needed for the alias
#ifdef __cplusplus
extern "C" {
#endif
    void pcint_null_callback(void);
#ifdef __cplusplus
}
#endif

void PinChangeInterruptEventPCINT0(void);
void PinChangeInterruptEventPCINT1(void);
void PinChangeInterruptEventPCINT2(void);
void PinChangeInterruptEventPCINT3(void);
void PinChangeInterruptEventPCINT4(void);
void PinChangeInterruptEventPCINT5(void);
void PinChangeInterruptEventPCINT6(void);
void PinChangeInterruptEventPCINT7(void);
void PinChangeInterruptEventPCINT8(void);
void PinChangeInterruptEventPCINT9(void);
void PinChangeInterruptEventPCINT10(void);
void PinChangeInterruptEventPCINT11(void);
void PinChangeInterruptEventPCINT12(void);
void PinChangeInterruptEventPCINT13(void);
void PinChangeInterruptEventPCINT14(void);
void PinChangeInterruptEventPCINT15(void);
void PinChangeInterruptEventPCINT16(void);
void PinChangeInterruptEventPCINT17(void);
void PinChangeInterruptEventPCINT18(void);
void PinChangeInterruptEventPCINT19(void);
void PinChangeInterruptEventPCINT20(void);
void PinChangeInterruptEventPCINT21(void);
void PinChangeInterruptEventPCINT22(void);
void PinChangeInterruptEventPCINT23(void);
void PinChangeInterruptEventPCINT24(void);
void PinChangeInterruptEventPCINT25(void);
void PinChangeInterruptEventPCINT26(void);
void PinChangeInterruptEventPCINT27(void);
void PinChangeInterruptEventPCINT28(void);
void PinChangeInterruptEventPCINT29(void);
void PinChangeInterruptEventPCINT30(void);
void PinChangeInterruptEventPCINT31(void);

extern uint8_t oldPorts[PCINT_NUM_USED_PORTS];
extern uint8_t fallingPorts[PCINT_NUM_USED_PORTS];
extern uint8_t risingPorts[PCINT_NUM_USED_PORTS];


//static inline uint8_t getArrayPosPCINT(uint8_t pcintPort) __attribute__((always_inline));
uint8_t getArrayPosPCINT(uint8_t pcintPort) {
	/*
	Maps the port to the array.
	This is needed since you can deactivate ports
	and the array will dynamically resize to save ram.

	The function does not need that much flash since the if and else
	are known at compile time, so the compiler removes all the complex logic.
	The return is is the input if all pins are activated for example.
	That's why the function is inline.
	*/

	if (PCINT_NUM_USED_PORTS == 1) {
		// only the first element is used for a single port
		return 0;
	}
	else if (PCINT_NUM_USED_PORTS == PCINT_NUM_PORTS) {
		// use all ports and down remap the array position.
		return pcintPort;
	}
	else if (PCINT_NUM_PORTS - PCINT_NUM_USED_PORTS == 1) {
		// one port is not used
		if (PCINT_USE_PORT0 == 0) {
			// first port is not used, decrease all port numbers
			return (pcintPort - 1);
		}
		else if (PCINT_HAS_PORT3 == 0) {
			// 3 ports (standard)
			if (PCINT_USE_PORT2 == 0) {
				// last port not used, no mapping needed
				return pcintPort;
			}
			else {
				// worst case, port in the middle not used, remap
				return ((pcintPort >> 1) & 0x01);
				//if (pcintPort == 0) return 0;
				//else return 1;
			}
		}
		else {
			// 4 ports (special case for a few AVRs)
			if (PCINT_USE_PORT3 == 0) {
				// last port not used, no mapping needed
				return pcintPort;
			}
			else {
				// worst case, one of two ports in the middle not used, remap
				if (PCINT_USE_PORT1 == 0) {
					// port1 not used, mapping needed
					if (pcintPort == 0)
						return 0;
					else
						return pcintPort - 1;
				}
				else if (PCINT_USE_PORT2 == 0) {
					// port2 not used, mapping needed
					if (pcintPort == 3)
						return 2;
					else
						return pcintPort;
				}
			}
		}

		// use all ports and down remap the array position.
		return pcintPort;
	}
	else if (PCINT_NUM_PORTS - PCINT_NUM_USED_PORTS == 2) {
		if (PCINT_USE_PORT2 == 0 && PCINT_USE_PORT3 == 0) {
			// no need for mapping
			return pcintPort;
		}
		else if (PCINT_USE_PORT0 == 0 && PCINT_USE_PORT3 == 0) {
			// 1 offset
			return (pcintPort - 1);
		}
		else if (PCINT_USE_PORT0 == 0 && PCINT_USE_PORT1 == 0) {
			// 2 offset
			return (pcintPort - 2);
		}
		else if (PCINT_USE_PORT0 == 0 && PCINT_USE_PORT2 == 0) {
			// 2 -> 1
			return (pcintPort >> 1);
		}
		else if (PCINT_USE_PORT1 == 0 && PCINT_USE_PORT2 == 0) {
			// 3 -> 1
			return (pcintPort >> 1);
		}
		else if (PCINT_USE_PORT1 == 0 && PCINT_USE_PORT3 == 0) {
			// 3 -> 1, 1 -> 0
			return (pcintPort >> 1);
		}
	}

	// error
	return 0;
}

//================================================================================
// Attach Function (partly inlined)
//================================================================================

void enablePinChangeInterruptHelper(const uint8_t pcintPort, const uint8_t pcintMask, const uint8_t arrayPos);
void attachPinChangeInterrupt0(void);
void attachPinChangeInterrupt1(void);
void attachPinChangeInterrupt2(void);
void attachPinChangeInterrupt3(void);

#if defined(PCINT_API)

/*
for (int i = 0; i < 32; i++) {
Serial.print("#if (PCINT_USE_PCINT");
Serial.print(i);
Serial.println(" == true)");
Serial.print("extern volatile callback callbackPCINT");
Serial.print(i);
Serial.println(";");
Serial.println("#endif");
}
*/
#if (PCINT_USE_PCINT0 == true)
extern volatile callback callbackPCINT0;
#endif
#if (PCINT_USE_PCINT1 == true)
extern volatile callback callbackPCINT1;
#endif
#if (PCINT_USE_PCINT2 == true)
extern volatile callback callbackPCINT2;
#endif
#if (PCINT_USE_PCINT3 == true)
extern volatile callback callbackPCINT3;
#endif
#if (PCINT_USE_PCINT4 == true)
extern volatile callback callbackPCINT4;
#endif
#if (PCINT_USE_PCINT5 == true)
extern volatile callback callbackPCINT5;
#endif
#if (PCINT_USE_PCINT6 == true)
extern volatile callback callbackPCINT6;
#endif
#if (PCINT_USE_PCINT7 == true)
extern volatile callback callbackPCINT7;
#endif
#if (PCINT_USE_PCINT8 == true)
extern volatile callback callbackPCINT8;
#endif
#if (PCINT_USE_PCINT9 == true)
extern volatile callback callbackPCINT9;
#endif
#if (PCINT_USE_PCINT10 == true)
extern volatile callback callbackPCINT10;
#endif
#if (PCINT_USE_PCINT11 == true)
extern volatile callback callbackPCINT11;
#endif
#if (PCINT_USE_PCINT12 == true)
extern volatile callback callbackPCINT12;
#endif
#if (PCINT_USE_PCINT13 == true)
extern volatile callback callbackPCINT13;
#endif
#if (PCINT_USE_PCINT14 == true)
extern volatile callback callbackPCINT14;
#endif
#if (PCINT_USE_PCINT15 == true)
extern volatile callback callbackPCINT15;
#endif
#if (PCINT_USE_PCINT16 == true)
extern volatile callback callbackPCINT16;
#endif
#if (PCINT_USE_PCINT17 == true)
extern volatile callback callbackPCINT17;
#endif
#if (PCINT_USE_PCINT18 == true)
extern volatile callback callbackPCINT18;
#endif
#if (PCINT_USE_PCINT19 == true)
extern volatile callback callbackPCINT19;
#endif
#if (PCINT_USE_PCINT20 == true)
extern volatile callback callbackPCINT20;
#endif
#if (PCINT_USE_PCINT21 == true)
extern volatile callback callbackPCINT21;
#endif
#if (PCINT_USE_PCINT22 == true)
extern volatile callback callbackPCINT22;
#endif
#if (PCINT_USE_PCINT23 == true)
extern volatile callback callbackPCINT23;
#endif
#if (PCINT_USE_PCINT24 == true)
extern volatile callback callbackPCINT24;
#endif
#if (PCINT_USE_PCINT25 == true)
extern volatile callback callbackPCINT25;
#endif
#if (PCINT_USE_PCINT26 == true)
extern volatile callback callbackPCINT26;
#endif
#if (PCINT_USE_PCINT27 == true)
extern volatile callback callbackPCINT27;
#endif
#if (PCINT_USE_PCINT28 == true)
extern volatile callback callbackPCINT28;
#endif
#if (PCINT_USE_PCINT29 == true)
extern volatile callback callbackPCINT29;
#endif
#if (PCINT_USE_PCINT30 == true)
extern volatile callback callbackPCINT30;
#endif
#if (PCINT_USE_PCINT31 == true)
extern volatile callback callbackPCINT31;
#endif

/*
for (int i = 0; i < 32; i++) {
Serial.print("#if (PCINT_USE_PCINT");
Serial.print(i);
Serial.println(" == true)");
Serial.print("if (pcintNum == ");
Serial.print(i);
Serial.println(")");
Serial.print("callbackPCINT");
Serial.print(i);
Serial.println(" = userFunc;");
Serial.println("#endif");
}
*/

// API attach function
static inline void attachPinChangeInterrupt(const uint8_t pcintNum, void(*userFunc)(void), const uint8_t mode) __attribute__((always_inline));
void attachPinChangeInterrupt(const uint8_t pcintNum, void(*userFunc)(void), const uint8_t mode) {
#else // no API attach function
static inline void attachPinChangeInterrupt(const uint8_t pcintNum, const uint8_t mode) __attribute__((always_inline));
void attachPinChangeInterrupt(const uint8_t pcintNum, const uint8_t mode) {
#endif // PCINT_API

	// check if pcint is a valid pcint, exclude deactivated ports
	uint8_t pcintPort = pcintNum / 8;
	uint8_t pcintBit = pcintNum % 8;

	// port 0
	if (pcintPort == 0 && PCINT_USE_PORT0 == true) {
		// use fake functions to make the ISRs compile with .a linkage
#if defined(PCINT_ALINKAGE) && !defined(PCINT_COMPILE_ENABLED_ISR)
		attachPinChangeInterrupt0();
#endif

		//  attache the function pointers for the API
#if defined(PCINT_API)
#if (PCINT_USE_PCINT0 == true)
		if (pcintNum == 0)
			callbackPCINT0 = userFunc;
#endif
#if (PCINT_USE_PCINT1 == true)
		if (pcintNum == 1)
			callbackPCINT1 = userFunc;
#endif
#if (PCINT_USE_PCINT2 == true)
		if (pcintNum == 2)
			callbackPCINT2 = userFunc;
#endif
#if (PCINT_USE_PCINT3 == true)
		if (pcintNum == 3)
			callbackPCINT3 = userFunc;
#endif
#if (PCINT_USE_PCINT4 == true)
		if (pcintNum == 4)
			callbackPCINT4 = userFunc;
#endif
#if (PCINT_USE_PCINT5 == true)
		if (pcintNum == 5)
			callbackPCINT5 = userFunc;
#endif
#if (PCINT_USE_PCINT6 == true)
		if (pcintNum == 6)
			callbackPCINT6 = userFunc;
#endif
#if (PCINT_USE_PCINT7 == true)
		if (pcintNum == 7)
			callbackPCINT7 = userFunc;
#endif
#endif // PCINT_API
	}

	// port 1
	else if (pcintPort == 1 && PCINT_USE_PORT1 == true) {
		// use fake functions to make the ISRs compile with .a linkage
#if defined(PCINT_ALINKAGE) && !defined(PCINT_COMPILE_ENABLED_ISR)
		attachPinChangeInterrupt1();
#endif

		//  attache the function pointers for the API
#if defined(PCINT_API)
#if (PCINT_USE_PCINT8 == true)
		if (pcintNum == 8)
			callbackPCINT8 = userFunc;
#endif
#if (PCINT_USE_PCINT9 == true)
		if (pcintNum == 9)
			callbackPCINT9 = userFunc;
#endif
#if (PCINT_USE_PCINT10 == true)
		if (pcintNum == 10)
			callbackPCINT10 = userFunc;
#endif
#if (PCINT_USE_PCINT11 == true)
		if (pcintNum == 11)
			callbackPCINT11 = userFunc;
#endif
#if (PCINT_USE_PCINT12 == true)
		if (pcintNum == 12)
			callbackPCINT12 = userFunc;
#endif
#if (PCINT_USE_PCINT13 == true)
		if (pcintNum == 13)
			callbackPCINT13 = userFunc;
#endif
#if (PCINT_USE_PCINT14 == true)
		if (pcintNum == 14)
			callbackPCINT14 = userFunc;
#endif
#if (PCINT_USE_PCINT15 == true)
		if (pcintNum == 15)
			callbackPCINT15 = userFunc;
#endif
#endif // PCINT_API
	}

	// port 2
	else if (pcintPort == 2 && PCINT_USE_PORT2 == true) {
		// use fake functions to make the ISRs compile with .a linkage
#if defined(PCINT_ALINKAGE) && !defined(PCINT_COMPILE_ENABLED_ISR)
		attachPinChangeInterrupt2();
#endif
		//  attache the function pointers for the API
#if defined(PCINT_API)
#if (PCINT_USE_PCINT16 == true)
		if (pcintNum == 16)
			callbackPCINT16 = userFunc;
#endif
#if (PCINT_USE_PCINT17 == true)
		if (pcintNum == 17)
			callbackPCINT17 = userFunc;
#endif
#if (PCINT_USE_PCINT18 == true)
		if (pcintNum == 18)
			callbackPCINT18 = userFunc;
#endif
#if (PCINT_USE_PCINT19 == true)
		if (pcintNum == 19)
			callbackPCINT19 = userFunc;
#endif
#if (PCINT_USE_PCINT20 == true)
		if (pcintNum == 20)
			callbackPCINT20 = userFunc;
#endif
#if (PCINT_USE_PCINT21 == true)
		if (pcintNum == 21)
			callbackPCINT21 = userFunc;
#endif
#if (PCINT_USE_PCINT22 == true)
		if (pcintNum == 22)
			callbackPCINT22 = userFunc;
#endif
#if (PCINT_USE_PCINT23 == true)
		if (pcintNum == 23)
			callbackPCINT23 = userFunc;
#endif
#endif // PCINT_API
	}

	// port 3
	else if (pcintPort == 3 && PCINT_USE_PORT3 == true) {
		// use fake functions to make the ISRs compile with .a linkage
#if defined(PCINT_ALINKAGE) && !defined(PCINT_COMPILE_ENABLED_ISR)
		attachPinChangeInterrupt3();
#endif
		//  attache the function pointers for the API
#if defined(PCINT_API)
#if (PCINT_USE_PCINT24 == true)
		if (pcintNum == 24)
			callbackPCINT24 = userFunc;
#endif
#if (PCINT_USE_PCINT25 == true)
		if (pcintNum == 25)
			callbackPCINT25 = userFunc;
#endif
#if (PCINT_USE_PCINT26 == true)
		if (pcintNum == 26)
			callbackPCINT26 = userFunc;
#endif
#if (PCINT_USE_PCINT27 == true)
		if (pcintNum == 27)
			callbackPCINT27 = userFunc;
#endif
#if (PCINT_USE_PCINT28 == true)
		if (pcintNum == 28)
			callbackPCINT28 = userFunc;
#endif
#if (PCINT_USE_PCINT29 == true)
		if (pcintNum == 29)
			callbackPCINT29 = userFunc;
#endif
#if (PCINT_USE_PCINT30 == true)
		if (pcintNum == 30)
			callbackPCINT30 = userFunc;
#endif
#if (PCINT_USE_PCINT31 == true)
		if (pcintNum == 31)
			callbackPCINT31 = userFunc;
#endif
#endif // PCINT_API
	}
	else return;

	// get bitmask and array position
	uint8_t pcintMask = (1 << pcintBit);
	uint8_t arrayPos = getArrayPosPCINT(pcintPort);

	// save settings related to mode and registers
	if (mode == CHANGE || mode == RISING)
		risingPorts[arrayPos] |= pcintMask;
	if (mode == CHANGE || mode == FALLING)
		fallingPorts[arrayPos] |= pcintMask;

	// call the actual hardware attach function
	enablePinChangeInterruptHelper(pcintPort, pcintMask, arrayPos);
}

// enable interrupt again if temporary disabled
//static inline void enablePinChangeInterrupt(const uint8_t pcintNum) __attribute__((always_inline));
void enablePinChangeInterrupt(const uint8_t pcintNum) {
	// get PCINT registers
	uint8_t pcintPort = pcintNum / 8;
	uint8_t pcintBit = pcintNum % 8;

	// check if pcint is a valid pcint, exclude deactivated ports
	if (pcintPort == 0) {
		if (PCINT_USE_PORT0 == false)
			return;
	}
	else if (pcintPort == 1) {
		if (PCINT_USE_PORT1 == false)
			return;
	}
	else if (pcintPort == 2) {
		if (PCINT_USE_PORT2 == false)
			return;
	}
	else if (pcintPort == 3) {
		if (PCINT_USE_PORT3 == false)
			return;
	}
	else return;

	// call the actual hardware attach function
	uint8_t pcintMask = (1 << pcintBit);
	uint8_t arrayPos = getArrayPosPCINT(pcintPort);
	enablePinChangeInterruptHelper(pcintPort, pcintMask, arrayPos);
}


//================================================================================
// Detach Function (partly inlined)
//================================================================================

void disablePinChangeInterruptHelper(const uint8_t pcintPort, const uint8_t pcintMask);
//static inline void detachPinChangeInterrupt(const uint8_t pcintNum) __attribute__((always_inline));

void detachPinChangeInterrupt(const uint8_t pcintNum) {
	// get PCINT registers
	uint8_t pcintPort = pcintNum / 8;
	uint8_t pcintBit = pcintNum % 8;

	// check if pcint is a valid pcint, exclude deactivated ports
	// port 0
	if (pcintPort == 0 && PCINT_USE_PORT0 == true) {
		//  attache the function pointers for the API
#if defined(PCINT_API)
#if (PCINT_USE_PCINT0 == true)
		if (pcintNum == 0)
			callbackPCINT0 = pcint_null_callback;
#endif
#if (PCINT_USE_PCINT1 == true)
		if (pcintNum == 1)
			callbackPCINT1 = pcint_null_callback;
#endif
#if (PCINT_USE_PCINT2 == true)
		if (pcintNum == 2)
			callbackPCINT2 = pcint_null_callback;
#endif
#if (PCINT_USE_PCINT3 == true)
		if (pcintNum == 3)
			callbackPCINT3 = pcint_null_callback;
#endif
#if (PCINT_USE_PCINT4 == true)
		if (pcintNum == 4)
			callbackPCINT4 = pcint_null_callback;
#endif
#if (PCINT_USE_PCINT5 == true)
		if (pcintNum == 5)
			callbackPCINT5 = pcint_null_callback;
#endif
#if (PCINT_USE_PCINT6 == true)
		if (pcintNum == 6)
			callbackPCINT6 = pcint_null_callback;
#endif
#if (PCINT_USE_PCINT7 == true)
		if (pcintNum == 7)
			callbackPCINT7 = pcint_null_callback;
#endif
#endif // PCINT_API
	}

	// port 1
	else if (pcintPort == 1 && PCINT_USE_PORT1 == true) {
		//  attache the function pointers for the API
#if defined(PCINT_API)
#if (PCINT_USE_PCINT8 == true)
		if (pcintNum == 8)
			callbackPCINT8 = pcint_null_callback;
#endif
#if (PCINT_USE_PCINT9 == true)
		if (pcintNum == 9)
			callbackPCINT9 = pcint_null_callback;
#endif
#if (PCINT_USE_PCINT10 == true)
		if (pcintNum == 10)
			callbackPCINT10 = pcint_null_callback;
#endif
#if (PCINT_USE_PCINT11 == true)
		if (pcintNum == 11)
			callbackPCINT11 = pcint_null_callback;
#endif
#if (PCINT_USE_PCINT12 == true)
		if (pcintNum == 12)
			callbackPCINT12 = pcint_null_callback;
#endif
#if (PCINT_USE_PCINT13 == true)
		if (pcintNum == 13)
			callbackPCINT13 = pcint_null_callback;
#endif
#if (PCINT_USE_PCINT14 == true)
		if (pcintNum == 14)
			callbackPCINT14 = pcint_null_callback;
#endif
#if (PCINT_USE_PCINT15 == true)
		if (pcintNum == 15)
			callbackPCINT15 = pcint_null_callback;
#endif
#endif // PCINT_API
	}

	// port 2
	else if (pcintPort == 2 && PCINT_USE_PORT2 == true) {
		//  attache the function pointers for the API
#if defined(PCINT_API)
#if (PCINT_USE_PCINT16 == true)
		if (pcintNum == 16)
			callbackPCINT16 = pcint_null_callback;
#endif
#if (PCINT_USE_PCINT17 == true)
		if (pcintNum == 17)
			callbackPCINT17 = pcint_null_callback;
#endif
#if (PCINT_USE_PCINT18 == true)
		if (pcintNum == 18)
			callbackPCINT18 = pcint_null_callback;
#endif
#if (PCINT_USE_PCINT19 == true)
		if (pcintNum == 19)
			callbackPCINT19 = pcint_null_callback;
#endif
#if (PCINT_USE_PCINT20 == true)
		if (pcintNum == 20)
			callbackPCINT20 = pcint_null_callback;
#endif
#if (PCINT_USE_PCINT21 == true)
		if (pcintNum == 21)
			callbackPCINT21 = pcint_null_callback;
#endif
#if (PCINT_USE_PCINT22 == true)
		if (pcintNum == 22)
			callbackPCINT22 = pcint_null_callback;
#endif
#if (PCINT_USE_PCINT23 == true)
		if (pcintNum == 23)
			callbackPCINT23 = pcint_null_callback;
#endif
#endif // PCINT_API
	}

	// port 3
	else if (pcintPort == 3 && PCINT_USE_PORT3 == true) {
		//  attache the function pointers for the API
#if defined(PCINT_API)
#if (PCINT_USE_PCINT24 == true)
		if (pcintNum == 24)
			callbackPCINT24 = pcint_null_callback;
#endif
#if (PCINT_USE_PCINT25 == true)
		if (pcintNum == 25)
			callbackPCINT25 = pcint_null_callback;
#endif
#if (PCINT_USE_PCINT26 == true)
		if (pcintNum == 26)
			callbackPCINT26 = pcint_null_callback;
#endif
#if (PCINT_USE_PCINT27 == true)
		if (pcintNum == 27)
			callbackPCINT27 = pcint_null_callback;
#endif
#if (PCINT_USE_PCINT28 == true)
		if (pcintNum == 28)
			callbackPCINT28 = pcint_null_callback;
#endif
#if (PCINT_USE_PCINT29 == true)
		if (pcintNum == 29)
			callbackPCINT29 = pcint_null_callback;
#endif
#if (PCINT_USE_PCINT30 == true)
		if (pcintNum == 30)
			callbackPCINT30 = pcint_null_callback;
#endif
#if (PCINT_USE_PCINT31 == true)
		if (pcintNum == 31)
			callbackPCINT31 = pcint_null_callback;
#endif
#endif // PCINT_API
	}
	else return;

	// get bitmask and array position
	uint8_t pcintMask = (1 << pcintBit);
	uint8_t arrayPos = getArrayPosPCINT(pcintPort);

	// delete setting
	risingPorts[arrayPos] &= ~pcintMask;
	fallingPorts[arrayPos] &= ~pcintMask;

	// call the actual hardware disable function
	disablePinChangeInterruptHelper(pcintPort, pcintMask);
}

//static inline void disablePinChangeInterrupt(const uint8_t pcintNum) __attribute__((always_inline));
void disablePinChangeInterrupt(const uint8_t pcintNum) {
	// get PCINT registers
	uint8_t pcintPort = pcintNum / 8;
	uint8_t pcintBit = pcintNum % 8;

	// check if pcint is a valid pcint, exclude deactivated ports
	if (pcintPort == 0) {
		if (PCINT_USE_PORT0 == false)
			return;
	}
	else if (pcintPort == 1) {
		if (PCINT_USE_PORT1 == false)
			return;
	}
	else if (pcintPort == 2) {
		if (PCINT_USE_PORT2 == false)
			return;
	}
	else if (pcintPort == 3) {
		if (PCINT_USE_PORT3 == false)
			return;
	}
	else return;

	// get bitmask
	uint8_t pcintMask = (1 << pcintBit);

	// Do not delete mode settings nor detach the user function
	// Just turn off interrupts

	// call the actual hardware disable function
	disablePinChangeInterruptHelper(pcintPort, pcintMask);
}

//================================================================================
// getTrigger Function (inlined)
//================================================================================

//static inline uint8_t getPinChangeInterruptTrigger(const uint8_t pcintNum) __attribute__((always_inline));
uint8_t getPinChangeInterruptTrigger(const uint8_t pcintNum) {
	// get PCINT registers
	uint8_t pcintPort = pcintNum / 8;
	uint8_t pcintBit = pcintNum % 8;

	// check if pcint is a valid pcint, exclude deactivated ports
	if (pcintPort == 0) {
		if (PCINT_USE_PORT0 == false)
			return CHANGE;
	}
	else if (pcintPort == 1) {
		if (PCINT_USE_PORT1 == false)
			return CHANGE;
	}
	else if (pcintPort == 2) {
		if (PCINT_USE_PORT2 == false)
			return CHANGE;
	}
	else if (pcintPort == 3) {
		if (PCINT_USE_PORT3 == false)
			return CHANGE;
	}
	else return CHANGE;

	uint8_t arrayPos = getArrayPosPCINT(pcintPort);

	// Check if no mode was set, return an error
	if(!(risingPorts[arrayPos] & (1 << pcintBit)) && !(fallingPorts[arrayPos] & (1 << pcintBit)))
		return CHANGE;

	// specify the CHANGE mode
	if (oldPorts[arrayPos] & (1 << pcintBit))
		return RISING;
	else
		return FALLING;
}

/*
Copyright (c) 2014-2021 NicoHood
See the readme for credit to other people.

Permission is hereby granted, free of charge, to any person obtaining a copy
of this software and associated documentation files (the "Software"), to deal
in the Software without restriction, including without limitation the rights
to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
copies of the Software, and to permit persons to whom the Software is
furnished to do so, subject to the following conditions:

The above copyright notice and this permission notice shall be included in
all copies or substantial portions of the Software.

THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN
THE SOFTWARE.
*/

// #include "PinChangeInterrupt.h"

//================================================================================
// Interrupt Handler
//================================================================================

// prevent compilation twice if included from the .cpp to force compile all ISRs
#if defined(PCINT_ALINKAGE) && defined(PCINT_COMPILE_ENABLED_ISR) && defined(PCINT_INCLUDE_FROM_CPP) \
	|| !defined(PCINT_ALINKAGE) || !defined(PCINT_COMPILE_ENABLED_ISR)

#if (PCINT_USE_PORT0 == true)

void attachPinChangeInterrupt0(void) {
	// fake function to make the IDE link this file
}

ISR(PCINT0_vect) {
	// get the new and old pin states for port
	uint8_t newPort = PCINT_INPUT_PORT0;

	// compare with the old value to detect a rising or falling
	uint8_t arrayPos = getArrayPosPCINT(0);
	uint8_t change = newPort ^ oldPorts[arrayPos];
	uint8_t rising = change & newPort;
	uint8_t falling = change & oldPorts[arrayPos];

	// check which pins are triggered, compared with the settings
	uint8_t risingTrigger = rising & risingPorts[arrayPos];
	uint8_t fallingTrigger = falling & fallingPorts[arrayPos];
	uint8_t trigger = risingTrigger | fallingTrigger;

	// save the new state for next comparison
	oldPorts[arrayPos] = newPort;

	// Execute all functions that should be triggered
	// This way we can exclude a single function
	// and the calling is also much faster
	// We may also reorder the pins for different priority
#if !defined(PCINT_CALLBACK_PORT0)
	PCINT_CALLBACK(0, 0);
	PCINT_CALLBACK(1, 1);
	PCINT_CALLBACK(2, 2);
	PCINT_CALLBACK(3, 3);
	PCINT_CALLBACK(4, 4);
	PCINT_CALLBACK(5, 5);
	PCINT_CALLBACK(6, 6);
	PCINT_CALLBACK(7, 7);
#else
	PCINT_CALLBACK_PORT0
#endif
}

#if defined(PCINT_API)

/*
for (int i = 0; i < 32; i++) {
Serial.print("#if (PCINT_USE_PCINT");
Serial.print(i);
Serial.println(" == true)");
Serial.print("volatile callback callbackPCINT");
Serial.print(i);
Serial.println(" = pcint_null_callback;");
Serial.print("void PinChangeInterruptEventPCINT");
Serial.print(i);
Serial.println("(void){");
Serial.print("	callbackPCINT");
Serial.print(i);
Serial.println("();");
Serial.println("}");
Serial.println("#endif");
}
*/
#if (PCINT_USE_PCINT0 == true)
volatile callback callbackPCINT0 = pcint_null_callback;
void PinChangeInterruptEventPCINT0(void) {
	callbackPCINT0();
}
#endif
#if (PCINT_USE_PCINT1 == true)
volatile callback callbackPCINT1 = pcint_null_callback;
void PinChangeInterruptEventPCINT1(void) {
	callbackPCINT1();
}
#endif
#if (PCINT_USE_PCINT2 == true)
volatile callback callbackPCINT2 = pcint_null_callback;
void PinChangeInterruptEventPCINT2(void) {
	callbackPCINT2();
}
#endif
#if (PCINT_USE_PCINT3 == true)
volatile callback callbackPCINT3 = pcint_null_callback;
void PinChangeInterruptEventPCINT3(void) {
	callbackPCINT3();
}
#endif
#if (PCINT_USE_PCINT4 == true)
volatile callback callbackPCINT4 = pcint_null_callback;
void PinChangeInterruptEventPCINT4(void) {
	callbackPCINT4();
}
#endif
#if (PCINT_USE_PCINT5 == true)
volatile callback callbackPCINT5 = pcint_null_callback;
void PinChangeInterruptEventPCINT5(void) {
	callbackPCINT5();
}
#endif
#if (PCINT_USE_PCINT6 == true)
volatile callback callbackPCINT6 = pcint_null_callback;
void PinChangeInterruptEventPCINT6(void) {
	callbackPCINT6();
}
#endif
#if (PCINT_USE_PCINT7 == true)
volatile callback callbackPCINT7 = pcint_null_callback;
void PinChangeInterruptEventPCINT7(void) {
	callbackPCINT7();
}
#endif

#endif // PCINT_API

#endif // PCINT_USE_PORT0

#endif // PCINT_INCLUDE_FROM_CPP

/*
Copyright (c) 2014-2021 NicoHood
See the readme for credit to other people.

Permission is hereby granted, free of charge, to any person obtaining a copy
of this software and associated documentation files (the "Software"), to deal
in the Software without restriction, including without limitation the rights
to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
copies of the Software, and to permit persons to whom the Software is
furnished to do so, subject to the following conditions:

The above copyright notice and this permission notice shall be included in
all copies or substantial portions of the Software.

THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN
THE SOFTWARE.
*/

// #include "PinChangeInterrupt.h"

//================================================================================
// Interrupt Handler
//================================================================================

// prevent compilation twice if included from the .cpp to force compile all ISRs
#if defined(PCINT_ALINKAGE) && defined(PCINT_COMPILE_ENABLED_ISR) && defined(PCINT_INCLUDE_FROM_CPP) \
	|| !defined(PCINT_ALINKAGE) || !defined(PCINT_COMPILE_ENABLED_ISR)

#if (PCINT_USE_PORT1 == true)

void attachPinChangeInterrupt1(void) {
	// fake function to make the IDE link this file
}

ISR(PCINT1_vect) {
	// get the new and old pin states for port
	uint8_t newPort = PCINT_INPUT_PORT1;

	// compare with the old value to detect a rising or falling
	uint8_t arrayPos = getArrayPosPCINT(1);
	uint8_t change = newPort ^ oldPorts[arrayPos];
	uint8_t rising = change & newPort;
	uint8_t falling = change & oldPorts[arrayPos];

	// check which pins are triggered, compared with the settings
	uint8_t risingTrigger = rising & risingPorts[arrayPos];
	uint8_t fallingTrigger = falling & fallingPorts[arrayPos];
	uint8_t trigger = risingTrigger | fallingTrigger;

	// save the new state for next comparison
	oldPorts[arrayPos] = newPort;

	// Execute all functions that should be triggered
	// This way we can exclude a single function
	// and the calling is also much faster
	// We may also reorder the pins for different priority
#if !defined(PCINT_CALLBACK_PORT1)
	PCINT_CALLBACK(0, 8);
	PCINT_CALLBACK(1, 9);
	PCINT_CALLBACK(2, 10);
	PCINT_CALLBACK(3, 11);
	PCINT_CALLBACK(4, 12);
	PCINT_CALLBACK(5, 13);
	PCINT_CALLBACK(6, 14);
	PCINT_CALLBACK(7, 15);
#else
	PCINT_CALLBACK_PORT1
#endif
}

#if defined(PCINT_API)

/*
for (int i = 0; i < 32; i++) {
Serial.print("#if (PCINT_USE_PCINT");
Serial.print(i);
Serial.println(" == true)");
Serial.print("volatile callback callbackPCINT");
Serial.print(i);
Serial.println(" = pcint_null_callback;");
Serial.print("void PinChangeInterruptEventPCINT");
Serial.print(i);
Serial.println("(void){");
Serial.print("	callbackPCINT");
Serial.print(i);
Serial.println("();");
Serial.println("}");
Serial.println("#endif");
}
*/
#if (PCINT_USE_PCINT8 == true)
volatile callback callbackPCINT8 = pcint_null_callback;
void PinChangeInterruptEventPCINT8(void) {
	callbackPCINT8();
}
#endif
#if (PCINT_USE_PCINT9 == true)
volatile callback callbackPCINT9 = pcint_null_callback;
void PinChangeInterruptEventPCINT9(void) {
	callbackPCINT9();
}
#endif
#if (PCINT_USE_PCINT10 == true)
volatile callback callbackPCINT10 = pcint_null_callback;
void PinChangeInterruptEventPCINT10(void) {
	callbackPCINT10();
}
#endif
#if (PCINT_USE_PCINT11 == true)
volatile callback callbackPCINT11 = pcint_null_callback;
void PinChangeInterruptEventPCINT11(void) {
	callbackPCINT11();
}
#endif
#if (PCINT_USE_PCINT12 == true)
volatile callback callbackPCINT12 = pcint_null_callback;
void PinChangeInterruptEventPCINT12(void) {
	callbackPCINT12();
}
#endif
#if (PCINT_USE_PCINT13 == true)
volatile callback callbackPCINT13 = pcint_null_callback;
void PinChangeInterruptEventPCINT13(void) {
	callbackPCINT13();
}
#endif
#if (PCINT_USE_PCINT14 == true)
volatile callback callbackPCINT14 = pcint_null_callback;
void PinChangeInterruptEventPCINT14(void) {
	callbackPCINT14();
}
#endif
#if (PCINT_USE_PCINT15 == true)
volatile callback callbackPCINT15 = pcint_null_callback;
void PinChangeInterruptEventPCINT15(void) {
	callbackPCINT15();
}
#endif

#endif // PCINT_API

#endif // PCINT_USE_PORT1

#endif // PCINT_INCLUDE_FROM_CPP

/*
Copyright (c) 2014-2021 NicoHood
See the readme for credit to other people.

Permission is hereby granted, free of charge, to any person obtaining a copy
of this software and associated documentation files (the "Software"), to deal
in the Software without restriction, including without limitation the rights
to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
copies of the Software, and to permit persons to whom the Software is
furnished to do so, subject to the following conditions:

The above copyright notice and this permission notice shall be included in
all copies or substantial portions of the Software.

THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN
THE SOFTWARE.
*/

// #include "PinChangeInterrupt.h"

//================================================================================
// Interrupt Handler
//================================================================================

// prevent compilation twice if included from the .cpp to force compile all ISRs
#if defined(PCINT_ALINKAGE) && defined(PCINT_COMPILE_ENABLED_ISR) && defined(PCINT_INCLUDE_FROM_CPP) \
	|| !defined(PCINT_ALINKAGE) || !defined(PCINT_COMPILE_ENABLED_ISR)

#if (PCINT_USE_PORT2 == true)

void attachPinChangeInterrupt2(void) {
	// fake function to make the IDE link this file
}

ISR(PCINT2_vect) {
	// get the new and old pin states for port
	uint8_t newPort = PCINT_INPUT_PORT2;

	// compare with the old value to detect a rising or falling
	uint8_t arrayPos = getArrayPosPCINT(2);
	uint8_t change = newPort ^ oldPorts[arrayPos];
	uint8_t rising = change & newPort;
	uint8_t falling = change & oldPorts[arrayPos];

	// check which pins are triggered, compared with the settings
	uint8_t risingTrigger = rising & risingPorts[arrayPos];
	uint8_t fallingTrigger = falling & fallingPorts[arrayPos];
	uint8_t trigger = risingTrigger | fallingTrigger;

	// save the new state for next comparison
	oldPorts[arrayPos] = newPort;

	// Execute all functions that should be triggered
	// This way we can exclude a single function
	// and the calling is also much faster
	// We may also reorder the pins for different priority
#if !defined(PCINT_CALLBACK_PORT2)
	PCINT_CALLBACK(0, 16);
	PCINT_CALLBACK(1, 17);
	PCINT_CALLBACK(2, 18);
	PCINT_CALLBACK(3, 19);
	PCINT_CALLBACK(4, 20);
	PCINT_CALLBACK(5, 21);
	PCINT_CALLBACK(6, 22);
	PCINT_CALLBACK(7, 23);
#else
	PCINT_CALLBACK_PORT2
#endif
}

#if defined(PCINT_API)

/*
for (int i = 0; i < 32; i++) {
Serial.print("#if (PCINT_USE_PCINT");
Serial.print(i);
Serial.println(" == true)");
Serial.print("volatile callback callbackPCINT");
Serial.print(i);
Serial.println(" = pcint_null_callback;");
Serial.print("void PinChangeInterruptEventPCINT");
Serial.print(i);
Serial.println("(void){");
Serial.print("	callbackPCINT");
Serial.print(i);
Serial.println("();");
Serial.println("}");
Serial.println("#endif");
}
*/
#if (PCINT_USE_PCINT16 == true)
volatile callback callbackPCINT16 = pcint_null_callback;
void PinChangeInterruptEventPCINT16(void) {
	callbackPCINT16();
}
#endif
#if (PCINT_USE_PCINT17 == true)
volatile callback callbackPCINT17 = pcint_null_callback;
void PinChangeInterruptEventPCINT17(void) {
	callbackPCINT17();
}
#endif
#if (PCINT_USE_PCINT18 == true)
volatile callback callbackPCINT18 = pcint_null_callback;
void PinChangeInterruptEventPCINT18(void) {
	callbackPCINT18();
}
#endif
#if (PCINT_USE_PCINT19 == true)
volatile callback callbackPCINT19 = pcint_null_callback;
void PinChangeInterruptEventPCINT19(void) {
	callbackPCINT19();
}
#endif
#if (PCINT_USE_PCINT20 == true)
volatile callback callbackPCINT20 = pcint_null_callback;
void PinChangeInterruptEventPCINT20(void) {
	callbackPCINT20();
}
#endif
#if (PCINT_USE_PCINT21 == true)
volatile callback callbackPCINT21 = pcint_null_callback;
void PinChangeInterruptEventPCINT21(void) {
	callbackPCINT21();
}
#endif
#if (PCINT_USE_PCINT22 == true)
volatile callback callbackPCINT22 = pcint_null_callback;
void PinChangeInterruptEventPCINT22(void) {
	callbackPCINT22();
}
#endif
#if (PCINT_USE_PCINT23 == true)
volatile callback callbackPCINT23 = pcint_null_callback;
void PinChangeInterruptEventPCINT23(void) {
	callbackPCINT23();
}
#endif

#endif // PCINT_API

#endif // PCINT_USE_PORT2

#endif // PCINT_INCLUDE_FROM_CPP

/*
Copyright (c) 2014-2021 NicoHood
See the readme for credit to other people.

Permission is hereby granted, free of charge, to any person obtaining a copy
of this software and associated documentation files (the "Software"), to deal
in the Software without restriction, including without limitation the rights
to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
copies of the Software, and to permit persons to whom the Software is
furnished to do so, subject to the following conditions:

The above copyright notice and this permission notice shall be included in
all copies or substantial portions of the Software.

THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN
THE SOFTWARE.
*/

// #include "PinChangeInterrupt.h"

//================================================================================
// Interrupt Handler
//================================================================================

// prevent compilation twice if included from the .cpp to force compile all ISRs
#if defined(PCINT_ALINKAGE) && defined(PCINT_COMPILE_ENABLED_ISR) && defined(PCINT_INCLUDE_FROM_CPP) \
	|| !defined(PCINT_ALINKAGE) || !defined(PCINT_COMPILE_ENABLED_ISR)

#if (PCINT_USE_PORT3 == true)

void attachPinChangeInterrupt3(void) {
	// fake function to make the IDE link this file
}

ISR(PCINT3_vect) {
	// get the new and old pin states for port
	uint8_t newPort = PCINT_INPUT_PORT3;

	// compare with the old value to detect a rising or falling
	uint8_t arrayPos = getArrayPosPCINT(3);
	uint8_t change = newPort ^ oldPorts[arrayPos];
	uint8_t rising = change & newPort;
	uint8_t falling = change & oldPorts[arrayPos];

	// check which pins are triggered, compared with the settings
	uint8_t risingTrigger = rising & risingPorts[arrayPos];
	uint8_t fallingTrigger = falling & fallingPorts[arrayPos];
	uint8_t trigger = risingTrigger | fallingTrigger;

	// save the new state for next comparison
	oldPorts[arrayPos] = newPort;

	// Execute all functions that should be triggered
	// This way we can exclude a single function
	// and the calling is also much faster
	// We may also reorder the pins for different priority
#if !defined(PCINT_CALLBACK_PORT3)
	PCINT_CALLBACK(0, 24);
	PCINT_CALLBACK(1, 25);
	PCINT_CALLBACK(2, 26);
	PCINT_CALLBACK(3, 27);
	PCINT_CALLBACK(4, 28);
	PCINT_CALLBACK(5, 29);
	PCINT_CALLBACK(6, 30);
	PCINT_CALLBACK(7, 31);
#else
	PCINT_CALLBACK_PORT3
#endif
}

#if defined(PCINT_API)

/*
for (int i = 0; i < 32; i++) {
Serial.print("#if (PCINT_USE_PCINT");
Serial.print(i);
Serial.println(" == true)");
Serial.print("volatile callback callbackPCINT");
Serial.print(i);
Serial.println(" = pcint_null_callback;");
Serial.print("void PinChangeInterruptEventPCINT");
Serial.print(i);
Serial.println("(void){");
Serial.print("	callbackPCINT");
Serial.print(i);
Serial.println("();");
Serial.println("}");
Serial.println("#endif");
}
*/
#if (PCINT_USE_PCINT24 == true)
volatile callback callbackPCINT24 = pcint_null_callback;
void PinChangeInterruptEventPCINT24(void) {
	callbackPCINT24();
}
#endif
#if (PCINT_USE_PCINT25 == true)
volatile callback callbackPCINT25 = pcint_null_callback;
void PinChangeInterruptEventPCINT25(void) {
	callbackPCINT25();
}
#endif
#if (PCINT_USE_PCINT26 == true)
volatile callback callbackPCINT26 = pcint_null_callback;
void PinChangeInterruptEventPCINT26(void) {
	callbackPCINT26();
}
#endif
#if (PCINT_USE_PCINT27 == true)
volatile callback callbackPCINT27 = pcint_null_callback;
void PinChangeInterruptEventPCINT27(void) {
	callbackPCINT27();
}
#endif
#if (PCINT_USE_PCINT28 == true)
volatile callback callbackPCINT28 = pcint_null_callback;
void PinChangeInterruptEventPCINT28(void) {
	callbackPCINT28();
}
#endif
#if (PCINT_USE_PCINT29 == true)
volatile callback callbackPCINT29 = pcint_null_callback;
void PinChangeInterruptEventPCINT29(void) {
	callbackPCINT29();
}
#endif
#if (PCINT_USE_PCINT30 == true)
volatile callback callbackPCINT30 = pcint_null_callback;
void PinChangeInterruptEventPCINT30(void) {
	callbackPCINT30();
}
#endif
#if (PCINT_USE_PCINT31 == true)
volatile callback callbackPCINT31 = pcint_null_callback;
void PinChangeInterruptEventPCINT31(void) {
	callbackPCINT31();
}
#endif

#endif // PCINT_API

#endif // PCINT_USE_PORT3

#endif // PCINT_INCLUDE_FROM_CPP

/*
Copyright (c) 2014-2021 NicoHood
See the readme for credit to other people.

Permission is hereby granted, free of charge, to any person obtaining a copy
of this software and associated documentation files (the "Software"), to deal
in the Software without restriction, including without limitation the rights
to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
copies of the Software, and to permit persons to whom the Software is
furnished to do so, subject to the following conditions:

The above copyright notice and this permission notice shall be included in
all copies or substantial portions of the Software.

THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN
THE SOFTWARE.
*/

// #include "PinChangeInterrupt.h"

// manually include cpp files here to save flash if only 1 ISR is present
// or if the user knows he just wants to compile all enabled ports.
// #if defined(PCINT_ALINKAGE) && defined(PCINT_COMPILE_ENABLED_ISR)
// #define PCINT_INCLUDE_FROM_CPP
// #include "PinChangeInterrupt0.cpp"
// #include "PinChangeInterrupt1.cpp"
// #include "PinChangeInterrupt2.cpp"
// #include "PinChangeInterrupt3.cpp"
// #else

//================================================================================
// Weak Callbacks
//================================================================================

// create all weak functions which are all (if not used) alias of the pcint_null_callback above
/*
for (int i = 0; i < 32; i++) {
Serial.print("void PinChangeInterruptEventPCINT");
Serial.print(i);
Serial.println("(void) __attribute__((weak, alias(\"pcint_null_callback\")));");
}
*/
  /*
void PinChangeInterruptEventPCINT0(void) __attribute__((weak, alias("pcint_null_callback")));
void PinChangeInterruptEventPCINT1(void) __attribute__((weak, alias("pcint_null_callback")));
void PinChangeInterruptEventPCINT2(void) __attribute__((weak, alias("pcint_null_callback")));
void PinChangeInterruptEventPCINT3(void) __attribute__((weak, alias("pcint_null_callback")));
void PinChangeInterruptEventPCINT4(void) __attribute__((weak, alias("pcint_null_callback")));
void PinChangeInterruptEventPCINT5(void) __attribute__((weak, alias("pcint_null_callback")));
void PinChangeInterruptEventPCINT6(void) __attribute__((weak, alias("pcint_null_callback")));
void PinChangeInterruptEventPCINT7(void) __attribute__((weak, alias("pcint_null_callback")));
void PinChangeInterruptEventPCINT8(void) __attribute__((weak, alias("pcint_null_callback")));
void PinChangeInterruptEventPCINT9(void) __attribute__((weak, alias("pcint_null_callback")));
void PinChangeInterruptEventPCINT10(void) __attribute__((weak, alias("pcint_null_callback")));
void PinChangeInterruptEventPCINT11(void) __attribute__((weak, alias("pcint_null_callback")));
void PinChangeInterruptEventPCINT12(void) __attribute__((weak, alias("pcint_null_callback")));
void PinChangeInterruptEventPCINT13(void) __attribute__((weak, alias("pcint_null_callback")));
void PinChangeInterruptEventPCINT14(void) __attribute__((weak, alias("pcint_null_callback")));
void PinChangeInterruptEventPCINT15(void) __attribute__((weak, alias("pcint_null_callback")));
void PinChangeInterruptEventPCINT16(void) __attribute__((weak, alias("pcint_null_callback")));
void PinChangeInterruptEventPCINT17(void) __attribute__((weak, alias("pcint_null_callback")));
void PinChangeInterruptEventPCINT18(void) __attribute__((weak, alias("pcint_null_callback")));
void PinChangeInterruptEventPCINT19(void) __attribute__((weak, alias("pcint_null_callback")));
void PinChangeInterruptEventPCINT20(void) __attribute__((weak, alias("pcint_null_callback")));
void PinChangeInterruptEventPCINT21(void) __attribute__((weak, alias("pcint_null_callback")));
void PinChangeInterruptEventPCINT22(void) __attribute__((weak, alias("pcint_null_callback")));
void PinChangeInterruptEventPCINT23(void) __attribute__((weak, alias("pcint_null_callback")));
void PinChangeInterruptEventPCINT24(void) __attribute__((weak, alias("pcint_null_callback")));
void PinChangeInterruptEventPCINT25(void) __attribute__((weak, alias("pcint_null_callback")));
void PinChangeInterruptEventPCINT26(void) __attribute__((weak, alias("pcint_null_callback")));
void PinChangeInterruptEventPCINT27(void) __attribute__((weak, alias("pcint_null_callback")));
void PinChangeInterruptEventPCINT28(void) __attribute__((weak, alias("pcint_null_callback")));
void PinChangeInterruptEventPCINT29(void) __attribute__((weak, alias("pcint_null_callback")));
void PinChangeInterruptEventPCINT30(void) __attribute__((weak, alias("pcint_null_callback")));
void PinChangeInterruptEventPCINT31(void) __attribute__((weak, alias("pcint_null_callback")));

##endif // PCINT_INCLUDE_FROM_CPP
*/
// useless function for weak implemented/not used functions, extern c needed for the alias
extern "C" {
	void pcint_null_callback(void) {
		// useless
	}
}

//================================================================================
// PinChangeInterrupt User Functions
//================================================================================

// variables to save the last port states and the interrupt settings
uint8_t oldPorts[PCINT_NUM_USED_PORTS] = { 0 };
uint8_t fallingPorts[PCINT_NUM_USED_PORTS] = { 0 };
uint8_t risingPorts[PCINT_NUM_USED_PORTS] = { 0 };

void enablePinChangeInterruptHelper(const uint8_t pcintPort, const uint8_t pcintMask, const uint8_t arrayPos){
	// Update the old state to the actual state
	switch(pcintPort){
#ifdef PCINT_INPUT_PORT0_USED
		case 0:
			oldPorts[arrayPos] = PCINT_INPUT_PORT0;
		break;
#endif
#ifdef PCINT_INPUT_PORT1_USED
		case 1:
			oldPorts[arrayPos] = PCINT_INPUT_PORT1;
		break;
#endif
#ifdef PCINT_INPUT_PORT2_USED
		case 2:
			oldPorts[arrayPos] = PCINT_INPUT_PORT2;
		break;
#endif
#ifdef PCINT_INPUT_PORT3_USED
		case 3:
			oldPorts[arrayPos] = PCINT_INPUT_PORT3;
		break;
#endif
	}

	// Pin change mask registers decide which pins are ENABLE as triggers
#ifdef PCMSK0
#ifdef PCMSK1
#ifdef PCMSK3
	// Special case for ATmega1284P where PCMSK3 is not directly after PCMSK2
	if(false){
#else
	// Special case for Attinyx4 where PCMSK1 and PCMSK0 are not next to each other
	if(&PCMSK1 - &PCMSK0 == 1){
#endif
#endif
		*(&PCMSK0 + pcintPort) |= pcintMask;
#ifdef PCMSK1
	}
	else{
		switch(pcintPort){
			case 0:
				PCMSK0 |= pcintMask;
			break;
			case 1:
				PCMSK1 |= pcintMask;
			break;
#ifdef PCMSK2
			case 2:
				PCMSK2 |= pcintMask;
			break;
#endif
#ifdef PCMSK3
			case 3:
				PCMSK3 |= pcintMask;
			break;
#endif
		}
	}
#endif
#elif defined(PCMSK)
	*(&PCMSK + pcintPort) |= pcintMask;
#endif

	// PCICR: Pin Change Interrupt Control Register - enables interrupt vectors
#ifdef PCICR
	PCICR |= (1  << (pcintPort + PCIE0));
#elif defined(GICR) /* e.g. ATmega162 */
	GICR |= (1  << (pcintPort + PCIE0));
#elif defined(GIMSK) && defined(PCIE0) /* e.g. ATtiny X4 */
	GIMSK |= (1  << (pcintPort + PCIE0));
#elif defined(GIMSK) && defined(PCIE) /* e.g. ATtiny X5 */
	GIMSK |= (1  << (pcintPort + PCIE));
#else
#error MCU has no such a register
#endif
}

void disablePinChangeInterruptHelper(const uint8_t pcintPort, const uint8_t pcintMask) {
	bool disable = false;
#ifdef PCMSK0
#ifdef PCMSK1
#ifdef PCMSK3
	// Special case for ATmega1284P where PCMSK3 is not directly after PCMSK2
	if (false){
#else
	// Special case for Attinyx4 where PCMSK1 and PCMSK0 are not next to each other
	if (&PCMSK1 - &PCMSK0 == 1) {
#endif // ifdef PCMSK3
#endif // ifdef PCMSK1
		// disable the mask.
		*(&PCMSK0 + pcintPort) &= ~pcintMask;

		// if that's the last one, disable the interrupt.
		if (*(&PCMSK0 + pcintPort) == 0) {
			disable = true;
		}
#ifdef PCMSK1
	}
	else {
		switch (pcintPort) {
			case 0:
				// disable the mask.
				PCMSK0 &= ~pcintMask;

				// if that's the last one, disable the interrupt.
				if (!PCMSK0) {
					disable = true;
				}
			break;
			case 1:
				// disable the mask.
				PCMSK1 &= ~pcintMask;

				// if that's the last one, disable the interrupt.
				if (!PCMSK1) {
					disable = true;
				}
			break;
#ifdef PCMSK2
			case 2:
				// disable the mask.
				PCMSK2 &= ~pcintMask;

				// if that's the last one, disable the interrupt.
				if (!PCMSK2) {
					disable = true;
				}
			break;
#endif // ifdef PCMSK2
#ifdef PCMSK3
			case 3:
				// disable the mask.
				PCMSK3 &= ~pcintMask;

				// if that's the last one, disable the interrupt.
				if (!PCMSK3) {
					disable = true;
				}
			break;
#endif // ifdef PCMSK3
		}
	}
#endif // ifdef PCMSK1
#elif defined(PCMSK)
	// disable the mask.
	*(&PCMSK + pcintPort) &= ~pcintMask;

	// if that's the last one, disable the interrupt.
	if (*(&PCMSK + pcintPort) == 0) {
		disable = true;
	}
#endif // ifdef PCMSK0

	if(disable)
	{
#ifdef PCICR
		PCICR &= ~(1  << (pcintPort + PCIE0));
#elif defined(GICR) /* e.g. ATmega162 */
		GICR &= ~(1  << (pcintPort + PCIE0));
#elif defined(GIMSK) && defined(PCIE0) /* e.g. ATtiny X4 */
		GIMSK &= ~(1  << (pcintPort + PCIE0));
#elif defined(GIMSK) && defined(PCIE) /* e.g. ATtiny X5 */
		GIMSK &= ~(1  << (pcintPort + PCIE));
#else
#error MCU has no such a register
#endif
	}
}

/*
asm output (nothing to optimize here)

ISR(PCINT0_vect) {
push r1
push r0
in r0, 0x3f ; 63
push r0
eor r1, r1
push r18
push r19
push r20
push r21
push r22
push r23
push r24
push r25
push r26
push r27
push r28
push r30
push r31

// get the new and old pin states for port
// uint8_t newPort = pinChangeInterruptPortToInput(port);
in r24, 0x03; 3 //(1) loads byte into newPort from I/O register

// loads old port and high + low setting
lds r18, 0x011E //(1 or 2) loads oldPorts into register
lds r28, 0x011B //(1 or 2) loads fallingPorts into register
lds r25, 0x0118 //(1 or 2) loads risingPorts into register

and	r28, r18 // oldPorts & fallingPorts
and	r25, r24 // newPort & risingPorts
or	r28, r25 // (oldPorts & fallingPorts) | (newPort & risingPorts)
eor r18, r24 // oldPorts^newPort
and	r28, r18 // ((oldPorts & fallingPorts) | (newPort & risingPorts)) & (oldPorts^newPort)

// save the new state for next comparison
// oldPorts[arrayPos] = newPort;
sts 0x011E, r24

// Execute all functions that should be triggered
sbrc r28, 0
call 0x37c ; 0x37c <_Z20PinChangeInterruptEventPCINT0v>
sbrc r28, 1
call 0x37c ; 0x37c <_Z20PinChangeInterruptEventPCINT0v>
sbrc r28, 2
call 0x318 ; 0x318 <_Z20PinChangeInterruptEventPCINT2v>
sbrc r28, 3
call 0x340 ; 0x340 <_Z20PinChangeInterruptEventPCINT3v>
sbrc r28, 4
call 0x37c ; 0x37c <_Z20PinChangeInterruptEventPCINT0v>
sbrc r28, 5
call 0x37c ; 0x37c <_Z20PinChangeInterruptEventPCINT0v>
sbrc r28, 6
call 0x37c ; 0x37c <_Z20PinChangeInterruptEventPCINT0v>
sbrc r28, 7
call 0x37c ; 0x37c <_Z20PinChangeInterruptEventPCINT0v>

pop r31
pop r30
pop r28
pop r27
pop r26
pop r25
pop r24
pop r23
pop r22
pop r21
pop r20
pop r19
pop r18
pop r0
out 0x3f, r0 ; 63
pop r0
pop r1
reti
}

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
  OCR1A = 12500;        // set COMPA Register
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

    Serial.println("Setup complete.");
}

/*
*/
void loop() {
    // nada
}
