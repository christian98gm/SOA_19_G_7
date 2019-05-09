#include "ext4.h"

//Date utils
#define DATE_INFO "%s: %s"
#define LAST_CHECK "Last check"
#define LAST_MOUNT "Last mount"
#define LAST_WRITTEN "Last written"

void getDate(int secs, char * dateType, char * msg);

void EXT4_showFileSystemInfo(int fd){
    
	//Get super block
	lseek(fd, SUPER_BLOCK_OFFSET, SEEK_SET);
    struct ext_super_block sb;
    read(fd, &sb, sizeof(struct ext_super_block));
 
	//Get inode info
	struct Ext4MetaData metadata;
	metadata.inode.inodeSize = sb.s_inode_size;
	metadata.inode.totalInodes = sb.s_inodes_count;
	metadata.inode.firstInode = sb.s_first_ino;
	metadata.inode.inodesGroup = sb.s_inodes_per_group;
	metadata.inode.freeInodes = sb.s_free_inodes_count;
	
	//Get block info
	metadata.block.blockSize = pow(2, 10 + sb.s_log_block_size);
	if(sb.s_feature_incompat & LONG_FEATURE_MASK) {
		metadata.block.reservedBlocks = (long long) sb.s_r_blocks_count_hi << 32 | (long) sb.s_r_blocks_count_lo;
		metadata.block.freeBlocks = (long long) sb.s_free_blocks_count_hi << 32 | (long) sb.s_free_blocks_count_lo;
		metadata.block.totalBlocks = (long long) sb.s_blocks_count_hi << 32 | (long) sb.s_blocks_count_lo;
	} else {
		metadata.block.reservedBlocks = (long) sb.s_r_blocks_count_lo;
		metadata.block.freeBlocks = (long) sb.s_free_blocks_count_lo;
		metadata.block.totalBlocks = (long) sb.s_blocks_count_lo;
	}
	metadata.block.firstBlock = sb.s_first_data_block;
	metadata.block.blockGroup = sb.s_blocks_per_group;
	metadata.block.fragsGroup = sb.s_clusters_per_group;
	
	//Get volume info
	strcpy(metadata.volume.volumeName, sb.s_volume_name);
	getDate(sb.s_lastcheck, LAST_CHECK, metadata.volume.lastCheck);
	getDate(sb.s_mtime, LAST_MOUNT, metadata.volume.lastMount);
	getDate(sb.s_wtime, LAST_WRITTEN, metadata.volume.lastWritten);
	
	//Show info
	VIEW_showExt4MetaData(metadata);
	
}

void EXT4_showFileMetadata(int fd, char *filename) {

    //Get super block
    lseek(fd, SUPER_BLOCK_OFFSET, SEEK_SET);
    struct ext_super_block sb;
    read(fd, &sb, sizeof(struct ext_super_block));

    //Get block group descriptor
    int blockSize = (int) pow(2, 10 + sb.s_log_block_size);
    struct ext4_group_desc groupDesc;
    lseek(fd, blockSize == 1024 ? blockSize * 2 : blockSize, SEEK_SET);
    read(fd, &groupDesc, sizeof(struct ext4_group_desc));

    //Get total inodes
    int inodeCount = sb.s_inodes_per_group;
    if(sb.s_feature_incompat & LONG_FEATURE_MASK) {
        inodeCount -= ((int) groupDesc.bg_itable_unused_hi << 16 | groupDesc.bg_itable_unused);
    } else {
        inodeCount -= groupDesc.bg_itable_unused;
    }

    //Get inode table location
    long long inodeTableBlock;
    if(sb.s_feature_incompat & LONG_FEATURE_MASK) {
        inodeTableBlock = ((long long) groupDesc.bg_inode_table_hi << 32 | (long) groupDesc.bg_inode_table_lo);
    } else {
        inodeTableBlock = (long) groupDesc.bg_inode_table_lo;
    }

    //Get root inode
    struct ext4_inode rootInode;
    int offset = inodeTableBlock * blockSize;
    lseek(fd, offset + sizeof(struct ext4_inode), SEEK_SET);
    read(fd, &rootInode, sizeof(struct ext4_inode));

    //Get extent header
    struct ext4_extent_header header;
    header.eh_magic = (rootInode.i_block[12] >> 16) & 0xFF;
    header.eh_entries = rootInode.i_block[12] & 0xFF;
    header.eh_max = (rootInode.i_block[13] >> 16) & 0xFF;
    header.eh_depth = rootInode.i_block[13] & 0xFF;
    header.eh_generation = rootInode.i_block[14];

    return;

}

void EXT4_showFileInfo(int fd, char *filename) {

}

void getDate(int secs, char * dateType, char * msg) {

	//Get time
	time_t t = secs;
	struct tm * time = localtime(&t);
	
	//Get date
	sprintf(msg, DATE_INFO, dateType, asctime(time));
	
}
