#include "rt/rt_api.h"

#define AI_CHUNKSIZE 128

int16_t input[AI_CHUNKSIZE];

#define SAMPLING_FREQ 10000              // 10kHz frequency
#define ELEM_SIZE 2                      // 16 bit per sample (2 bytes)
#define NB_ELEM 32                       // number of samples per capture
#define BUFF_SIZE (NB_ELEM * ELEM_SIZE)  // buffer size in bytes (equal to half input array size (AI_CHUNKSIZE/2))

// use two buffers for optimal performance and to avoid loosing audio samples
__attribute__((section(".l2_data"))) uint8_t buffA[BUFF_SIZE];  // 1. buffer
__attribute__((section(".l2_data"))) uint8_t buffB[BUFF_SIZE];  // 2. buffer

// each buffer is coupled to an event
rt_event_t *eventA;
rt_event_t *eventB;

// i2s handler
rt_i2s_t *i2s;

// initialization of i2s peripheral and events
int init_i2s() {
  if (rt_event_alloc(NULL, 1)) return -1;

  rt_i2s_conf_t i2s_conf;
  rt_i2s_conf_init(&i2s_conf);
  i2s_conf.frequency = SAMPLING_FREQ;
  i2s_conf.dual = 0;
  i2s_conf.width = 16;
  i2s_conf.id = 0;
  i2s_conf.pdm = 0;

  i2s = rt_i2s_open(NULL, &i2s_conf, NULL);
  if (i2s == NULL) return -1;

  eventA = rt_event_get_blocking(NULL);
  eventB = rt_event_get_blocking(NULL);

  rt_i2s_capture(i2s, buffA, BUFF_SIZE, eventA);
  rt_i2s_capture(i2s, buffB, BUFF_SIZE, eventB);

  rt_i2s_start(i2s);

  return 0;
}

// terminates and releases i2s resources
void close_i2s() {
  rt_i2s_stop(i2s);
  rt_i2s_close(i2s, NULL);
}

// event-based audio samples extraction from i2s peripheral and calculation by NN
void process_audio_samples() {
  rt_event_t *current_event = eventA;
  uint8_t *current_buffer = buffA;
  const uint32_t CHUNKS_TO_COMPUTE = 3;

  for (int block_counter = 0; block_counter < CHUNKS_TO_COMPUTE; block_counter++) {
    // PRINT(h_uart, "Start of chunk %d of %d...\n", block_counter+1, CHUNKS_TO_COMPUTE);

    // wait until the current buffer is full
    // PRINT(h_uart, "Wait for I2S event...\n");
    rt_event_wait(current_event);

    // prepare buffer for next capture
    rt_i2s_capture(i2s, current_buffer, BUFF_SIZE, current_event);

    // some computation with printf
    // PRINT(h_uart, "Start computation\n");

    // prepare input buffer for next chunk processing
    // copy second half of input buffer to the first half of input buffer
    memcpy(input, &input[AI_CHUNKSIZE / 2], AI_CHUNKSIZE / 2);

    // switch buffer
    if (current_event == eventA) {
      current_event = eventB;
      current_buffer = buffB;
      eventA = rt_event_get_blocking(NULL);
    } else {
      current_event = eventA;
      current_buffer = buffA;
      eventB = rt_event_get_blocking(NULL);
    }
  }
}

int main(void) {
  int return_value = -1;

  __rt_freq_set_value(RT_FREQ_DOMAIN_FC, 200000000);
  __rt_freq_set_value(RT_FREQ_DOMAIN_PERIPH, 100000000);

  return_value = init_i2s();
  process_audio_samples();
  // close_i2s();
  // return_value = 0;
  // rt_uart_close(h_uart, NULL);

  return return_value;
}
