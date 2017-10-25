#include <stdint.h>
#include <avr/pgmspace.h>

/*
enum switches {
    SW0,
    SW1,
    SW2,
    SW3,
    SW4,
    SW5,
    SW6,
    SW7,
    SW8,
    SW9,
    SW10
};
#define SWITCH_COUNT 11
enum pwms {
    PWM0,
    PWM1,
    PWM2,
    PWM3,
    PWM4,
    PWM5,
    PWM6,
    PWM7,
    PWM8,
    PWM9,
    PWM10,
    PWM11,
    PWM12,
    PWM13,
    PWM14,
    PWM15
};
#define PWM_COUNT 16


enum pwm_types {PWM, ONOFF, UNUSED_OUT};

const uint8_t PwmConf[PWM_COUNT] PROGMEM = {
    PWM,
    PWM,
    PWM,
    PWM,
    PWM,
    ONOFF,
    PWM,
    PWM,
    PWM,
    ONOFF,
    ONOFF,
    UNUSED_OUT,
    UNUSED_OUT,
    UNUSED_OUT,
    UNUSED_OUT,
};

struct SwitchState {
    uint8_t sw_state;
    uint16_t pwm_state;
};


#define HW_SWITCH_COUNT 16

struct HWSwitch {
    uint8_t state_hist;
    uint16_t off_time;
};

enum hwswitch_types { MAIN_SW, OPT_SW, UNUSE_SW };

struct HWSwitchMap {
    uint8_t switch_n;
    uint8_t hwswitch_type;
};

const struct HWSwitchMap HWSwitchConf[HW_SWITCH_COUNT] PROGMEM = {
    {SW0, MAIN_SW}, //1
    {SW0, OPT_SW},  //2
    {SW1, MAIN_SW}, //3
    {SW1, OPT_SW},  //4
    {SW2, MAIN_SW}, //5
    {SW3, MAIN_SW}, //6
    {SW4, MAIN_SW}, //7
    {SW5, MAIN_SW}, //8
    {SW6, MAIN_SW}, //9
    {SW7, MAIN_SW}, //10
    {SW8, MAIN_SW}, //11
    {SW9, MAIN_SW}, //12
    {SW10, MAIN_SW}, //13
    {SW1, UNUSE_SW}, //14
    {SW1, UNUSE_SW}, //15
};


struct Switch {
    uint8_t sw_state_n;
    uint8_t pwm_out_n;
};
const struct Switch SwitchesConf[SWITCH_COUNT] PROGMEM = {
    { SW0, PWM0},
    { SW1, PWM2},
    { SW2, PWM1},
    { SW3, PWM3},
    { SW4, PWM4},
    { SW5, PWM5},
    { SW6, PWM6},
    { SW7, PWM7},
    { SW8, PWM8},
    { SW9, PWM9},
    { SW10, PWM10}
};



#define PWM_MODE_COUNT 3
volatile uint16_t Pwm_modes[SWITCH_COUNT][PWM_MODE_COUNT];
volatile struct SwitchState Switches_state[SWITCH_COUNT];
volatile struct HWSwitch HWSwitches_state[HW_SWITCH_COUNT];

*/
