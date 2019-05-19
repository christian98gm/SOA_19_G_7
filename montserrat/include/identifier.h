#ifndef PRACTICA_SOA_IDENTIFIER_H
#define PRACTICA_SOA_IDENTIFIER_H

#include <unistd.h>
#include <stdio.h>
#include <stdint.h>

#include "ext4types.h"
#include "fat32types.h"
#include "types.h"

/**
 * CONSTANTS
 **/

//Filesystem codes
#define FAT12_FS 0
#define FAT16_FS 1
#define FAT32_FS 2
#define EXT2_FS 10
#define EXT3_FS 11
#define EXT4_FS 12

//Filesystem names
#define FAT12_FS_NAME "FAT12"
#define FAT16_FS_NAME "FAT16"
#define FAT32_FS_NAME "FAT32"
#define EXT2_FS_NAME "EXT2"
#define EXT3_FS_NAME "EXT3"
#define EXT4_FS_NAME "EXT4"
#define UNDEFINED_FS_NAME "UNDEFINED"

/**
 * FUNCTIONS
 **/

int FSID_getFileSystemType(int fd);

char * FSID_getFileSystemName(int type);

#endif //PRACTICA_SOA_IDENTIFIER_H
