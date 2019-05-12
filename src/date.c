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

void DATE_getLongDate(uint32_t secs, char * date) {

    //Get time
    time_t t = secs;
    struct tm * time = localtime(&t);

    //Get date
    strcpy(date, asctime(time));

}
