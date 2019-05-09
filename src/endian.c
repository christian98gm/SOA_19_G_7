//
// Created by christian on 09/05/2019.
//

#include "endian.h"

unsigned int ENDIAN_swap16(unsigned int num) {
    return ((num >> 8) & 0xFF) | ((num << 8) & 0xFF00);
}

unsigned long ENDIAN_swap32(unsigned long num) {
    return (
            ((num >> 24) & 0xFF) |
            ((num >> 8) & 0xFF00) |
            ((num << 8) & 0xFF0000) |
            ((num << 24) & 0xFF000000)
    );
}
