#ifndef PRACTICA_SOA_VIEW_H
#define PRACTICA_SOA_VIEW_H

#include <stdio.h>

#include "types.h"
#include "identifier.h"


//Argument messages
#define UNDEFINED_OPERATION "Operation [%s] could not be recognized. It must be one of the following: -info | -search | -show\n"
#define WRONG_FORMAT "Wrong invocation format!\n"
#define FORMAT_REMINDER "Remember that the invocation format of the executable must be the following:\n./executable_name <operation> <volume_name> [<filename>]\n"
#define MISSING_FILESYSTEM "Error. Filesystem [%s] not found.\n"
#define UNDEFINED_FILESYSTEM "Filesystem not recognized (%s)\n"

//Filesystem metadata messages
#define FILESYSTEM_INFO "---- Filesystem Information ----\n\nFilesystem: %s\n\n"
#define INODE_INFO "INODE INFO\nInode Size: %d\nNumber of Inodes: %d\nFirst Inode: %d\nInodes Group: %d\nFree Inodes: %d\n\n"
#define BLOCK_INFO "BLOCK INFO\nBlock Size: %d\nReserved Blocks: %ld\nFree Blocks: %ld\nTotal Blocks: %ld\nFirst Block: %d\nBlock Group: %d\nFrags Group: %d\n\n"
#define VOLUME_INFO "VOLUME INFO\nVolume name: %s\n"
#define FAT_INFO "System Name: %s\nSector Size: %u\nSectors per Cluster: %u\nReserved Sectors: %u\nNumber of FATs: %u\nMaximum Root Entries: %u\nSectors per FAT: %u\nLabel: %s\n\n"

//Filesystem file search
#define FILE_NOT_FOUND "Error. File not found.\n"
#define FILE_METADATA "File found! Size: %d bytes. Created on: %hu/%hu/%hu\n"

void VIEW_undefinedOperation(char * operation);

void VIEW_wrongFormat();

void VIEW_missingVolume(char * volume);

void VIEW_undefinedFileSystem(char * fileSystem);

void VIEW_showExt4MetaData(struct Ext4MetaData metadata);

void VIEW_showFat32MetaData(struct fat_BS fat_boot, struct fat_extBS_32 fat_boot_ext_32);

void VIEW_showFat32FileMetadata(struct dir_entry *entry, int status);
#endif //PRACTICA_SOA_VIEW_H
