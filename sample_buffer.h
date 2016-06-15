#include <stdint.h>

struct sample_buffer_item {
  uint16_t sample;
  struct sample_buffer_item * next;
  struct sample_buffer_item * prev;
  uint8_t id;
};

struct sample_buffer {
  uint8_t depth;
  struct sample_buffer_item * ring;
};

void sample_buffer_init(struct sample_buffer *, uint8_t depth);
void sample_buffer_write(struct sample_buffer *, uint16_t);
uint16_t sample_buffer_avg(struct sample_buffer *);
uint16_t sample_buffer_avg_recent(struct sample_buffer *, uint16_t t);
