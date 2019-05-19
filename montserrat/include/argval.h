#ifndef PRACTICA_SOA_ARGVAL_H
#define PRACTICA_SOA_ARGVAL_H

#include <string.h>

#include "view.h"

/**
 * CONSTANTS
 **/

//Operations
#define GET_INFO "-info"
#define GET_FILE_METADATA "-search"
#define GET_FILE_INFO "-show"

//Validation codes
#define NO_OP_CODE 0
#define GET_INFO_CODE 1
#define GET_FILE_METADATA_CODE 2
#define GET_FILE_INFO_CODE 3

/**
 * FUNCTIONS
 **/

int ARGVAL_validate(int argc, char * argv[]);

#endif //PRACTICA_SOA_ARGVAL_H
