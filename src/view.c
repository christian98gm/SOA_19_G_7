#include "view.h"

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
	printf(FILESYSTEM_INFO, EXT4_FS_NAME);
	printf(INODE_INFO, metadata.inode.inodeSize, metadata.inode.totalInodes, metadata.inode.firstInode, metadata.inode.inodesGroup, metadata.inode.freeInodes);
	printf(BLOCK_INFO, (int) metadata.block.blockSize, metadata.block.reservedBlocks, metadata.block.freeBlocks, metadata.block.totalBlocks, metadata.block.firstBlock, metadata.block.blockGroup, metadata.block.fragsGroup);
	printf(VOLUME_INFO, metadata.volume.volumeName);
	printf("%s", metadata.volume.lastCheck);
	printf("%s", metadata.volume.lastMount);
	printf("%s\n", metadata.volume.lastWritten);
}

void VIEW_showFat32MetaData(struct fat_BS fat_boot, struct fat_extBS_32 fat_boot_ext_32) {
	printf(FILESYSTEM_INFO, FAT32_FS_NAME);
	printf(FAT_INFO, fat_boot.oem_name, fat_boot.bytes_per_sector, fat_boot.sectors_per_cluster, fat_boot.number_reserved_sectors, fat_boot.number_fat_tables, fat_boot.number_entries_32bits_root_12_16, fat_boot_ext_32.number_sectors_fat_table_32, fat_boot_ext_32.volume_label);
}
