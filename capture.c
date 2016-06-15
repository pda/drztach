#include <avr/interrupt.h>
#include <avr/io.h>
#include <util/delay.h>

#include "capture.h"
#include "sample_buffer.h"
#include "display.h" // remove me; externalize display

#define TACH_SAMPLE_DEPTH 8
#define TACH_IN_MASKB 0b00000001
#define TACH_PRESCALE 256

struct sample_buffer_item items[TACH_SAMPLE_DEPTH];
struct sample_buffer sample_buffer = {
  .ring = items
};

void capture_setup_timer();

ISR(TIMER1_CAPT_vect) {
  // reset timer
  TCNT1H = 0;
  TCNT1L = 0;
  // read captured value
  uint8_t low = ICR1L;
  uint8_t high = ICR1H;
  uint16_t value = ((uint16_t)high << 8) | low;
  // store
  sample_buffer_write(&sample_buffer, value);
}

ISR(TIMER1_OVF_vect) {
  // crude timeout; zero frequency
  for (uint8_t i = 0; i < sample_buffer.depth; i++) {
    sample_buffer_write(&sample_buffer, 0);
  }
}

void capture_setup() {
  sample_buffer_init(&sample_buffer, TACH_SAMPLE_DEPTH);
  capture_setup_timer();
}

// On rising edge TIMER1_CAPT_vect interrupts with timer copied to ICR1.
void capture_setup_timer() {
  // set pin as input
  DDRB &= ~TACH_IN_MASKB;

  // ensure Timer1 enabled in Power Reduction Register
  PRR &= ~_BV(PRTIM1);

  // ensure Timer1 Waveform Generation: normal mode (0)
  TCCR1A &= ~(_BV(WGM10) | _BV(WGM11));
  TCCR1B &= ~(_BV(WGM12) | _BV(WGM13));
  
  // enable Timer1 Input Capture Noise Canceller
  TCCR1B |= _BV(ICNC1);

  // set Timer1 Input Capture Edge Select to rising
  TCCR1B |= _BV(ICES1);

  // set Timer1 Clock Select 0b100 == 256 prescaler.
  uint8_t cs_bits = _BV(CS12) | _BV(CS11) | _BV(CS10);
  TCCR1B = (TCCR1B & ~cs_bits) | _BV(CS12);

  // TODO: better timeout (0 Hz) detection
  // - Output Compare: OCR1A value, OCIE1A flag in TIMSK1

  // ICIE1: Timer1 Input Capture Interrupt Enable
  // TOIE1: Timer1 Overflow Interrupt Enable
  TIMSK1 |= _BV(ICIE1) | _BV(TOIE1);
}

void capture_loop() {
  uint32_t ticks = (uint32_t)sample_buffer_avg(&sample_buffer);
  uint8_t rpm1k;
  if (ticks == 0) {
    rpm1k = 0;
  } else {
    uint32_t us = (ticks * TACH_PRESCALE) / (F_CPU / 1000000);
    uint16_t hz = 1000000 / us;
    uint16_t rpm = hz * 60;
    rpm1k = rpm / 1000;
  }

  // TODO: expose data, externalize display
  display_show(rpm1k);
}
