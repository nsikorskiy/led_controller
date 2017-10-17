#include "ds3231.h"

uint8_t bcd2dec(uint8_t bcd)
{
    return ((bcd / 16) * 10) + (bcd % 16);
}

uint8_t dec2bcd(uint8_t dec)
{
    return ((dec / 10) * 16) + (dec % 10);
}

uint8_t API_RTCDateGet(void) {
    return 0;
}


void API_RTCDateSet(
        int8_t  tm_sec,  /**< seconds after the minute - [ 0 to 59 ] */
        int8_t  tm_min,  /**< minutes after the hour - [ 0 to 59 ] */
        int8_t  tm_hour, /**< hours since midnight - [ 0 to 23 ] */
        int8_t  tm_mday, /**< day of the month - [ 1 to 31 ] */
        int8_t  tm_wday, /**< days since Sunday - [ 0 to 6 ] */
        int8_t  tm_mon,  /**< months since January - [ 0 to 11 ] */
        int16_t tm_year  /**< years since 1900 */
        ) {

    return 0;
}
