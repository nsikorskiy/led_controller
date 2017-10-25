
#ifndef L_UART485_H
#define L_UART485_H

#define UART_F 38400L
#define UART_UBRR0 (F_CPU / (16 * UART_F)) - 1

#define UART_TX_BUFFER_SIZE 64
#define UART_RX_BUFFER_SIZE 64



//Flags Timer
#define    TX_TIME  0
//Flags TX
#define    TX_IN_PROGRESS 1
#define    TX_PING 2
#define    TX_BUFFER_READY 3
#define    TX_SEND_RX_ERROR 4
//Flags RX
#define    RX_IN_PROGRESS 5
#define    RX_BUFFER_READY 6
#define    RX_ERROR 7
//volatile uint8_t uart_flags = 0;

#define    ERR_RX_OVERFLOW 5 //должно быть 5 %) чтоб не пресеклось с битами ошибок получения FE0, DOR0, UPE0
#define    ERR_COUNTER_MASK 1 //'00000001'
#define    ERR_CLEAN 1 
#define    CHAR_N 44 // Сместим код ошибки в таблице аски, так чтоб она была печатной
/* 7 -> |0|0|ERR_RX_OVERFLOW|FE0|DOR0|UPE0|0|ERR_SEND_COUNTER| <- 0
 Младший разряд отведен под счётчик отправки байтов ошибки. Перед отправкой ошибки или сравнением необходимо его маскировать.
 Всего отправлется три байта 'E' 'Код ошибки в виде символа' '\n' */
// volatile uint8_t uart_errors = ERR_CLEAN; // 3 байта на отправку, один отправляется сразу.
// Занчит нужно отследить ещё два

    // 0 приём, 1 предача 
#define UART_485_TRANSMIT_PORT_PIN PORTD2
#define UART_485_TRANSMIT_PORT_CONF DDRD
#define UART_485_TRANSMIT_PORT_CONF_N DDD2
#define UART_485_TRANSMIT_PORT PORTD

#define UART_485_TRANSMIT_PORT_SETUP (UART_485_TRANSMIT_PORT_CONF |= _BV(UART_485_TRANSMIT_PORT_CONF_N))
#define UART_485_TRANSMIT_PORT_DEFAULT (UART_485_TRANSMIT_PORT &= ~_BV(UART_485_TRANSMIT_PORT_PIN))

#define UART_485_NL '\r' // \r for proteus \n for normal system

int uart_init();
int uart_write(char *buf, uint8_t size);
int uart_read_size(void);
int uart_read_tobuf(char *buf);

#endif /* L_UART485_H */
