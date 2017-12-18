#include "swstate.h"
#include <string.h>
#include <avr/pgmspace.h>

const char swname_0[] PROGMEM = "Zerkalo";
const char swname_1[] PROGMEM = "Koridor";
const char swname_2[] PROGMEM = "Spalnia";

PGM_P const swname_table[SW_COUNT] PROGMEM =
{
    swname_0,
    swname_1,
    swname_2,
};

void swstate_api_list(char *buf) {
    for(int i=0; i<SW_COUNT; i++) {
        strcat_P(buf, (PGM_P)pgm_read_word(&(swname_table[i])));
        strcat(buf, " ");
        uint8_t type = swstates[i].type + 48;
        char type_str[] = "X";
        type_str[0] = (char) type;
        strcat(buf, type_str);
        strcat(buf, "\n");
    }
}

void swstate_api_get_status(char *buf, uint8_t sw_n) {
    uint8_t state = swstates[sw_n].state + 48;
    char state_str[] = "X";
    state_str[0] = (char) state;
    strcat(buf, state_str);
}

void swstate_api_set_status(uint8_t sw_n, uint8_t bit_n, uint8_t state) {
    swstates[sw_n].state |= _BV(SW_INTR);
    if (state) {
        swstates[sw_n].state |= _BV(bit_n);
    } else {
        swstates[sw_n].state &= ~_BV(bit_n);
    }
    swstate_save_state(sw_n, swstates[sw_n].state);
}
