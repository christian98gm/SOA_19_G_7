#include "ext4.h"

/**
 * AUX CONSTANTS
 **/

#define __FILE_ENTRY 0
#define __DIR_ENTRY 1
#define __OTHER_ENTRY 2

/**
 * CORE FUNCTIONS HEADER
 **/

void navigateFileExtentTree(int fd, uint64_t offset, uint16_t blockSize, struct ext_super_block sb, uint64_t tableOffset, uint64_t fileSize, uint64_t * curSize);

int getFileFragment(int fd, uint64_t offset, uint64_t size);

uint32_t navigateDirExtentTree(int fd, char *filename, uint64_t offset, uint16_t blockSize, struct ext_super_block sb, uint64_t tableOffset);

uint32_t getFileInode(int fd, char *filename, uint64_t offset, uint16_t blockSize, struct ext_super_block sb, uint64_t tableOffset);

/**
 * METADATA FUNCTIONS HEADER
 **/

struct Ext4MetaData getSuperBlockMetaData(struct ext_super_block sb);

struct FileMetaData getInodeMetaData(struct ext4_inode inode, uint16_t inodeSize);

/**
 * AUX FUNCTIONS HEADER
 **/

struct ext_super_block getSuperBlock(int fd);

struct ext4_group_desc getGroupDescriptor(int fd, struct ext_super_block sb, uint32_t blockSize);

uint64_t getInodeTableOffset(struct ext_super_block sb, struct ext4_group_desc groupDesc, uint32_t blockSize);

struct ext4_inode getInodeFromTable(int fd, uint64_t tableOffset, uint32_t inodeIndex, uint16_t inodeSize);

int getEntryType(int fd, struct ext4_dir_entry_2 dirEntry, struct ext_super_block sb, uint64_t tableOffset);

/**
 * FUNCTIONS
 **/

void EXT4_showFileSystemInfo(int fd){
	VIEW_showExt4MetaData(getSuperBlockMetaData(getSuperBlock(fd)));
}

void EXT4_showFileMetadata(int fd, char *filename) {

    //Get super block
    struct ext_super_block sb = getSuperBlock(fd);

    //Get block size
    uint32_t blockSize = (uint32_t) pow(2, 10 + sb.s_log_block_size);

    //Get block group descriptor
    struct ext4_group_desc groupDesc = getGroupDescriptor(fd, sb, blockSize);

    //Get inode table offset and root inode offset
    uint64_t inodeTableOffset = getInodeTableOffset(sb, groupDesc, blockSize);
    uint16_t rootInodeOffset = (ROOT_INODE_INDEX - 1) * sb.s_inode_size;

    //Search file inode
    uint32_t fileInode = navigateDirExtentTree(fd, filename, inodeTableOffset + rootInodeOffset + EXT_HEADER_OFFSET,
                                               blockSize, sb, inodeTableOffset);
    if(fileInode != 0) {
        VIEW_fileFound(getInodeMetaData(getInodeFromTable(fd, inodeTableOffset, fileInode, sb.s_inode_size), sb.s_inode_size));
    } else {
        VIEW_fileNotFound();
    }

}

void EXT4_showFileInfo(int fd, char * filename) {

    //Get super block
    struct ext_super_block sb = getSuperBlock(fd);

    //Get block size
    uint32_t blockSize = (uint32_t) pow(2, 10 + sb.s_log_block_size);

    //Get block group descriptor
    struct ext4_group_desc groupDesc = getGroupDescriptor(fd, sb, blockSize);

    //Get inode table offset and root inode offset
    uint64_t inodeTableOffset = getInodeTableOffset(sb, groupDesc, blockSize);
    uint16_t rootInodeOffset = (ROOT_INODE_INDEX - 1) * sb.s_inode_size;

    //Search file inode
    uint32_t fileInode = navigateDirExtentTree(fd, filename, inodeTableOffset + rootInodeOffset + EXT_HEADER_OFFSET,
                                               blockSize, sb, inodeTableOffset);
    if(fileInode != 0) {

        //Get file size
        struct ext4_inode inode = getInodeFromTable(fd, inodeTableOffset, fileInode, sb.s_inode_size);
        uint64_t fileSize = (uint64_t) inode.i_size_high << 32 | inode.i_size_lo;

        //Get file data
        uint64_t curSize = 0;
        VIEW_showStartFile();
        navigateFileExtentTree(fd, inodeTableOffset + (fileInode - 1) * sb.s_inode_size + EXT_HEADER_OFFSET, blockSize, sb, inodeTableOffset, fileSize, &curSize);
        VIEW_showEndFile();

    } else {
        VIEW_fileNotFound();
    }

}

