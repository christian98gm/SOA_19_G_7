#include "ext4.h"

int showBasicEXT4FileSystemInfo(int fd){
    lseek(fd, 1024, SEEK_SET);
    struct ext_super_block superblock;
    read(fd, &superblock, sizeof(struct ext_super_block));
    /**
     * ---- Filesystem Information ----
Filesystem: EXT4
INODE INFO
Inode Size: 128
Number of Inodes: 64
First Inode: 11
Inodes Group: 64
Free Inodes: 53
BLOCK INFO
Block Size: 4096
Reserved Blocks: 102
Free Blocks: 1012
Total Blocks: 2048
First Block: 0
Block group: 32768
Frags Group: 32768
VOLUME INFO
Volume name: ext4_prova1
Last check: Mon May 1 21:52:59 2017
Last mount: Mon May 1 21:52:59 2017
Last written: Mon May 1 21:52:59 2017
     */
    printf("\n----Filesystem Information----\n");
    printf("Filesystem: EXT4\n");
    printf("\nINODE INFO\n");
    printf("Inode Size: %u",superblock.s_inode_size);
    return 0;
}