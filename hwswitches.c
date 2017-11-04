#include "hwswitches.h"
#include <avr/interrupt.h>
#include <util/atomic.h>
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
    TIMSK0 |= _BV(TOIE0);
}

void led_debug(void) {
    DDRB |= _BV(DDB5);
    if (PORTB & _BV(DDB5)) {
        PORTB &= ~_BV(DDB5);
    } else {
        PORTB |= _BV(DDB5);
    }
}

volatile uint8_t hwswitch_timer_dec = 0;
volatile uint8_t hwswitch_timer_read = 0;
volatile uint8_t hwswitch_first_run = 0;

inline void hwswitch_timer(void) {

    // Хочу детект 2 секунды
    // частота T0 сейчас  16000000/256 = 62500
    // геристр для отсчёта 256
    // 1/(2/256)
    // 128.00000000000000000000
    // ((16000000/8)/256)/128
    // 61.03515625000000000000
    // нужно декрементить каждые 61 тиков
    if (hwswitch_timer_dec <= 61) {
        hwswitch_timer_dec++;
    } else {
        hwswitch_timer_dec = 0;
        hwswitch_flag |= HW_SW_DEC;
    }
    
    // Средняя длительность помехи 5-10ms (наверное)
    // для уверенного детекта нужно 8 циклов
    // нужно чтоб задержка непривысила 20ms
    // 20/8
    // 2.50000000000000000000
    // (20/8)/1000
    // .00250000000000000000
    // 1/((20/8)/1000)
    // 400.00000000000000000000
    // ((16000000/8)/256)/400
    // 19.53125000000000000000
    if (hwswitch_timer_read <= 20) {
        hwswitch_timer_read++;
    } else {
        hwswitch_timer_read = 0;
        hwswitch_flag |= HW_SW_READ;
        if (hwswitch_first_run < 8){
            hwswitch_first_run++;
        } 
        if (hwswitch_first_run == 8) {
            hwswitch_first_run++;
            hwswitch_flag |= HW_SW_FEEL;
        }
    }
    
}

ISR(TIMER0_OVF_vect){
    hwswitch_timer();
}


void hwswitch_dec_time_switches(void) {
    if (hwswitch_flag & HW_SW_DEC) {
        for (int i=0; i<HW_SW_COUNT; i++){
            if (hwswitches[i].state_time) {
                hwswitches[i].state_time--;
            }
        }
        ATOMIC_BLOCK(ATOMIC_RESTORESTATE) {
            hwswitch_flag &=~HW_SW_DEC;
        }
    }
}

void hwswitch_scan_keys(void) {
    if (hwswitch_flag & HW_SW_READ) {
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
                if ((hwswitches[hwswitch_num].shift_bits == 0) && (hwswitches[hwswitch_num].state & HW_SW_N_ON)) {
                    if (hwswitches[hwswitch_num].state_time) {
                        hwswitches[hwswitch_num].state = HW_SW_N_INTR | HW_SW_N_FLAP;
                    } else {
                        hwswitches[hwswitch_num].state = HW_SW_N_INTR;
                    }
                    hwswitches[hwswitch_num].state_time = 0xFF;
                    hwswitch_flag |= HW_SW_INTR;
                }

                if ((hwswitches[hwswitch_num].shift_bits == 0xFF) && ((hwswitches[hwswitch_num].state & HW_SW_N_ON) == 0)) {
                    if (hwswitches[hwswitch_num].state_time) {
                        hwswitches[hwswitch_num].state = HW_SW_N_INTR | HW_SW_N_FLAP | HW_SW_N_ON;
                    } else {
                        hwswitches[hwswitch_num].state = HW_SW_N_INTR |  HW_SW_N_ON;
                    }
                    hwswitches[hwswitch_num].state_time = 0xFF;
                    hwswitch_flag |= HW_SW_INTR;
                }


                hwswitch_num++;
            }
            //деактивируем столбец
            HW_SW_COLUMN_PORT &= ~_BV(col);
        }
        ATOMIC_BLOCK(ATOMIC_RESTORESTATE) {
            hwswitch_flag &=~HW_SW_READ;
        }
    }

}
