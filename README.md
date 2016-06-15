DR-Z400SM Tachometer
====================

The DR-Z400SM and other DRZ 400 models don't have a tachometer. This will be one.

Output: a 10 LED left-to-right bar graph, with each LED representing 1000 RPM, showing up to the 10,000 RPM redline. Choose LED colors to indicate power band, near redline etc.

Input: the stator has a pickup coil which sends a pulse to the CDI to trigger discharge via the spark coil once per 360 degree crank turn, which is twice per 720 degree four-stroke cycle. Either the relatively low voltage pulse from the stator, or an inductor wire coiled around the spark lead, will be filtered to produce a clean safe 5V rising edge per spark.

Bonus input/output: photoresistor (or cadmium-free phototransistor?) to detect low ambient light and dim LEDs via PWM switching transistor.

AVR ATmega328P microcontroller chosen because;

- enough pins to direct drive 10 LEDs, signal input, light sensor, PWM output, etc.
- suitable 16-bit timer with prescaler, input capture mode, basic noise rejection.
- easy to prototype using Arduino hardware.
- good compiler / lib: avr-gcc and avr-libc
- internal 8 MHz oscillator should suffice; low component count.

Approximate BOM, details later:

- custom circuit board
-  1 x ATmega328P
- 10 x LED
- 10 x resistor
-  1 x photoresistor
-  1 x transistor
-  1 x 5V regulator
-  1 x capacitor; electrolytic (power)
-  1 x capacitor; ceramic (decoupling)
-  1 x power connector
-  1 x sensor/pickup wire
-  1 x hall effect sensor?


TODO / NOTES
------------

* 16-bit Timer1 with 256x or 64x prescaler
    * pages 113, 120, 141 of ATmega328P datasheet.
* Input Capture Unit? PORTB0, arduino D8 (pg 120)
    * has Input Capture Noise Canceler (ICNC1 in TCCR1B)
* Handle timout; drop display to zero etc.
    * use timer overflow or compare interrupt
* Sample depth?
    * multiple means high latency for low revs
    * time-based? store 100ms (100,000 us) of data
    * write fixed depth, read latest until all or seen >= 100ms
    * read until 100ms seen or all sample read, avg.

```
   100 RPM spark 300,000 us
 1,000 RPM spark  30,000 us
10,000 RPM spark   3,000 us

8 Mhz CPU              0.125 us
 16-bit overflow   8,192 us
  8x prescaler    65,536 us (precision:  1 us)
 64x prescaler   524,288 us (precision:  8 us)
256x prescaler 2,097,152 us (precision: 32 us)

16 MHz CPU             0.0625 us
 16-bit overflow   4,096 us
  8x prescaler    32,768 us (precision:  0.5 us)
 64x prescaler   262,144 us (precision:  4 us)
256x prescaler 1,048,576 us (precision: 16 us)

       90 Hz test frequency from 555 timer
   11,111 us
      694 ticks at 16 us (16 MHz with prescale=256)

x     256 prescaler
= 177,664 unscaled ticks at 0.0625 us
  177,664 * (1,000,000 / 16,000,000) us
=  11,104 us
1,000,000 / 11,104 = 90 Hz
*      60
=    5403 RPM
```
