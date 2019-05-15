/*
 * uart_.h
 *
 *  Created on: 1-Oct-2011
 *  Author: J.M.V.C.
 */


void UART0_IRQHandler(void);
extern void uart0_init(int baudrate);
extern void tx_cadena_UART0(char *ptr);
static int uart0_set_baudrate(unsigned int baudrate);
void config_uart(void);
void clear_buffer(void);


