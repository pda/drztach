#include <avr/io.h>
#include <stdint.h>
#include <util/delay.h>

#include "display.h"

#define TACH_MASKC 0b00111100 // Arduino A2-5
#define TACH_MASKD 0b11111100 // Arduino D2-7

uint8_t tach_mapc[] = {
  0b00000000, // 0
  0b00000100, // 1
  0b00001100, // 2
  0b00011100, // 3
  0b00111100, // 4
  0b00111100, // 5
  0b00111100, // 6
  0b00111100, // 7
  0b00111100, // 8
  0b00111100, // 9
  0b00111100 // 10
};

uint8_t tach_mapd[] = {
  0b00000000, // 0
  0b00000000, // 1
  0b00000000, // 2
  0b00000000, // 3
  0b00000000, // 4
  0b00000100, // 5
  0b00001100, // 6
  0b00011100, // 7
  0b00111100, // 8
  0b01111100, // 9
  0b11111100 // 10
};

void display_setup() {
  DDRC |= TACH_MASKC;
  DDRD |= TACH_MASKD;
}

void display_show(uint8_t n) {
  PORTC = (PORTC & ~TACH_MASKC) | tach_mapc[n];
  PORTD = (PORTD & ~TACH_MASKD) | tach_mapd[n];
}

void display_test() {
  uint8_t i = 0;
  for (i = 0; i <= 9; i++) {
    display_show(i);
    _delay_ms(20);
  }
  for (i = 10; i >= 1; i--) {
    display_show(i);
    _delay_ms(80);
  }
  display_show(0);
}
