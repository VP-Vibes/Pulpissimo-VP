// ###### #    # #    # ##### # ######  ####  
// #      #   #  #    #   #   # #      #      
// #####  ####   #    #   #     #####   ####  
// #      #  #   #    #   #   # #           # 
// #      #   #  #    #   #   # #      #    # 
// ###### #    #  ####    #   # ######  ####  
// ########################################## :: printf using uart

#include "uart_print.h"
#include <rt/rt_api.h>
#include <stdarg.h>

void uart_printf(rt_uart_t *uart, const char *format, ...) {
	va_list vargs;
	va_start(vargs, format);
	char tx_buffer[UART_BUFFER_SIZE];
	int length = vsnprintf(tx_buffer, UART_BUFFER_SIZE, format, vargs);
	va_end(vargs);
	tx_buffer[length] = '\n';
    // No event is specified so we are doing a blocking call, which means
    // we are blocked on this call until the whole buffer is sent
    rt_uart_write(uart, tx_buffer, length + 1, NULL);
}
