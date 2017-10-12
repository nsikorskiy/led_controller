

#define UART_TX_BUFFER_SIZE 64
#define UART_RX_BUFFER_SIZE 64

#define UART_485_TRANSMIT_PORT_PIN PORTD2
#define UART_485_TRANSMIT_PORT_CONF DDRD
#define UART_485_TRANSMIT_PORT PORTD

#define UART_485_NL '\r' // \r for proteus \n for normal system

int uart_init();
int uart_write(unsigned char *buf, int8_t size);
int uart_read_size(void);
int uart_read_tobuf(unsigned char *buf);
