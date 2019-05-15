
void UART2_IRQHandler(void);
extern void uart2_init(int baudrate);
extern void tx_cadena_UART2(char *ptr);
static int uart2_set_baudrate(unsigned int baudrate);
void config_uart_2(void);
void clear_buffer_2(void);



