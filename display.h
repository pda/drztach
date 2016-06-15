#include <stdint.h>

typedef enum tach_err_t {
  TACH_ERR_DISPLAY_OOB = 1
} tach_err_t;

void display_setup();
void display_test();
void display_show(uint8_t);
void display_error(tach_err_t);
