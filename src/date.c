#include "date.h"

/**
 * FUNCTIONS
 **/

void DATE_getShortDate(uint32_t secs, char * msg) {

    //Get time
    time_t t = secs;
    struct tm * time = localtime(&t);

    //Get numeric date
    sprintf(msg, "%.2d/%.2d/%.4d", time->tm_mday, (time->tm_mon + 1), (time->tm_year + 1900));

}

void DATE_getShortDateFromBytes(uint16_t bytes, char * date) {
    sprintf(date, "%.2hu/%.2hu/%.4hu", bytes & 31, (bytes >> 5) & 5, 1980 + (bytes >> 9));
}

void DATE_getLongDate(uint32_t secs, char * date) {

    //Get time
    time_t t = secs;
    struct tm * time = localtime(&t);

    //Get date
    strcpy(date, asctime(time));

}
