#include <stdio.h>

#include "sample_buffer.h"

void sample_buffer_init(struct sample_buffer * sb, uint8_t depth) {
  sb->depth = depth;
  uint8_t last = depth - 1;
  struct sample_buffer_item * item = sb->ring;
  for (uint8_t i = 0; i <= last; ++i) {
    item->id = i;
    item->sample = 0;
    item->prev = (i == 0) ? item + last : item - 1;
    item->next = (i == last) ? item - last : item + 1;
    item = item->next;
  }
}

void sample_buffer_write(struct sample_buffer * sb, uint16_t s) {
  struct sample_buffer_item * item = sb->ring;
  item->sample = s;
  sb->ring = item->next;
}

uint16_t sample_buffer_avg(struct sample_buffer * sb) {
  uint32_t accumulator = 0;
  struct sample_buffer_item * item = sb->ring;
  for (uint8_t i = 0; i < sb->depth; ++i) {
    accumulator += item->sample;
    item = item->next;
  }
  return accumulator / sb->depth;
}

/**
 * Average of most recent time samples recorded up to `t` units ago.
 * Prevents old small values dragging a recent large value down,
 * and thus prevents lag caused by slow samples arriving.
 * e.g. oldest->newest: {20, 18, 22, 20, 10000}
 * sample_buffer_avg(sb) == 2016
 * sample_buffer_avg_recent(sb, 1000) === 10000
 *
 */
uint16_t sample_buffer_avg_recent(struct sample_buffer * sb, uint16_t t) {
  uint32_t accumulator = 0;
  uint8_t samples = 0;
  struct sample_buffer_item * item = sb->ring->prev;
  for (uint8_t i = 0; i < sb->depth; ++i) {
    accumulator += item->sample;
    ++samples;
    if (accumulator >= t) {
      break;
    } else {
      item = item->prev;
    }
  }
  return accumulator / samples;
}
