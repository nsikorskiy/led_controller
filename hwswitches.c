#include "hwswitches.h"
#include <avr/interrupt.h>

/*
 *  0  1  2  3
 *  4  5  6  7
 *  8  9  10 11
 *  12 13 14 15
 *
 */

void hwswitch_init(void) {
    HW_SW_ROW_INIT_PORT;
    HW_SW_ROW_INIT_ZSTATE;
    HW_SW_COLUMN_INIT_PORT;
}



void scan_keys(void) {
    uint8_t pin = 0;
    int hwswitch_num = 0;
    for(int col=0;col<HW_SW_COLUMN_COUNT;col++) {
        //активируем столбец
        HW_SW_COLUMN_PORT |= _BV(col);
        //очищаем pin
        pin = 0;
        for(int row=0;row<HW_SW_ROW_COUNT;row++){
            //читаем состояние строки
            pin = HW_SW_ROW_PORT & _BV(row);
            if (pin) {
                hwswitches[hwswitch_num].shift_bits <<= 1;
                hwswitches[hwswitch_num].shift_bits |= _BV(0);
            } else {
                hwswitches[hwswitch_num].shift_bits <<= 1;
            }
            //sate chek
            if ((hwswitches[hwswitch_num].shift_bits == 0) && (hwswitches[hwswitch_num].state == 1)) {
                hwswitches[hwswitch_num].state = _BV(7);
                hwswitches[hwswitch_num].state_time = 0xFF;
                hwswitch_flag |= HW_SW_INTR;
            }

            if ((hwswitches[hwswitch_num].shift_bits == 0xFF) && (hwswitches[hwswitch_num].state == 0)) {
                hwswitches[hwswitch_num].state = _BV(7) | _BV(0);
                hwswitches[hwswitch_num].state_time = 0xFF;
                hwswitch_flag |= HW_SW_INTR;
            }


            hwswitch_num++;
        }
        //деактивируем столбец
        HW_SW_COLUMN_PORT &= ~_BV(col);
    }

}
