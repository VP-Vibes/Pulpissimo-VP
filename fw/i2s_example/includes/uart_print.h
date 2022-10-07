// ###### #    # #    # ##### # ######  ####  
// #      #   #  #    #   #   # #      #      
// #####  ####   #    #   #     #####   ####  
// #      #  #   #    #   #   # #           # 
// #      #   #  #    #   #   # #      #    # 
// ###### #    #  ####    #   # ######  ####  
// ########################################## :: printf using uart

#ifndef UART_PRINT_H
#define UART_PRINT_H
#define UART_BUFFER_SIZE 256

#include <rt/rt_api.h>
#include <stdarg.h>

void uart_printf(rt_uart_t *uart, const char *format, ...);

#endif // UART_PRINT_H
