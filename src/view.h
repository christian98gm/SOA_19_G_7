#ifndef PRACTICA_SOA_VIEW_H
#define PRACTICA_SOA_VIEW_H

#include <unistd.h>
#include <stdio.h>
#include <stdint.h>

#include "types.h"
#include "ext4types.h"
#include "fat32types.h"

/**
 * CONSTANTS
 **/

//Argument messages
#define UNDEFINED_OPERATION "Operation [%s] could not be recognized. It must be one of the following: -info | -search | -show\n"
#define WRONG_FORMAT "Wrong invocation format!\n"
#define FORMAT_REMINDER "Remember that the invocation format of the executable must be the following:\n./executable_name <operation> <volume_name> [<filename>]\n"

//Filesystem identify messages
#define MISSING_FILESYSTEM "Error. Filesystem [%s] not found.\n"
#define UNDEFINED_FILESYSTEM "Filesystem not recognized (%s)\n"

//Filesystem metadata messages
#define FILESYSTEM_INFO "---- Filesystem Information ----\n\nFilesystem: %s\n\n"
#define INODE_INFO "INODE INFO\nInode Size: %d\nNumber of Inodes: %d\nFirst Inode: %d\nInodes Group: %d\nFree Inodes: %d\n\n"
#define BLOCK_INFO "BLOCK INFO\nBlock Size: %d\nReserved Blocks: %ld\nFree Blocks: %ld\nTotal Blocks: %ld\nFirst Block: %d\nBlock Group: %d\nFrags Group: %d\n\n"
#define VOLUME_INFO "VOLUME INFO\nVolume name: %s\n"
#define FAT_INFO "System Name: %s\nSector Size: %u\nSectors per Cluster: %u\nReserved Sectors: %u\nNumber of FATs: %u\nMaximum Root Entries: %u\nSectors per FAT: %u\nLabel: %s\n\n"
#define LAST_CHECK "Last check: %s"
#define LAST_MOUNT "Last mount: %s"
#define LAST_WRITTEN "Last written: %s"

//File search messages
#define FILE_NOT_FOUND "Error. File not found.\n"
#define FILE_FOUND_METADATA_2 "File found! Size: %ld bytes. Created on: %s\n"

//File data messages
#define FILE_FOUND_CONTENT "\nFile found! Showing content...\n"
#define START_FILE "\n---------------------START_FILE----------------------\n"
#define END_FILE "\n---------------------END_FILE----------------------\n"

/**
 * FUNCTIONS
 **/

void VIEW_undefinedOperation(char * operation);

void VIEW_wrongFormat();

void VIEW_missingVolume(char * volume);

void VIEW_undefinedFileSystem(char * fileSystem);

void VIEW_showExt4MetaData(struct Ext4MetaData metadata);

void VIEW_showFat32MetaData(struct fat_BS fat_boot, struct fat_extBS_32 fat_boot_ext_32);

void VIEW_fileFound(struct FileMetaData metaData);

void VIEW_fileNotFound();

void VIEW_showFileInfo(char * info, uint64_t size);

void VIEW_showStartFile();
void VIEW_showEndFile();
void VIEW_showFileFragment(char * info, uint64_t size);

#endif //PRACTICA_SOA_VIEW_H
