#include "display.h"

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
  display_setup();
  display_test();
}

void loop() {
}
