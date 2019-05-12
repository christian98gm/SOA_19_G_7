//
// Created by christian on 12/05/2019.
//

#include "ext4struct.h"

struct Ext4MetaData EXT4S_getMetaData(struct ext_super_block sb) {

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
