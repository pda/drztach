#include <stdio.h>
#include <assert.h>

#include "sample_buffer.h"

struct sample_buffer_item items[256];
struct sample_buffer sample_buffer = {
  .ring = items
};
struct sample_buffer * sb = &sample_buffer;

void dump_sb(struct sample_buffer * sb) {
  struct sample_buffer_item * item = sb->ring;
  printf("sample_buffer:\n");
  for (uint8_t i = 0; i < sb->depth; ++i) {
    printf(
      "  %u <-[%u:%5u]-> %u\n",
      item->prev->id,
      item->id,
      item->sample,
      item->next->id
    );
    item = item->next;
  }
  printf("  avg: %u\n", sample_buffer_avg(sb));
}

int main() {
  sample_buffer_init(sb, 3);
  assert(sample_buffer_avg(sb) == 0);
  sample_buffer_write(sb, 10);
  sample_buffer_write(sb, 10);
  sample_buffer_write(sb, 10);
  sample_buffer_write(sb, 10);
  sample_buffer_write(sb, 1000);
  sample_buffer_write(sb, 2000);
  sample_buffer_write(sb, 4000);
  dump_sb(sb);
  assert(sample_buffer_avg(sb) == 2333);

  sample_buffer_init(sb, 8);
  assert(sample_buffer_avg(sb) == 0);
  sample_buffer_write(sb,  100);
  sample_buffer_write(sb,  200);
  sample_buffer_write(sb,  150);
  sample_buffer_write(sb, 6000);
  sample_buffer_write(sb, 8000);
  dump_sb(sb);
  assert(sample_buffer_avg_recent(sb,  4000) == 8000);
  assert(sample_buffer_avg_recent(sb,  8000) == 8000);
  assert(sample_buffer_avg_recent(sb,  8001) == 7000);
  assert(sample_buffer_avg_recent(sb, 14010) == 4716);

  return 0;
}
