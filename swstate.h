#include <avr/io.h>

#ifndef L_SWSTATE_H
#define L_SWSTATE_H
//#define SWST_COUNT 11

//Что есть
// 1. hw свичи. Их нужно связывать с swst (+swtohwmap)
// 2. есть текстовые имена
// 3. есть интерфейс виртуальных свичей
// 4. есть выходы (+out_n,)
// 5. есть режимы выходов (+SW_TYPE)
// 6. режимы работы по времени
// 7. хранение состояний.
// 8. определится с режимами. нормальный, альтернативный, ночной, -аварийный
//
//

void swstate_apply(void);

#define SW_COUNT 3

#define SW_HW_BIT    0
#define SW_VIRT_BIT  1
#define SW_ALT_BIT   2
#define SW_NIGHT_BIT 3
#define SW_ALARM_BIT 4
#define SW_FLUSH_BIT 5


#define SW_TYPE_ONOFF   _BV(0)
#define SW_TYPE_DIMM    _BV(1)
#define SW_TYPE_ALT     _BV(2)
#define SW_TYPE_ALARM   _BV(3)
#define SW_TYPE_NIGHT   _BV(4)

struct SWState {
    uint8_t state;
    uint8_t type;
    uint8_t out_n;
};

volatile struct SWState swstates[SW_COUNT];

#define SW_MAP_TYPE_MAIN 0
#define SW_MAP_TYPE_ALT  1

struct SWStateTOHWmap {
    uint8_t sw_n;
    uint8_t hw_n;
    uint8_t map_type;
}; 

#define SW_MAP_COUNT 3 //Нужно обновлять если меняется количество
struct SWStateTOHWmap swtohwmap[SW_MAP_COUNT];


#endif /* L_SWSTATE_H */
