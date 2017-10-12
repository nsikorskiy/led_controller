#include <stdint.h>
#include <stdlib.h>
#include <avr/interrupt.h>
#include <avr/sleep.h>
#include <avr/io.h>
#include "led_controller.h"
#include "twi.h"
#include "uart485.h"
#include "api.h"

void Init() {
    set_sleep_mode(SLEEP_MODE_IDLE); //Сон для остановки при отправке в uart
}

void Setup() {
    uart_init();
    twi_init();
    sei();
}

void Restore() {
}


unsigned char buffer[64];
int8_t api_in_size=0;
int8_t api_out_size=0;

//unsigned char dbg[12] = {'A','P','I',' ','R','E','T',' ','N','N','N','\r'};
//unsigned char dbg_n[3];

int main() {
    Init();
    Setup();
    Restore();
    while (1) {
        api_in_size = uart_read_size();
        if (api_in_size != 0) {
            uart_read_tobuf(buffer);
            api_out_size = api(buffer, api_in_size, buffer);
            //dbg_n[0] = ' ';
            //dbg_n[1] = ' ';
            //dbg_n[2] = ' ';
            //utoa(sizeof("L"), dbg_n, 10);
            //dbg[8] = dbg_n[0];
            //dbg[9] = dbg_n[1];
            //dbg[10] = dbg_n[2];
            //uart_write(dbg, 12);
            if (api_out_size != 0){
                uart_write(buffer, api_out_size);
            }
        }

    }
    return 0;
}