/**
 * CORE FUNCTIONS IMPLEMENTATION
 **/

void navigateFileExtentTree(int fd, uint64_t offset, uint16_t blockSize, struct ext_super_block sb, uint64_t tableOffset, uint64_t fileSize, uint64_t * curSize) {

    //Get header
    struct ext4_extent_header header;
    lseek(fd, offset, SEEK_SET);
    read(fd, &header, sizeof(struct ext4_extent_header));

    //Check depth
    if(header.eh_depth == 0) {

        //Get leaves data
        *curSize = 0;
        for(int i = 0; i < header.eh_entries; i++) {

            //Get leaf
            struct ext4_extent leaf;
            lseek(fd, offset + (i + 1) * EXT_NODE_OFFSET, SEEK_SET);
            read(fd, &leaf, sizeof(struct ext4_extent));

            //Get data offset
            uint64_t dataOffset = (uint64_t) leaf.ee_start_hi << 32 | leaf.ee_start_lo;
            dataOffset *= blockSize;

            //Search file data
            for(int j = 0; j < leaf.ee_len; j++) {

                //Get file fragment
                uint64_t size = fileSize - *curSize <= blockSize ? fileSize - *curSize : blockSize;
                if(getFileFragment(fd, dataOffset + (j * blockSize), size) > 0) {
                    (*curSize) += size;
                } else {
                    break;
                }

            }

        }
    } else {
        for(int i = 0; i < header.eh_entries; i++) {

            //Get extent node
            struct ext4_extent_idx node;
            lseek(fd, offset + (i + 1) * EXT_NODE_OFFSET, SEEK_SET);
            read(fd, &node, sizeof(struct ext4_extent_idx));

            //Get next extent offset
            uint64_t nodeOffset = (uint64_t) node.ei_leaf_hi << 32 | node.ei_leaf_lo;
            nodeOffset *= blockSize;

            //Find file
            uint64_t newSize = 0;
            navigateFileExtentTree(fd, nodeOffset, blockSize, sb, tableOffset, fileSize, &newSize);
            (*curSize) += newSize;

        }
    }

}

int getFileFragment(int fd, uint64_t offset, uint64_t size) {

    //Check size
    if(size == 0) {
        return -1;
    }

    //Get data
    char * data = (char *) malloc(size);
    if(data != NULL) {
        lseek(fd, offset, SEEK_SET);
        read(fd, data, size);
        VIEW_showFileFragment(data, size);
        free(data);
        return 1;
    } else {
        return -1;
    }

}

