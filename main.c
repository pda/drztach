#include <avr/interrupt.h>

#include "display.h"
#include "capture.h"

void setup();
void loop();

int main() {
  setup();
  for (;;) {
    loop();
  }
  return 0;
}

void setup() {
  capture_setup();
  display_setup();
  display_test();
  sei();
}

void loop() {
  capture_loop();
}
