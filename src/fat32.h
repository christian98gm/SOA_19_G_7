#ifndef PRACTICA_SOA_FAT32_H
#define PRACTICA_SOA_FAT32_H

#include <unistd.h>
#include <stdio.h>
#include <sys/param.h>
#include <stdlib.h>
#include <ctype.h>
#include <stdint.h>

#include "types.h"
#include "view.h"
#include "fat32types.h"
#include "date.h"

/**
 * FUNCTIONS
 **/

void FAT32_showFileSystemInfo(int fd);

void FAT32_showFileMetadata(int fd_Aux, char *filename);

void FAT32_showFileInfo(int fd_Aux, char *filenameAux);

#endif //PRACTICA_SOA_FAT32_H
