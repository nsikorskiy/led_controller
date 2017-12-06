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
};

void apply_hw(void) {
    if (hwswitch_flag & (HW_SW_INTR | HW_SW_FEEL))  {
        hwswitch_flag &= ~HW_SW_INTR;
        for (uint8_t i=0; i<SW_MAP_COUNT; i++){
            uint8_t hw_n = swtohwmap[i].hw_n;
            if (hwswitches[hw_n].state & HW_SW_N_INTR){
                hwswitches[hw_n].state &= ~HW_SW_N_INTR;
                if (swtohwmap[i].map_type == SW_MAP_TYPE_MAIN){
                    //FLUHS  ?
                }
                if (swtohwmap[i].map_type == SW_MAP_TYPE_ALT){
                }
            }
        }
    }
}

void swstate_apply(void) {
    apply_hw();

}
