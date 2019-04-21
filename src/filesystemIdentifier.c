#include "filesystemIdentifier.h"

/**
 * 0 -> FAT12
 * 1 -> FAT16
 * 2 -> FAT32
 * 10 -> EXT2
 * 11 -> EXT3
 * 12 -> EXT4
 * @return
 */
#define EXT_MAG_SIGN 0xEF53

int getFATFileSystemSubType(int fd);
int getEXTFileSystemSubType(int fd);

int getFileSystemType(int fd){

    lseek(fd, 1024 + 0x38,SEEK_SET);
    unsigned short magic_signature;
    read(fd, &magic_signature, sizeof(magic_signature));
    int isEXT = (magic_signature == EXT_MAG_SIGN) ? 1: 0;
    int type;
    if(isEXT) type = getEXTFileSystemSubType(fd);
    else type = getFATFileSystemSubType(fd);

    switch (type){
        case 0:
            printf("File System not recognized (FAT12)\n");
            break;
        case 1:
            printf("File System not recognized (FAT16)\n");
            break;
        case 2:
         //   printf("Filesystem: FAT32\n");
            break;
        case 10:
            printf("File System not recognized (EXT2)\n");
            break;
        case 11:
            printf("File System not recognized (EXT3)\n");
            break;
        case 12:
          //  printf("Filesystem: EXT4\n");
            break;
    }
    return type;
}
int getEXTFileSystemSubType(int fd){
    lseek(fd, 1024, SEEK_SET);
    struct ext_super_block superblock;
    read(fd, &superblock, sizeof(struct ext_super_block));

    int hasJournal = superblock.s_feature_compat & 0x00000004;
    int hasExtAttr = superblock.s_feature_compat & 0x00000008;

    if(hasExtAttr) return 12;
    if(hasJournal) return 11;
    else return 10;


}
int getFATFileSystemSubType(int fd){
    lseek(fd,0 ,SEEK_SET);
    struct fat_BS fat_boot;
    struct fat_extBS_32 fat_boot_ext_32;
    read(fd,&fat_boot, sizeof(fat_boot)); //Llegim estructura comuna entre fat
    read(fd, &fat_boot_ext_32, sizeof(fat_boot_ext_32)); //Llegim estructura particular fat32 (o merda si es fat12/16).

    /* Depen de si es fat12/16 o si es fat32, el camp de sectors per fat esta a un lloc o un altre.
     * Si es fat32, el camp de sectors per fat de fat12/16 (fat_boot.number_sectors_fat_table_12_16) esta a 0.
     */
    int fat_size = (fat_boot.number_sectors_fat_table_12_16 == 0)? fat_boot_ext_32.number_sectors_fat_table_32 : fat_boot.number_sectors_fat_table_12_16;
    /*
     *En fat12/16 root es especial i te uns sectors dedicats a ells. En fat32, aixo es 0.
     * Consequentment fat_boot.number_entries_32bits_root_12_16 es 0, i per tant root_dir_sectors es 0
     */
    int root_dir_sectors = ((fat_boot.number_entries_32bits_root_12_16 * 32) + (fat_boot.bytes_per_sector - 1)) / fat_boot.bytes_per_sector;

    //Depenent num sectors, un o l'altre. De per si no indica si es fat12/16 o fat32, ja que es numero de sectors totals. (diria).
    int total_sectors = fat_boot.total_number_sectors_small == 0 ?  fat_boot.total_number_sectors_big : fat_boot.total_number_sectors_small;

    int data_sectors = total_sectors - (fat_boot.number_reserved_sectors + (fat_boot.number_fat_tables * fat_size) + root_dir_sectors);
    int total_clusters = data_sectors / fat_boot.sectors_per_cluster;

    int fat_type;

    if(total_clusters < 4085){
        fat_type = 0;
    }else if(total_clusters < 65525){
        fat_type = 1;
    }else {
        fat_type = 2;
    }
    return fat_type;

}