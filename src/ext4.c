#include "ext4.h"
#include <stdint.h>

//Date utils
#define DATE_INFO "%s: %s"
#define LAST_CHECK "Last check"
#define LAST_MOUNT "Last mount"
#define LAST_WRITTEN "Last written"

void getDate(int secs, char * dateType, char * msg);
uint16_t getInodeSize(int fd);
uint64_t getInodeOffset(int fd);
uint32_t findFileInode(int fd, char * filename, uint64_t offset, uint16_t blockSize, int typed);
uint32_t checkFileExistance(int fd, char * filename, uint64_t offset, uint16_t blockSize) {
    return 0;
}
uint32_t checkFileExistance2(int fd, char * filename, uint64_t offset, uint16_t blockSize);

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
    int groupOffset = sb.s_first_data_block == 1 ? blockSize * 2 : blockSize;
    struct ext4_group_desc groupDesc;
    lseek(fd, groupOffset, SEEK_SET);
    read(fd, &groupDesc, sizeof(struct ext4_group_desc));

    //Get offsets
    uint64_t inodeInit = getInodeOffset(fd);
    int rootOffset = sb.s_inode_size;

    //Get root inode
    lseek(fd, inodeInit + rootOffset, SEEK_SET);
    struct ext4_inode rootInode;
    read(fd, &rootInode, sizeof(struct ext4_inode));

    //Jump to extent
    uint64_t extentOffset = inodeInit + rootOffset + EXT_HEADER_OFFSET;
    lseek(fd, extentOffset, SEEK_SET);

    //Navigate root
    uint32_t fileInode = findFileInode(fd, filename, extentOffset, blockSize, sb.s_feature_incompat & EXT_DIR_ENTRY);

    //Check if file was found
    if(fileInode != 0) {
        printf("%d -> File found", fileInode);
        //TODO: File found
    } else {
        printf("File not found");
        //TODO: File not found
    }

}

uint64_t getInodeOffset(int fd) {

    //Get super block
    lseek(fd, SUPER_BLOCK_OFFSET, SEEK_SET);
    struct ext_super_block sb;
    read(fd, &sb, sizeof(struct ext_super_block));

    //Get block group descriptor
    int blockSize = (int) pow(2, 10 + sb.s_log_block_size);
    int groupOffset = sb.s_first_data_block == 1 ? blockSize * 2 : blockSize;
    struct ext4_group_desc groupDesc;
    lseek(fd, groupOffset, SEEK_SET);
    read(fd, &groupDesc, sizeof(struct ext4_group_desc));

    //Get inode table location
    long long inodeTableBlock;
    if(sb.s_feature_incompat & LONG_FEATURE_MASK) {
        inodeTableBlock = ((long long) groupDesc.bg_inode_table_hi << 32 | groupDesc.bg_inode_table_lo);
    } else {
        inodeTableBlock = groupDesc.bg_inode_table_lo;
    }

    return inodeTableBlock * blockSize;

}

uint16_t getInodeSize(int fd) {

    //Get super block
    lseek(fd, SUPER_BLOCK_OFFSET, SEEK_SET);
    struct ext_super_block sb;
    read(fd, &sb, sizeof(struct ext_super_block));

    //Get block size
    return sb.s_inode_size;

}

uint32_t findFileInode(int fd, char * filename, uint64_t offset, uint16_t blockSize, int typed) {

    //Get header
    struct ext4_extent_header header;
    lseek(fd, offset, SEEK_SET);
    read(fd, &header, sizeof(struct ext4_extent_header));

    //Get file inode
    uint32_t fileInode;
    if(header.eh_depth == 0) {
        for(int i = 0; i < header.eh_entries; i++) {

            //Get leaf
            struct ext4_extent leaf;
            lseek(fd, offset + (i + 1) * EXT_NODE_OFFSET, SEEK_SET);
            read(fd, &leaf, sizeof(struct ext4_extent));

            //Go to data offset
            uint64_t dataOffset = (leaf.ee_start_hi << 32) | leaf.ee_start_lo;
            dataOffset *= blockSize;

            //Get inode
            fileInode = typed != 0 ? checkFileExistance2(fd, filename, dataOffset, blockSize) : checkFileExistance(fd, filename, dataOffset, blockSize);
            if(fileInode != 0) {
                return fileInode;
            }

        }
    } else {
        for(int i = 0; i < header.eh_entries; i++) {

            //Get node
            struct ext4_extent_idx node;
            lseek(fd, offset + (i + 1) * EXT_NODE_OFFSET, SEEK_SET);
            read(fd, &node, sizeof(struct ext4_extent_idx));

            //Get new header
            uint64_t nodeOffset = (node.ei_leaf_hi << 32) | node.ei_leaf_lo;
            nodeOffset *= blockSize;

            //Find file
            fileInode = findFileInode(fd, filename, nodeOffset, blockSize, typed);
            if(fileInode != 0) {
                return fileInode;
            }

        }
    }

    return 0;

}

uint32_t checkFileExistance2(int fd, char * filename, uint64_t offset, uint16_t blockSize) {

    struct ext4_dir_entry_2 dirEntry;

    //Directory loop
    uint16_t curSize = 0;
    while(curSize < blockSize) {

        //Get directory entry
        lseek(fd, offset + curSize, SEEK_SET);
        read(fd, &dirEntry, sizeof(struct ext4_dir_entry_2) - sizeof(char *));

        //Get dir name
        dirEntry.name = (char *) malloc(sizeof(char) * dirEntry.name_len + sizeof(char));
        if(dirEntry.name != NULL) {

            //Read name
            read(fd, dirEntry.name, sizeof(char) * dirEntry.name_len);
            dirEntry.name[dirEntry.name_len] = '\0';

            //Check inode 0
            if(dirEntry.inode == 0) {
                return 0;
            }

            //Compare to target
            if(dirEntry.file_type & FILE_FLAG_MASK && strcmp(dirEntry.name, filename) == 0) {
                free(dirEntry.name);
                return dirEntry.inode;
            } else {
                if(dirEntry.file_type & DIR_FLAG_MASK && strcmp(dirEntry.name, ".") != 0 && strcmp(dirEntry.name, "..") != 0) {
                    free(dirEntry.name);
                    uint32_t fileInode = findFileInode(fd, filename, getInodeOffset(fd) + (dirEntry.inode - 1) * getInodeSize(fd) + EXT_HEADER_OFFSET, blockSize, 1);
                    if(fileInode != 0) {
                        return fileInode;
                    }
                } else {
                    free(dirEntry.name);
                }
            }

        }

        //Increase size
        curSize += dirEntry.rec_len;

    }

    return 0;

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
