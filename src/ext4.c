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
	metadata.inode.inodesGroup = -1;
	metadata.inode.freeInodes = sb.s_free_inodes_count;
	
	//Get block info
	metadata.block.blockSize = pow(2, 10 + sb.s_log_block_size);
	if(sb.s_feature_incompat & LONG_FEATURE_MASK) {
		metadata.block.reservedBlocks = ((long) sb.s_r_blocks_count_hi << 32) | ((long) sb.s_r_blocks_count_lo & 0xFFFFFFFFL);
		metadata.block.freeBlocks = ((long) sb.s_free_blocks_count_hi << 32) | ((long) sb.s_free_blocks_count_lo & 0xFFFFFFFFL);
		metadata.block.totalBlocks = ((long) sb.s_blocks_count_hi << 32) | ((long) sb.s_blocks_count_lo & 0xFFFFFFFFL);
	} else {
		metadata.block.reservedBlocks = (long) sb.s_r_blocks_count_lo;
		metadata.block.freeBlocks = (long) sb.s_free_blocks_count_lo;
		metadata.block.totalBlocks = (long) sb.s_blocks_count_lo;
	}
	metadata.block.firstBlock = sb.s_first_data_block;
	metadata.block.blockGroup = sb.s_block_group_nr;
	metadata.block.fragsGroup = -1;
	
	//Get volume info
	strcpy(metadata.volume.volumeName, sb.s_volume_name);
	getDate(sb.s_lastcheck, LAST_CHECK, metadata.volume.lastCheck);
	getDate(sb.s_mtime, LAST_MOUNT, metadata.volume.lastMount);
	getDate(sb.s_wtime, LAST_WRITTEN, metadata.volume.lastWritten);
	
	//Show info
	VIEW_showExt4MetaData(metadata);
	
}

void getDate(int secs, char * dateType, char * msg) {

	//Get time
	time_t t = secs;
	struct tm * time = localtime(&t);
	
	//Get date
	sprintf(msg, DATE_INFO, dateType, asctime(time));
	
}
