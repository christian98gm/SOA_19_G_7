#ifndef PRACTICA_SOA_EXT4_H
#define PRACTICA_SOA_EXT4_H

#include <unistd.h>
#include <stdio.h>
#include <math.h>
#include <string.h>
#include <stdint.h>
#include <stdlib.h>

#include "types.h"
#include "ext4types.h"
#include "view.h"

/**
 * FUNCTIONS
 **/

void EXT4_showFileSystemInfo(int fd);

void EXT4_showFileMetadata(int fd, char *filename);

void EXT4_showFileInfo(int fd, char * filename);

#endif //PRACTICA_SOA_EXT4_H
