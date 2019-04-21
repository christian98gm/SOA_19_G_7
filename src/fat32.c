#include "fat32.h"



int showBasicFAT32FileSystemInfo(int fd){
    lseek(fd,0 ,SEEK_SET);
    struct fat_BS fat_boot;
    struct fat_extBS_32 fat_boot_ext_32;
    read(fd,&fat_boot, sizeof(fat_boot)); //Llegim estructura comuna entre fat
    read(fd, &fat_boot_ext_32, sizeof(fat_boot_ext_32)); //Llegim estructura particular fat32 (o merda si es fat12/16).
    /**
     * ---- Filesystem Information ----
    Filesystem: FAT32
    System Name: mkdosfs
    Sector Size: 1024
    Sectors per Cluster: 1
    Reserved Sectors: 1
    Number of FATs: 2
    Maximum Root Entries: 512
    Sectors per FAT: 16
    Label: TEST2
     */
    printf("\n----Filesystem Information----\n");
    printf("Filesystem: FAT32\n");
    printf("System Name: "); //TODO: Segur que es aixo ?
    for(int i = 0; i < 8; i++){
        printf("%c",fat_boot.oem_name[i]);
    }
    printf("\n");
    printf("Sector Size: %u\n", fat_boot.bytes_per_sector);
    printf("Sectors per cluster %u\n",fat_boot.sectors_per_cluster);
    printf("Reserved Sectors %u\n",fat_boot.number_reserved_sectors);
    printf("Number of FATs: %u\n", fat_boot.number_fat_tables);
    printf("Maximum Root Entries: %u\n", fat_boot.number_entries_32bits_root_12_16); //TODO: Maximum ?? Segur ?????
    printf("Sectors per FAT: %u\n",fat_boot_ext_32.number_sectors_fat_table_32);
    printf("Label: ");
    for(int i = 0; i < 11; i++){
        printf("%c",fat_boot_ext_32.volume_label[i]);
    }
    printf("\n");

    return 0;
}
