#ifndef PRACTICA_SOA_FAT32_H
#define PRACTICA_SOA_FAT32_H

#include <unistd.h>
#include <stdio.h>
#include <sys/param.h>
#include "types.h"
#include "view.h"
#include <stdlib.h>
#include <ctype.h>

#define MAX_FAT12 4084
#define MAX_FAT16 65525

#define FAT32_ENTRY_SIZE 32



#define FAT32_ENTRY_FREE 0xE5
#define FAT32_ENTRY_END 0x00

#define ATTR_LONG_NAME 0x0F

#define FAT32_SHORT_VOLUMENAME_MAX 11
#define FAT32_LONG_NAME_SEGMENT 26
#define ATTR_DIRECTORY 0x10
typedef struct {
    char *filename;
    unsigned char attributes;
    unsigned int first_cluster;
    unsigned int file_size;
    unsigned short isLongSpecialFilename;
} __attribute__((packed))dir_entry;



void FAT32_showFileSystemInfo(int fd);
int FAT32_showFileInfo(int fd_Aux, char* filename);

#endif //PRACTICA_SOA_FAT32_H
