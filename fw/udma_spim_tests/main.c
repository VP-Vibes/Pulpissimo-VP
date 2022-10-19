

#include "rt/rt_api.h"

// receive data from external device using uDMA via SPIM the transfer takes place synchronously i.e. the CPU execution
// is blocked till this function returns
int testReceiveSync() {
  // this is the pulp-rt-examples in pulp-platform
  int buffer_size = 16;

  rt_spim_conf_t conf;
  rt_spim_conf_init(&conf);
  conf.max_baudrate = 1000000;
  conf.id = 0;
  conf.cs = 1;

  // open the device
  rt_spim_t *spim = rt_spim_open(NULL, &conf, NULL);
  if (spim == NULL) {
    return -1;
  }


  // allocate a buffer in L2-mem for receiving 16B of data
  char *rx_buffer = rt_alloc(RT_ALLOC_PERIPH, buffer_size);
  if (rx_buffer == NULL) {
    return -1;
  }

  // receive the buffer from SPI using basic synchronous mode
  // this will block until the whole transfer is done
  rt_spim_receive(spim, rx_buffer, buffer_size * 8, RT_SPIM_CS_AUTO, NULL);


  return 0;
}

// receive data from external device using uDMA via SPIM the transfer takes place asynchronously i.e. the CPU execution
// is not blocked till this function returns the CPU can do other stuff till an interrupt is received for end of
// transfer by uDMA
int testReceiveAsync() {
  return 0;
}

int testTransmitSync() {
  return 0;
}

int testTransmitAsync() {
  return 0;
}

int main() {
  int status1 = testReceiveSync();
  int status2 = testReceiveAsync();
  int status3 = testTransmitSync();
  int status4 = testTransmitAsync();

  if (status1 == 0) {
    *(volatile int *)(0x1a104074) = 0x45;
  } else if (status1 == -1) {
    *(volatile int *)(0x1a104074) = 0x23;
  }

  return 0;
}