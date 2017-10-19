#include "api.h"
#include <stdint.h>
#include <stdlib.h>
#include <string.h>
#include <avr/pgmspace.h>
#include "ds3231.h"
#include "uart485.h"

char api_buffer[64];
int8_t input_size = 0;
int8_t ret = 0;

int8_t cmd_ERROR(void) {
    strcpy_P(api_buffer, PSTR("CMD ERR\r"));
    return 8;
}



int8_t api_get_date(void) {
    strcpy_P(api_buffer, PSTR("martobr 18\r"));
    return 11;
}

int8_t api_set_date(uint8_t offset) {
    strcpy_P(api_buffer, PSTR("OK\r"));
    return 3;
}


int8_t api_echo(uint8_t offset) {
    memcpy(api_buffer, api_buffer + offset, input_size - offset);
    api_buffer[input_size - offset] = '\r';
    return input_size - offset + 1; // + \r
}

int8_t cmd_parser(void) {

    #define ECHO_S 5
    if ((input_size > ECHO_S) && (strncmp_P(api_buffer, PSTR("ECHO "), ECHO_S) == 0)) {
        return api_echo(ECHO_S);
    } else
    #define SET_DATE_S 9
    if ((input_size > SET_DATE_S) && (strncmp_P(api_buffer, PSTR("SET DATE "), SET_DATE_S) == 0)) {
        return api_set_date(SET_DATE_S);
    } else 
    #define GET_DATE_S 8
    if ((input_size >= GET_DATE_S) && (strncmp_P(api_buffer, PSTR("GET DATE "), GET_DATE_S) == 0)) {
        return api_get_date();
    }

    return cmd_ERROR();
}

void api(void) {
    input_size = uart_read_size();
    if (input_size) {
        uart_read_tobuf(api_buffer);
        uart_write(api_buffer, cmd_parser());
    }
}
