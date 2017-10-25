#include <avr/io.h>

/*
 *  0  1  2  3
 *  4  5  6  7
 *  8  9  10 11
 *  12 13 14 15
 *
 */


#define HW_SW_COUNT 16
#define HW_SW_ROW_COUNT 4
#define HW_SW_COLUMN_COUNT 4

// ROW входы
#define HW_SW_ROW_PORT PINC
// COLUMN выходы
#define HW_SW_COLUMN_PORT PORTB

#define HW_SW_ROW_INIT_PORT (DDRC &= ~(_BV(DDC0)|_BV(DDC1)|_BV(DDC2)|_BV(DDC3))) //Настраиваем как вход
#define HW_SW_ROW_INIT_ZSTATE (PORTC &= ~(_BV(PORTC0)|_BV(PORTC1)|_BV(PORTC2)|_BV(PORTC3))) //Без подтяжки
#define HW_SW_COLUMN_INIT_PORT DDRB |= _BV(DDB0)|_BV(DDB1)|_BV(DDB2)|_BV(DDB3) //Настраиваем как выход

void hwswitch_init(void);

#define HW_SW_FEEL _BV(7) // Флаг показывает, что после сключений прошёл полный
// цикл наполнения регистров сдвига. Значит состояние актуальное.
#define HW_SW_INTR _BV(0) // Есть изменение состояния хардварных свичей
volatile uint8_t hwswitch_flag;

// state бит 7 - флаг изменённости. бит 1 - flap, бит 0 - on/off
// offtime декриментится после изменения, если при изменении 
// статуса он не равен 0 то выставить flap

struct HWSwitch {
    uint8_t state;
    uint8_t shift_bits;
    uint8_t off_time;
};

volatile struct HWSwitch hwswitches[HW_SW_COUNT];
