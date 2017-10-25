#include <avr/io.h>
#include <string.h>
#include <avr/interrupt.h>
#include <util/atomic.h>
#include <inttypes.h>
#include <avr/sleep.h>
#include "uart485.h"


char uart_tx_buffer[UART_TX_BUFFER_SIZE];
char uart_rx_buffer[UART_RX_BUFFER_SIZE];
int8_t uart_tx_buffer_index = 0;
int8_t uart_rx_buffer_index = 0;



volatile uint8_t uart_flags = 0;
volatile uint8_t uart_errors = ERR_CLEAN;

void start_timer_rx(void);

int uart_init() {

    UCSR0B = _BV(TXEN0) | _BV(RXEN0) | _BV(RXCIE0); //Включаем пины для приёма и предачи. Разрешаем прерывания RX
    UCSR0C = _BV(UCSZ00) | _BV(UCSZ01) | _BV(UPM01)| _BV(UPM00); //8bit odd parity 

    UBRR0 = UART_UBRR0;
    start_timer_rx();
    // Порт для управления направлением rs485
    UART_485_TRANSMIT_PORT_SETUP;
    UART_485_TRANSMIT_PORT_DEFAULT;  //разрешаем приём 
    // 0 приём, 1 предача 

    return 0;
}

int uart_write(char *buf, uint8_t size){
    if (size == 0 ) return 1;

    while ((uart_flags & _BV(TX_IN_PROGRESS)) || (uart_flags & _BV(TX_BUFFER_READY))) {
        sleep_mode();
    }
    uart_tx_buffer_index = size - 1; // хранит номер элемента а не размер
    for (; size>=1; size--) {
        uart_tx_buffer[size - 1] = buf[uart_tx_buffer_index - size + 1];
    }
    ATOMIC_BLOCK(ATOMIC_FORCEON){
        uart_flags |= _BV(TX_BUFFER_READY);
    }
    return 0;
}

int uart_read_size(void) {
    if (uart_flags & _BV(RX_BUFFER_READY)) {
        return uart_rx_buffer_index;
    }
    return 0;
}

int uart_read_tobuf(char *buf){
    memcpy(buf, uart_rx_buffer, uart_rx_buffer_index+1);
    ATOMIC_BLOCK(ATOMIC_FORCEON){
        uart_flags &= ~_BV(RX_BUFFER_READY);
    }
    return 0;
}

void start_timer_tx(void) {
    // TX TIME!
    uart_flags |= _BV(TX_TIME);
    OCR0B = (uint8_t)TCNT0 + 3;
    TIMSK0 |= _BV(OCIE0B); //Enable iterrupt
}

void start_timer_rx(void) {
    //RX time
    uart_flags &= ~_BV(TX_TIME);
    OCR0B = (uint8_t)TCNT0 - 3;
    TIMSK0 |= _BV(OCIE0B); //Enable iterrupt
}
void stop_timer(void) {
    TIMSK0 &= ~_BV(OCIE0B); // Disable iterrupt
}

ISR(TIMER0_COMPB_vect) {
    if (uart_flags & _BV(TX_TIME)) {
        if (uart_flags & _BV(RX_ERROR)){ // Ответ об ошибке
            stop_timer();
            UART_485_TRANSMIT_PORT |= _BV(UART_485_TRANSMIT_PORT_PIN); // set tx in rs485
            UDR0 = 'E'; //первый символ ошибки
            UCSR0B |= _BV(UDRIE0);
        } else {
            if (uart_flags & _BV(TX_PING) ) { //ping
                stop_timer();
                UART_485_TRANSMIT_PORT |= _BV(UART_485_TRANSMIT_PORT_PIN); // set tx in rs485
                uart_flags &= ~_BV(TX_TIME);
                uart_flags &= ~_BV(TX_PING);
                UDR0 = UART_485_NL;
                UCSR0B |= _BV(TXCIE0); // отправляем один байт, значит нам не нужно UDRIE0
            } else {
                if (uart_flags & _BV(TX_BUFFER_READY)){ //cарт предачи буфера
                    stop_timer();
                    UART_485_TRANSMIT_PORT |= _BV(UART_485_TRANSMIT_PORT_PIN); // set tx in rs485
                    uart_flags |= _BV(TX_IN_PROGRESS);
                    UDR0 = uart_tx_buffer[uart_tx_buffer_index--];
                    UCSR0B |= _BV(UDRIE0);
                }
            }
        }
    }
    uart_flags ^= _BV(TX_TIME);
}

