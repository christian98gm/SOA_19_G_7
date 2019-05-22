#include "view.h"

/**
 * FUNCTIONS
 **/

void VIEW_undefinedOperation(char * operation) {
	printf(UNDEFINED_OPERATION, operation);
	printf(FORMAT_REMINDER);
}

void VIEW_wrongFormat() {
	printf(WRONG_FORMAT);
	printf(FORMAT_REMINDER);
}

void VIEW_missingVolume(char * volume) {
	printf(MISSING_FILESYSTEM, volume);
}

void VIEW_undefinedFileSystem(char * fileSystem) {
	printf(UNDEFINED_FILESYSTEM, fileSystem);
}

void VIEW_showExt4MetaData(struct Ext4MetaData metadata) {
	printf(FILESYSTEM_INFO, "EXT4");
	printf(INODE_INFO, (unsigned short int) metadata.inode.inodeSize, (unsigned int) metadata.inode.totalInodes, (unsigned int) metadata.inode.firstInode, (unsigned int) metadata.inode.inodesGroup, (unsigned int) metadata.inode.freeInodes);
	printf(BLOCK_INFO, (unsigned int) metadata.block.blockSize, (unsigned long long int) metadata.block.reservedBlocks, (unsigned long long int) metadata.block.freeBlocks, (unsigned long long int) metadata.block.totalBlocks, (unsigned int) metadata.block.firstBlock, (unsigned int) metadata.block.blockGroup, (unsigned int) metadata.block.fragsGroup);
	printf(VOLUME_INFO, metadata.volume.volumeName);
	printf(LAST_CHECK, metadata.volume.lastCheck);
	printf(LAST_MOUNT, metadata.volume.lastMount);
	printf(LAST_WRITTEN, metadata.volume.lastWritten);
}

void VIEW_showFat32MetaData(struct fat_BS fat_boot, struct fat_extBS_32 fat_boot_ext_32) {
	printf(FILESYSTEM_INFO, "FAT32");
	printf(FAT_INFO, fat_boot.oem_name, fat_boot.bytes_per_sector, fat_boot.sectors_per_cluster, fat_boot.number_reserved_sectors, fat_boot.number_fat_tables, fat_boot.number_entries_32bits_root_12_16, fat_boot_ext_32.number_sectors_fat_table_32, fat_boot_ext_32.volume_label);
}

void VIEW_fileFound(struct FileMetaData metaData) {
    printf(FILE_FOUND_METADATA, (unsigned long long int) metaData.size, metaData.createdAt);
}

void VIEW_fileNotFound() {
    printf(FILE_NOT_FOUND);
}

void VIEW_showStartFile(){
    printf(FILE_FOUND_CONTENT);
    printf(START_FILE);
}

void VIEW_showEndFile(){
    printf(END_FILE);
}

void VIEW_showFileFragment(unsigned char * info, uint64_t size){
    if(size == 0) return;
    write(1, info, size);
}
