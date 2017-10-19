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
    TCCR0B = _BV(CS02); // CLK/256 таймер для uart
    // регистр сравнениея B используется для прерываний
}

void Setup() {
    uart_init();
    twi_init();
    sei();
}

void Restore() {
}



int main() {
    Init();
    Setup();
    Restore();
    while (1) {
        api();
    }
    return 0;
}
