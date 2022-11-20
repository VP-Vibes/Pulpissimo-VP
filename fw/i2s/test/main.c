

#include "rt/rt_api.h"

#define SAMPLING_FREQ 10000
#define NB_ELEM 128
#define ELEM_SIZE 2
#define BUFF_SIZE (NB_ELEM * ELEM_SIZE)

#define NOTIFY_SIMULATOR

static uint8_t buff[BUFF_SIZE];

int main() {
  if (rt_event_alloc(NULL, 1)) {
    return -1;
  }
  rt_i2s_conf_t i2s_conf;
  rt_i2s_conf_init(&i2s_conf);
  i2s_conf.frequency = SAMPLING_FREQ;
  i2s_conf.dual = 0;
  i2s_conf.width = 16;
  i2s_conf.id = 0;
  // TODO: so far only works with PDM disabled!!
  i2s_conf.pdm = 0;

  rt_i2s_t *i2s = rt_i2s_open(NULL, &i2s_conf, NULL);
  if (i2s == NULL) {
    return -1;
  }

  rt_event_t *event = rt_event_get_blocking(NULL);
  rt_i2s_capture(i2s, buff, BUFF_SIZE, event);
  rt_i2s_start(i2s);
  rt_event_wait(event);
  rt_i2s_stop(i2s);
  rt_i2s_close(i2s, NULL);

  // to indicate to simulator environment that program finished successfully
#ifdef NOTIFY_SIMULATOR
  *(volatile int *)(0x1a104074) = 0x99;
#endif

  return 0;
}
