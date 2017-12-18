#include <avr/io.h>
#include "hwswitches.h"
#include "swstate.h"

volatile struct SWState swstates[SW_COUNT] = {
    {0, SW_TYPE_ONOFF,                          0},
    {0, SW_TYPE_ONOFF|SW_TYPE_DIMM,             1},
    {0, SW_TYPE_ONOFF|SW_TYPE_DIMM|SW_TYPE_ALT, 3}
};

struct SWStateTOHWmap swtohwmap[SW_MAP_COUNT] = {
    {0, 0, SW_MAP_TYPE_MAIN},
    {1, 1, SW_MAP_TYPE_MAIN},
    {1, 2, SW_MAP_TYPE_ALT},
    {2, 3, SW_MAP_TYPE_MAIN},
};

void swload_state(void) {
}

int swstate_save_state(uint8_t sw_n, uint8_t state) {
    return 0;
}

void sw_set_state_from_hw(uint8_t sw_n, uint8_t hw_state, uint8_t state_bit, uint8_t set_flap) {
    swstates[sw_n].state |= _BV(SW_INTR);
    if ((hw_state & HW_SW_N_ON) != HW_SW_N_ON) { //off
        swstates[sw_n].state &=~_BV(state_bit);
    }
    if ((hw_state & HW_SW_N_ON) == HW_SW_N_ON) { //on
        swstates[sw_n].state |= _BV(state_bit);
    }
    if (set_flap) {
        if (hw_state & HW_SW_N_FLAP) {
            swstates[sw_n].state |= _BV(SW_FLAP_BIT);
        } else {
            swstates[sw_n].state &= ~_BV(SW_FLAP_BIT);
        }
    }
    swstate_save_state(sw_n, swstates[sw_n].state);
}

void apply_hw(void) {
    if (hwswitch_flag & (HW_SW_INTR | HW_SW_FEEL))  {
        hwswitch_flag &= ~HW_SW_INTR;
        for (uint8_t i=0; i<SW_MAP_COUNT; i++){
            uint8_t hw_n = swtohwmap[i].hw_n;
            if (hwswitches[hw_n].state & HW_SW_N_INTR){
                hwswitches[hw_n].state &= ~HW_SW_N_INTR;
                if (swtohwmap[i].map_type == SW_MAP_TYPE_ALT){
                    sw_set_state_from_hw(i, hwswitches[hw_n].state, SW_ALT_BIT, SW_NOT_UPDATE_FLAP);
                }
                if (swtohwmap[i].map_type == SW_MAP_TYPE_MAIN){
                    sw_set_state_from_hw(i, hwswitches[hw_n].state, SW_HW_BIT, SW_UPDATE_FLAP);
                }
            }
        }
    }
}


void swstate_apply(void) {
    apply_hw();
}
