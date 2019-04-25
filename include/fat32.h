#ifndef PRACTICA_SOA_FAT32_H
#define PRACTICA_SOA_FAT32_H

#include <unistd.h>

#include "types.h"
#include "view.h"

#define MAX_FAT12 4084
#define MAX_FAT16 65524

void FAT32_showFileSystemInfo(int fd);

#endif //PRACTICA_SOA_FAT32_H
