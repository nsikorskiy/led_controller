
/*
 *  0  1  2  3
 *  4  5  6  7
 *  8  9  10 11
 *  12 13 14 15
 *
 */


#define SW_COUNT 16
#define SW_ROW_COUNT 4
#define SW_COLUMN_COUNT 4

uint8_t HWSwitch_flag;

// state бит 7 - флаг изменённости. бит 1 - flush, бит 0 - on/off
// offtime декриментится после off, если при изменении 
// статуса на on он не равен 0 то выставить flush

struct HWSwitch {
    uint8_t state;
    uint8_t shift_bits;
    uint8_t off_time;
};

volatile struct HWSwitch HWSwitches[SW_COUNT];
