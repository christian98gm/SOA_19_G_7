#include "identifier.h"

int getFATFileSystemSubType(int fd);
int getEXTFileSystemSubType(int fd);

int FSID_getFileSystemType(int fd){
	
	//Look for magic number
    lseek(fd, SUPER_BLOCK_OFFSET + MAGIC_NUMBER_OFFSET, SEEK_SET);
    unsigned short magic_signature;
    read(fd, &magic_signature, sizeof(magic_signature));
	
	//Get filesystem type
    if(magic_signature == EXT_MAG_SIGN) {
		return getEXTFileSystemSubType(fd);
	} else {
		return getFATFileSystemSubType(fd);
    }
	
}

char * FSID_getFileSystemName(int type) {
	switch(type) {
        case 0:
            return FAT12_FS_NAME;
        case 1:
            return FAT16_FS_NAME;
        case 2:
			return FAT32_FS_NAME;
		case 10:
            return EXT2_FS_NAME;
        case 11:
            return EXT3_FS_NAME;
        case 12:
			return EXT4_FS_NAME;
		default:
			return UNDEFINED_FS_NAME;
    }
}

int getEXTFileSystemSubType(int fd){
	
	//Get superblock
    lseek(fd, SUPER_BLOCK_OFFSET, SEEK_SET);
    struct ext_super_block superblock;
    read(fd, &superblock, sizeof(struct ext_super_block));

	//Check Ext4
	if(superblock.s_feature_incompat & EXTENT_FEATURE_MASK) {
		return EXT4_FS;
	}
	
	//Check Ext3
	if(superblock.s_feature_compat & JOURNAL_FEATURE_MASK) {
		return EXT3_FS;
	} else {
		return EXT2_FS;
	}

}

int getFATFileSystemSubType(int fd){
	
	//Go to filesystem start
    lseek(fd, 0, SEEK_SET);
	
	//Get common Fat struct
    struct fat_BS fat_boot;
	read(fd,&fat_boot, sizeof(fat_boot));

	//Get Fat32 struct
    struct fat_extBS_32 fat_boot_ext_32;
    read(fd, &fat_boot_ext_32, sizeof(fat_boot_ext_32));

	//Get sector size (Fat32 has 'number_sectors_fat_table_12_16' to 0)
    int fat_size = fat_boot.number_sectors_fat_table_12_16 == 0 ? fat_boot_ext_32.number_sectors_fat_table_32 : fat_boot.number_sectors_fat_table_12_16;
    
	//Get root sectors, (Fat12/Fat16 reserved values, 0 value for Fat32)
    int root_dir_sectors = ((fat_boot.number_entries_32bits_root_12_16 * 32) + (fat_boot.bytes_per_sector - 1)) / fat_boot.bytes_per_sector;

    //Get total sectors
    int total_sectors = fat_boot.total_number_sectors_small == 0 ?  fat_boot.total_number_sectors_big : fat_boot.total_number_sectors_small;

	//Get data sectors
    int data_sectors = total_sectors - (fat_boot.number_reserved_sectors + fat_boot.number_fat_tables * fat_size + root_dir_sectors);
    
	//Get total clusters
	int total_clusters = data_sectors / fat_boot.sectors_per_cluster;

	//Get Fat type
    if(total_clusters <= MAX_FAT12){
        return FAT12_FS;
    } else if(total_clusters <= MAX_FAT16){
        return FAT16_FS;
    } else {
        return FAT32_FS;
    }

}