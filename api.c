#include "api.h"
#include <stdint.h>
#include <stdlib.h>
#include <string.h>
#include <avr/pgmspace.h>
#include "ds3231.h"

int8_t ret;

int8_t cmp_data( char *in) {
    return strncmp_P(in+4, PSTR("DATE "), 5);
}

int8_t get_date( char *in, int8_t in_size,  char *out) {
    if (cmp_data(in) == 0) {
    }
    return 0;
}

int8_t set_date( char *in, int8_t in_size,  char *out) {
    if (cmp_data(in) == 0) {
    }
    return 0;
}

int8_t api_set( char *in, int8_t in_size,  char *out) {
    ret = set_date(in, in_size, out);
    if (ret) return ret;
    return 0;
}

int8_t api_get( char *in, int8_t in_size,  char *out) {
    ret = get_date(in, in_size, out);
    if (ret) return ret;
    return 0;
}

int8_t api_echo( char *in, int8_t in_size,  char *out) {
    memcpy(out, in + 5, in_size - 5);
    out[in_size - 5] = '\r';
    return in_size - 5 + 1;
}



int8_t api( char *in, int8_t size,  char *out) {

    if (size > 3) {
        if ( strncmp_P(in, PSTR("SET "), 4) == 0 ){
            return api_set(in, size, out);
        }
        if ( strncmp_P(in, PSTR("GET "), 4) == 0){
            return api_get(in, size, out);
        }
        if ( strncmp_P(in,  PSTR("ECHO "), 5) == 0) {
            return api_echo(in, size, out);
        }

    }

    strcpy_P(out, PSTR("CMD ERR\r"));
    return 8;
}