uint32_t navigateDirExtentTree(int fd, char *filename, uint64_t offset, uint16_t blockSize, struct ext_super_block sb,
                               uint64_t tableOffset) {

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

            //Get data offset
            uint64_t dataOffset = (uint64_t) leaf.ee_start_hi << 32 | leaf.ee_start_lo;
            dataOffset *= blockSize;

            //Search file inode
            for(int j = 0; j < leaf.ee_len; j++) {
                fileInode = getFileInode(fd, filename, dataOffset + (j * blockSize), blockSize, sb, tableOffset);
                if(fileInode != 0) {
                    return fileInode;
                }
            }

        }
    } else {
        for(int i = 0; i < header.eh_entries; i++) {

            //Get extent node
            struct ext4_extent_idx node;
            lseek(fd, offset + (i + 1) * EXT_NODE_OFFSET, SEEK_SET);
            read(fd, &node, sizeof(struct ext4_extent_idx));

            //Get next extent offset
            uint64_t nodeOffset = (uint64_t) node.ei_leaf_hi << 32 | node.ei_leaf_lo;
            nodeOffset *= blockSize;

            //Find file
            fileInode = navigateDirExtentTree(fd, filename, nodeOffset, blockSize, sb, tableOffset);
            if(fileInode != 0) {
                return fileInode;
            }

        }
    }

    //File not found
    return 0;

}

uint32_t getFileInode(int fd, char *filename, uint64_t offset, uint16_t blockSize, struct ext_super_block sb, uint64_t tableOffset) {

    struct ext4_dir_entry_2 dirEntry;

    //Directory loop
    uint16_t curSize = 0;
    while(curSize < blockSize) {

        //Get directory entry
        lseek(fd, offset + curSize, SEEK_SET);
        read(fd, &dirEntry, sizeof(struct ext4_dir_entry_2));

        //Delimit entry name
        uint16_t nameLength;
        if(sb.s_feature_incompat & EXT_DIR_ENTRY_MASK) {
            nameLength = (uint16_t) dirEntry.name_len;
        } else {
            nameLength = (uint16_t) dirEntry.name_len << 8 | dirEntry.file_type;
        }
        dirEntry.name[nameLength] = '\0';

        //Check if is a valid entry
        if(dirEntry.inode == 0) {
            return 0;
        }

        //Get file type
        switch(getEntryType(fd, dirEntry, sb, tableOffset)) {
            case __FILE_ENTRY:
                if(strcmp(dirEntry.name, filename) == 0) {
                    return dirEntry.inode;
                }
                break;
            case __DIR_ENTRY:
                if(strcmp(dirEntry.name, ".") != 0 && strcmp(dirEntry.name, "..") != 0) {
                    uint32_t fileInode = navigateDirExtentTree(fd, filename,
                                                               tableOffset + (dirEntry.inode - 1) * sb.s_inode_size +
                                                               EXT_HEADER_OFFSET, blockSize, sb, tableOffset);
                    if(fileInode != 0) {
                        return fileInode;
                    }
                }
                break;
        }

        //Increase size
        curSize += dirEntry.rec_len;

    }

    return 0;

}

/**
 * METADATA FUCNTIONS IMPLEMENTATION
 **/

struct Ext4MetaData getSuperBlockMetaData(struct ext_super_block sb) {

    //Get inode info
    struct Ext4MetaData metaData;
    metaData.inode.inodeSize = sb.s_inode_size;
    metaData.inode.totalInodes = sb.s_inodes_count;
    metaData.inode.firstInode = sb.s_first_ino;
    metaData.inode.inodesGroup = sb.s_inodes_per_group;
    metaData.inode.freeInodes = sb.s_free_inodes_count;

    //Get block info
    metaData.block.blockSize = (uint32_t) pow(2, 10 + sb.s_log_block_size);
    if(sb.s_feature_incompat & LONG_FEATURE_MASK) {
        metaData.block.reservedBlocks = (uint64_t) sb.s_r_blocks_count_hi << 32 | sb.s_r_blocks_count_lo;
        metaData.block.freeBlocks = (uint64_t) sb.s_free_blocks_count_hi << 32 | sb.s_free_blocks_count_lo;
        metaData.block.totalBlocks = (uint64_t) sb.s_blocks_count_hi << 32 | sb.s_blocks_count_lo;
    } else {
        metaData.block.reservedBlocks = (uint64_t) sb.s_r_blocks_count_lo;
        metaData.block.freeBlocks = (uint64_t) sb.s_free_blocks_count_lo;
        metaData.block.totalBlocks = (uint64_t) sb.s_blocks_count_lo;
    }
    metaData.block.firstBlock = sb.s_first_data_block;
    metaData.block.blockGroup = sb.s_blocks_per_group;
    metaData.block.fragsGroup = sb.s_clusters_per_group;

