#include "ext4.h"

uint16_t getInodeSize(int fd);
uint64_t getInodeOffset(int fd);
uint32_t findFileInode(int fd, char * filename, uint64_t offset, uint16_t blockSize, int typed);
uint32_t checkFileExistance(int fd, char * filename, uint64_t offset, uint16_t blockSize) {
    return 0;
}
struct FileMetaData getInodeMetaData(int fd, uint32_t fileInode);

uint32_t checkFileExistance2(int fd, char * filename, uint64_t offset, uint16_t blockSize);

void EXT4_showFileSystemInfo(int fd){
    
	//Get super block
	lseek(fd, SUPER_BLOCK_OFFSET, SEEK_SET);
    struct ext_super_block sb;
    read(fd, &sb, sizeof(struct ext_super_block));
 
	//Get inode info and show it
	VIEW_showExt4MetaData(EXT4S_getMetaData(sb));
	
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
        struct FileMetaData metaData;
        metaData = getInodeMetaData(fd, fileInode);
        VIEW_fileFound(metaData);
    } else {
        VIEW_fileNotFound();
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

struct FileMetaData getInodeMetaData(int fd, uint32_t fileInode) {

    //Go to inode offset
    uint64_t offset = getInodeOffset(fd);
    offset += (getInodeSize(fd) * (fileInode - 1));

    //Get inode
    struct ext4_inode inode;
    lseek(fd, offset, SEEK_SET);
    read(fd, &inode, sizeof(struct ext4_inode));

    //Get data
    struct FileMetaData fileMetaData;
    fileMetaData.size = (inode.i_size_high << 32) | inode.i_size_lo;
    DATE_getShortDate(inode.i_crtime, fileMetaData.createdAt);
    return fileMetaData;

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
        read(fd, &dirEntry, sizeof(struct ext4_dir_entry_2));
        dirEntry.name[dirEntry.name_len] = '\0';

        //Check inode 0
        if(dirEntry.inode == 0) {
            return 0;
        }

        //Compare to target
        if(dirEntry.file_type & FILE_FLAG_MASK && strcmp(dirEntry.name, filename) == 0) {
            return dirEntry.inode;
        } else {
            if(dirEntry.file_type & DIR_FLAG_MASK && strcmp(dirEntry.name, ".") != 0 && strcmp(dirEntry.name, "..") != 0) {
                uint32_t fileInode = findFileInode(fd, filename, getInodeOffset(fd) + (dirEntry.inode - 1) * getInodeSize(fd) + EXT_HEADER_OFFSET, blockSize, 1);
                if(fileInode != 0) {
                    return fileInode;
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
