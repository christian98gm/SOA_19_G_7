#ifndef PRACTICA_SOA_DATE_H
#define PRACTICA_SOA_DATE_H

#include <stdint.h>
#include <time.h>
#include <string.h>
#include <stdio.h>

/**
 * CONSTANTS
 **/

#define MAX_DATE_STRING 64

/**
 * FUNCTIONS
 **/

void DATE_getShortDate(uint32_t secs, char * date);

void DATE_getLongDate(uint32_t secs, char * date);

#endif //PRACTICA_SOA_DATE_H
