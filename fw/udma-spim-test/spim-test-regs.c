


#include <stdint.h>
#include <stdio.h>
#include <unistd.h>

#include "encoding.h"
#include "platform.h"

int main() {
  *(volatile int *)(0x1a102100) = 0x45;

  return 0;
}
