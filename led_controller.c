#include <stdint.h>
#include <stdlib.h>
#include <avr/interrupt.h>
#include <avr/sleep.h>
#include <avr/io.h>
#include "led_controller.h"
#include "libs/twi.h"
#include "libs/uart485.h"
#include "api.h"
#include "hwswitches.h"
#include "swstate.h"

void Init() {
    set_sleep_mode(SLEEP_MODE_IDLE); //Сон для остановки при отправке в uart
    TCCR0B = _BV(CS01); // CLK/8 таймер для uart и HW свичей
    // регистр сравнениея B используется для прерываний
    // переполнение используется для отсчёта времени опроса кнопок
}

void Setup() {
    uart_init();
    twi_init();
    hwswitch_init();
    sei();
}

void Restore() {
}

void tt(char *t) {
    t[0] = ' ';
    t[1] = ' '; 
}

int main() {
    Init();
    Setup();
    Restore();
    char n[3];
    char c[] = "SW NN STATE: N\r";
    while (1) {
        api();
        hwswitch_scan_keys();
        hwswitch_dec_time_switches();
        swstate_apply();
        /* if (hwswitch_flag & (HW_SW_INTR | HW_SW_FEEL))  {
            hwswitch_flag &= ~HW_SW_INTR;
            for (uint8_t i=0; i<HW_SW_COUNT; i++){
                if (hwswitches[i].state & HW_SW_N_INTR){
                    hwswitches[i].state &= ~HW_SW_N_INTR;
                    tt(n);
                    itoa(i, n, 10);
                    c[3] = n[0];
                    c[4] = n[1];
                    tt(n);
                    itoa(hwswitches[i].state, n, 10);
                    c[13] = n[0];
                    uart_write(c, 15);
                }
            }
        }*/

        sleep_mode();
    }
    return 0;
}
