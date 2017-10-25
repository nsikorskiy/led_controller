#include <avr/pgmspace.h>
#include <stdlib.h>
#include "twi.h"
#include "ds3231.h"

uint8_t bcd2dec(uint8_t bcd)
{
    return ((bcd / 16) * 10) + (bcd % 16);
}

uint8_t dec2bcd(uint8_t dec)
{
    return ((dec / 10) * 16) + (dec % 10);
}

void bcd2char(uint8_t bcd, char *buf) {
    // берём старшие 4 бита и смещаем их вправо. 
    // добавляемс 48, дабы число не сравнялось с таковым в ASCII
    buf[0] = (char)(((bcd & 0xF0) >> 4) + 48);
    buf[1] = (char)((bcd & 0x0F ) + 48);
}

uint8_t char2bcd(char *buf) {
    uint8_t s, m;
    s = (((uint8_t)buf[0] - 48) << 4);
    m = (((uint8_t)buf[1] - 48) & 0x0F);
    return s | m;
}

uint8_t api_rtcdate_get(char *out) {
    #define RTC_TIME_READ_SIZE 7
    uint8_t data[RTC_TIME_READ_SIZE];
    uint8_t ret = RTC_SECONDS; // чтоб не выделять пременную сохраним адрес тут и передадим адрес пременной

    ret = twi_writeTo(RTC_ADDR, &ret, 1, 1, 0);
    if (ret) {
        strcpy_P(out, PSTR("Read RTC1 ERRN\r"));
        out[13] = ret + 48;
        return 15;
    }
    twi_readFrom(RTC_ADDR, data, RTC_TIME_READ_SIZE, 1);

                       //   67:910:1213  1617-1920-2223  26
    strcpy_P(out, PSTR("Date: NN:NN:NN, MM-MM-MM, WW\r"));

    bcd2char(data[2] & ~_BV(DS1307_CH), out+6);
    bcd2char(data[1], out+9);
    bcd2char(data[0] & ~_BV(HR1224), out+12);
    bcd2char(data[4], out+16);
    bcd2char(data[5], out+19);
    bcd2char(data[6] & ~_BV(CENTURY), out+22);
    bcd2char(data[3], out+26);
    return 29;
}

uint8_t api_rtcdate_set(char *in, uint8_t offset){
    #define RTC_TIME_WRITE_SIZE 8
    uint8_t data[RTC_TIME_WRITE_SIZE];
    uint8_t ret;
    //0  3  6  9  12 15 18
    //NN:NN:NN MM-MM-MM WW
    data[0] = (uint8_t) RTC_SECONDS;
    data[1] = char2bcd(in+offset+6); //Sec
    data[2] = char2bcd(in+offset+3); //Min
    data[3] = char2bcd(in+offset+0); //Hor
    data[4] = char2bcd(in+offset+18);//wday
    data[5] = char2bcd(in+offset+9); //day
    data[6] = char2bcd(in+offset+12); //month
    data[7] = char2bcd(in+offset+15); //yaer
    ret = twi_writeTo(RTC_ADDR, data, RTC_TIME_WRITE_SIZE, 1, 1);
    if (ret) {
        strcpy_P(in, PSTR("Write RTC ERRN\r"));
        in[13] = ret + 48;
        return 15;

    } else {
        strcpy_P(in, PSTR("OK\r"));
        return 3;
    }
    return 0;
}
