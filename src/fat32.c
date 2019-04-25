#include "fat32.h"

void FAT32_showFileSystemInfo(int fd){
	
	//Go to filesystem start
    lseek(fd, 0, SEEK_SET);
	
	//Get common Fat struct
    struct fat_BS fat_boot;
    read(fd,&fat_boot, sizeof(fat_boot));
	
	//Get Fat32 struct
    struct fat_extBS_32 fat_boot_ext_32;
    read(fd, &fat_boot_ext_32, sizeof(fat_boot_ext_32));
	
	//Show Fat32 info
	VIEW_showFat32MetaData(fat_boot, fat_boot_ext_32);
	
}
