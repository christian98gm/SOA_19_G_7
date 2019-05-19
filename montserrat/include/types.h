#ifndef PRACTICA_SOA_TYPES_H
#define PRACTICA_SOA_TYPES_H

#include <stdint.h>
#include "ext4types.h"
#include "date.h"

/**
 * FILE METADATA STRUCTS
 **/

//File metadata
struct FileMetaData {
    uint64_t size;
    char createdAt[MAX_DATE_STRING];
};

/**
 * EXT4 METADATA STRUCTS
 **/

//Ext4 superblock inode metadata
struct InodeInfo {
    uint16_t inodeSize;
    uint32_t totalInodes;
    uint32_t firstInode;
    uint32_t inodesGroup;
    uint32_t freeInodes;
};

//Ext4 superblock block metadata
struct BlockInfo {
    uint32_t blockSize;
    uint64_t reservedBlocks;
    uint64_t freeBlocks;
    uint64_t totalBlocks;
    uint32_t firstBlock;
    uint32_t blockGroup;
    uint32_t fragsGroup;
};

//Ext4 superblock volume metadata
struct VolumeInfo {
    char volumeName[MAX_VOL_NAME];
    char lastCheck[MAX_DATE_STRING];
    char lastMount[MAX_DATE_STRING];
    char lastWritten[MAX_DATE_STRING];
};

//Ext4 superblock metadata
struct Ext4MetaData {
    struct InodeInfo inode;
    struct BlockInfo block;
    struct VolumeInfo volume;
};

#endif //PRACTICA_SOA_TYPES_H