    //Get volume info
    strcpy(metaData.volume.volumeName, sb.s_volume_name);
    DATE_getLongDate(sb.s_lastcheck, metaData.volume.lastCheck);
    DATE_getLongDate(sb.s_mtime, metaData.volume.lastMount);
    DATE_getLongDate(sb.s_wtime, metaData.volume.lastWritten);

    return metaData;

}

struct FileMetaData getInodeMetaData(struct ext4_inode inode, uint16_t inodeSize) {
    struct FileMetaData fileMetaData;
    fileMetaData.size = (uint64_t) inode.i_size_high << 32 | inode.i_size_lo;
    if(inodeSize > 128) {
        DATE_getShortDate(inode.i_crtime, fileMetaData.createdAt);
    } else {
        fileMetaData.createdAt[0] = '-';
        fileMetaData.createdAt[1] = '\0';
    }
    return fileMetaData;
}

/**
 * AUX FUNCTIONS IMPLEMENTATION
 **/

struct ext_super_block getSuperBlock(int fd) {
    struct ext_super_block sb;
    lseek(fd, SUPER_BLOCK_OFFSET, SEEK_SET);
    read(fd, &sb, sizeof(struct ext_super_block));
    return sb;
}

struct ext4_group_desc getGroupDescriptor(int fd, struct ext_super_block sb, uint32_t blockSize) {
    uint32_t groupOffset = sb.s_first_data_block == 1 ? blockSize * 2 : blockSize;
    struct ext4_group_desc groupDesc;
    lseek(fd, groupOffset, SEEK_SET);
    read(fd, &groupDesc, sizeof(struct ext4_group_desc));
    return groupDesc;
}

uint64_t getInodeTableOffset(struct ext_super_block sb, struct ext4_group_desc groupDesc, uint32_t blockSize) {

    //Get inode table location
    uint64_t inodeTableBlock;
    if(sb.s_feature_incompat & LONG_FEATURE_MASK) {
        inodeTableBlock = (uint64_t) groupDesc.bg_inode_table_hi << 32 | groupDesc.bg_inode_table_lo;
    } else {
        inodeTableBlock = (uint64_t) groupDesc.bg_inode_table_lo;
    }

    return inodeTableBlock * blockSize;

}

struct ext4_inode getInodeFromTable(int fd, uint64_t tableOffset, uint32_t inodeIndex, uint16_t inodeSize) {
    struct ext4_inode inode;
    lseek(fd, tableOffset + (inodeIndex - 1) * inodeSize, SEEK_SET);
    read(fd, &inode, sizeof(struct ext4_inode));
    return inode;
}

int getEntryType(int fd, struct ext4_dir_entry_2 dirEntry, struct ext_super_block sb, uint64_t tableOffset) {

    if(sb.s_feature_incompat & EXT_DIR_ENTRY_MASK) {
        if(dirEntry.file_type & DIR_ENTRY_FILE_MASK) {
            return __FILE_ENTRY;
        } else if(dirEntry.file_type & DIR_ENTRY_DIR_MASK) {
            return __DIR_ENTRY;
        } else {
            return __OTHER_ENTRY;
        }
    } else {

        //Get inode
        struct ext4_inode inode = getInodeFromTable(fd, tableOffset, dirEntry.inode, sb.s_inode_size);

        //Check entry type
        if(inode.i_mode & INODE_FILE_MASK) {
            return __FILE_ENTRY;
        } else if(inode.i_mode & INODE_DIR_MASK) {
            return __DIR_ENTRY;
        } else {
            return __OTHER_ENTRY;
        }

    }

}