ISR(USART_RX_vect) {
    // load data
    char c = UDR0;
    // check uart errors
    if (UCSR0A & (_BV(FE0)|_BV(DOR0)|_BV(UPE0))) {
        //drop
        uart_flags |= _BV(RX_ERROR);
        //log error
        uart_errors |= UCSR0A & (_BV(FE0)|_BV(DOR0)|_BV(UPE0));
    }
    if (uart_flags & _BV(RX_IN_PROGRESS)) {
        if ( (uart_flags & _BV(RX_ERROR)) && (c == UART_485_NL) ) {
            //Конец приёма при ошибке
            //stop RX
            uart_flags &= ~_BV(RX_IN_PROGRESS);
            // Ошибка обработается при срабатывании таймера
            start_timer_tx();
        } else {
            if (c == UART_485_NL) { //Конец приёма
                //stop RX
                uart_flags |= _BV(RX_BUFFER_READY);
                uart_flags &= ~_BV(RX_IN_PROGRESS);
                start_timer_tx();
            } else {
                if (uart_rx_buffer_index == UART_RX_BUFFER_SIZE ){
                    uart_flags |= _BV(RX_ERROR);
                    uart_errors |= _BV(ERR_RX_OVERFLOW); //Log error
                } else {
                    uart_rx_buffer[uart_rx_buffer_index++] = c;
                }
            }
        }
    } else {
        if (c == UART_485_NL) { // FIXME Может проверить статус ошибки? вдруг это не \n?
            uart_flags |= _BV(TX_PING);
            stop_timer();
            start_timer_tx();
        } else { //начало приёма
            //FIXME Нужно проверить, не начался ли приём, в уже наполненный буфер
            //start rx
            uart_rx_buffer_index = 0;
            stop_timer();
            uart_rx_buffer[uart_rx_buffer_index++] = c;
            uart_flags |= _BV(RX_IN_PROGRESS);
        }
    }
}
    


ISR(USART_UDRE_vect) {
    if (uart_flags & _BV(RX_ERROR)) {
        switch(uart_errors & ERR_COUNTER_MASK){
            //case(2):
            //    UDR0 = 'E';   Это это было послано таймером для инициализации передачи
            //    break;
            case(1):
                uart_errors = uart_errors - 1; // декрементим бит
                UDR0 = (char) (uart_errors + CHAR_N);
                break;
            case(0):
                UDR0 = UART_485_NL;
                UCSR0B &= ~_BV(UDRIE0);
                UCSR0B |= _BV(TXCIE0);
                break;
        }
    } else { 
        if(uart_tx_buffer_index != 0 ) {
            UDR0 = uart_tx_buffer[uart_tx_buffer_index--];
        } else {
            UDR0 = uart_tx_buffer[uart_tx_buffer_index];
            //disable udre_vect
            UCSR0B &= ~_BV(UDRIE0);
            //enable tx_vect
            UCSR0B |= _BV(TXCIE0);
        }
    }
}

ISR(USART_TX_vect) {
    if (uart_flags & _BV(RX_ERROR)) {
        uart_flags &= ~_BV(RX_ERROR);
        uart_errors = ERR_CLEAN;
    } else {
        uart_flags &= ~(_BV(TX_BUFFER_READY)|_BV(TX_IN_PROGRESS));
    }
    //disable tx_vect
    UCSR0B &= ~_BV(TXCIE0);
    UART_485_TRANSMIT_PORT &= ~_BV(UART_485_TRANSMIT_PORT_PIN); // set rx
    //set timer to RX
    start_timer_rx();
}



