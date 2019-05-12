#ifndef PRACTICA_SOA_EXT4_H
#define PRACTICA_SOA_EXT4_H

#include <unistd.h>
#include <stdio.h>
#include <math.h>
#include <string.h>
#include <time.h>
#include <stdint.h>
#include <stdlib.h>

#include "types.h"
#include "view.h"
#include "date.h"

//Ext4 utils
#define SUPER_BLOCK_OFFSET 1024
#define LONG_FEATURE_MASK 0x80
#define EXTENT_FEATURE_MASK 0x40
#define JOURNAL_FEATURE_MASK 0x4
#define MAGIC_NUMBER_OFFSET 0x38
#define EXT_MAG_SIGN 0xEF53
#define EXT_HEADER_OFFSET 0x28
#define EXT_NODE_OFFSET 0x0C
#define EXT_DIR_ENTRY 0x2
#define FILE_FLAG_MASK 0x1
#define DIR_FLAG_MASK 0x2

void EXT4_showFileSystemInfo(int fd);
void EXT4_showFileMetadata(int fd, char *filename);
void EXT4_showFileInfo(int fd, char *filename);

#endif //PRACTICA_SOA_EXT4_H
