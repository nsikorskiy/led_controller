#include "api.h"
#include <stdint.h>
#include <stdlib.h>
#include <string.h>
#include  <avr/pgmspace.h>

int8_t api_set(unsigned char *in, int8_t in_size, unsigned char *out) {
    strcpy_P(out, PSTR("SET!\r"));
    return 5;
}

int8_t api_get(unsigned char *in, int8_t in_size, unsigned char *out) {
    strcpy_P(out, PSTR("GET!\r"));
    return 5;
}

int8_t api_echo(unsigned char *in, int8_t in_size, unsigned char *out) {
    memcpy(out, in + 5, in_size - 5);
    out[in_size - 5] = '\r';
    return in_size - 5 + 1;
}



int8_t api(unsigned char *in, int8_t size, unsigned char *out) {

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
