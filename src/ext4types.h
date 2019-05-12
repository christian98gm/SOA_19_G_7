#ifndef PRACTICA_SOA_EXT4TYPES_H
#define PRACTICA_SOA_EXT4TYPES_H

#include <stdint.h>

/**
 * CONSTANTS
 **/

//Masks
#define LONG_FEATURE_MASK 0x80
#define EXTENT_FEATURE_MASK 0x40
#define JOURNAL_FEATURE_MASK 0x4
#define EXT_DIR_ENTRY_MASK 0x2
#define DIR_ENTRY_FILE_MASK 0x1
#define DIR_ENTRY_DIR_MASK 0x2
#define INODE_FILE_MASK 0x8000
#define INODE_DIR_MASK 0x4000

//Offsets
#define SUPER_BLOCK_OFFSET 1024
#define MAGIC_NUMBER_OFFSET 0x38
#define EXT_HEADER_OFFSET 0x28
#define EXT_NODE_OFFSET 0x0C

//Others
#define EXT_MAG_SIGN 0xEF53
#define MAX_VOL_NAME 16
#define EXT4_N_BLOCKS 15
#define EXT4_NAME_LEN 256
#define ROOT_INODE_INDEX 2

/**
 * MAIN STRUCTS
 **/

/*
 * Structure of the super block
 */
struct ext_super_block {
    /*00*/
    uint32_t s_inodes_count;            /* Inodes count */
    uint32_t s_blocks_count_lo;         /* Blocks count */
    uint32_t s_r_blocks_count_lo;       /* Reserved blocks count */
    uint32_t s_free_blocks_count_lo;    /* Free blocks count */
    /*10*/
    uint32_t s_free_inodes_count;       /* Free inodes count */
    uint32_t s_first_data_block;        /* First Data Block */
    uint32_t s_log_block_size;          /* Block size */
    uint32_t s_log_cluster_size;        /* Allocation cluster size */
    /*20*/
    uint32_t s_blocks_per_group;        /* # Blocks per group */
    uint32_t s_clusters_per_group;      /* # Clusters per group */
    uint32_t s_inodes_per_group;        /* # Inodes per group */
    uint32_t s_mtime;                   /* Mount time */
    /*30*/
    uint32_t s_wtime;                   /* Write time */
    uint16_t s_mnt_count;               /* Mount count */
    uint16_t s_max_mnt_count;           /* Maximal mount count */
    uint16_t s_magic;                   /* Magic signature */
    uint16_t s_state;                   /* File system state */
    uint16_t s_errors;                  /* Behaviour when detecting errors */
    uint16_t s_minor_rev_level;         /* minor revision level */
    /*40*/
    unsigned int s_lastcheck;           /* time of last check */
    unsigned int s_checkinterval;       /* max. time between checks */
    unsigned int s_creator_os;          /* OS */
    unsigned int s_rev_level;           /* Revision level */
    /*50*/
    unsigned short s_def_resuid;        /* Default uid for reserved blocks */
    unsigned short s_def_resgid;        /* Default gid for reserved blocks */
    /*
     * These fields are for EXT4_DYNAMIC_REV superblocks only.
     *
     * Note: the difference between the compatible feature set and
     * the incompatible feature set is that if there is a bit set
     * in the incompatible feature set that the kernel doesn't
     * know about, it should refuse to mount the filesystem.
     *
     * e2fsck's requirements are more strict; if it doesn't know
     * about a feature in either the compatible or incompatible
     * feature set, it must abort and not try to meddle with
     * things it doesn't understand...
     */
    uint32_t s_first_ino;               /* First non-reserved inode */
    uint16_t s_inode_size;              /* size of inode structure */
    uint16_t s_block_group_nr;          /* block group # of this superblock */
    uint32_t s_feature_compat;          /* compatible feature set */
    /*60*/
    uint32_t s_feature_incompat;        /* incompatible feature set */
    uint32_t s_feature_ro_compat;       /* readonly-compatible feature set */
    /*68*/
    uint8_t s_uuid[16];                 /* 128-bit uuid for volume */
    /*78*/
    char s_volume_name[16];             /* volume name */
    /*88*/
    char s_last_mounted[64];            /* directory where last mounted */
    /*C8*/
    uint32_t s_algorithm_usage_bitmap;  /* For compression */
    /*
     * Performance hints.  Directory preallocation should only
     * happen if the EXT4_FEATURE_COMPAT_DIR_PREALLOC flag is on.
     */
    uint8_t s_prealloc_blocks;          /* Nr of blocks to try to preallocate*/
    uint8_t s_prealloc_dir_blocks;      /* Nr to preallocate for dirs */
    uint16_t s_reserved_gdt_blocks;     /* Per group desc for online growth */
    /*
     * Journaling support valid if EXT4_FEATURE_COMPAT_HAS_JOURNAL set.
     */
    /*D0*/
    uint8_t s_journal_uuid[16];         /* uuid of journal superblock */
    /*E0*/
    uint32_t s_journal_inum;            /* inode number of journal file */
    uint32_t s_journal_dev;             /* device number of journal file */
    uint32_t s_last_orphan;             /* start of list of inodes to delete */
    uint32_t s_hash_seed[4];            /* HTREE hash seed */
    uint8_t s_def_hash_version;         /* Default hash version to use */
    uint8_t s_jnl_backup_type;
    uint16_t s_desc_size;               /* size of group descriptor */
    /*100*/
    uint32_t s_default_mount_opts;
    uint32_t s_first_meta_bg;           /* First metablock block group */
    uint32_t s_mkfs_time;               /* When the filesystem was created */
    uint32_t s_jnl_blocks[17];          /* Backup of the journal inode */
    /* 64bit support valid if EXT4_FEATURE_COMPAT_64BIT */
    /*150*/
    uint32_t s_blocks_count_hi;         /* Blocks count */
    uint32_t s_r_blocks_count_hi;       /* Reserved blocks count */
    uint32_t s_free_blocks_count_hi;    /* Free blocks count */
    uint16_t s_min_extra_isize;         /* All inodes have at least # bytes */
    uint16_t s_want_extra_isize;        /* New inodes should reserve # bytes */
    uint32_t s_flags;                   /* Miscellaneous flags */
    uint16_t s_raid_stride;             /* RAID stride */
    uint16_t s_mmp_update_interval;     /* # seconds to wait in MMP checking */
    uint64_t s_mmp_block;               /* Block for multi-mount protection */
    uint32_t s_raid_stripe_width;       /* blocks on all data disks (N*stride)*/
    uint8_t s_log_groups_per_flex;      /* FLEX_BG group size */
    uint8_t s_reserved_char_pad2;
    uint16_t s_reserved_pad;
    uint32_t s_reserved[162];           /* Padding to the end of the block */
};

/*
 * Structure of a blocks group descriptor
 */
struct ext4_group_desc {
    uint32_t bg_block_bitmap_lo;        /* Blocks bitmap block */
    uint32_t bg_inode_bitmap_lo;        /* Inodes bitmap block */
    uint32_t bg_inode_table_lo;         /* Inodes table block */
    uint16_t bg_free_blocks_count_lo;   /* Free blocks count */
    uint16_t bg_free_inodes_count_lo;   /* Free inodes count */
    uint16_t bg_used_dirs_count_lo;     /* Directories count */
    uint16_t bg_flags;                  /* EXT4_BG_flags (INODE_UNINIT, etc) */
    uint32_t bg_exclude_bitmap_lo;      /* Exclude bitmap for snapshots */
    uint16_t bg_block_bitmap_csum_lo;   /* crc32c(s_uuid+grp_num+bbitmap) LE */
    uint16_t bg_inode_bitmap_csum_lo;   /* crc32c(s_uuid+grp_num+ibitmap) LE */
    uint16_t bg_itable_unused_lo;       /* Unused inodes count */
    uint16_t bg_checksum;               /* crc16(sb_uuid+group+desc) */
    uint32_t bg_block_bitmap_hi;        /* Blocks bitmap block MSB */
    uint32_t bg_inode_bitmap_hi;        /* Inodes bitmap block MSB */
    uint32_t bg_inode_table_hi;         /* Inodes table block MSB */
    uint16_t bg_free_blocks_count_hi;   /* Free blocks count MSB */
    uint16_t bg_free_inodes_count_hi;   /* Free inodes count MSB */
    uint16_t bg_used_dirs_count_hi;     /* Directories count MSB */
    uint16_t bg_itable_unused_hi;       /* Unused inodes count MSB */
    uint32_t bg_exclude_bitmap_hi;      /* Exclude bitmap block MSB */
    uint16_t bg_block_bitmap_csum_hi;   /* crc32c(s_uuid+grp_num+bbitmap) BE */
    uint16_t bg_inode_bitmap_csum_hi;   /* crc32c(s_uuid+grp_num+ibitmap) BE */
    uint32_t bg_reserved;
};

/*
 * Structure of an inode on the disk
 */
struct ext4_inode {
    uint16_t i_mode;                    /* File mode */
    uint16_t i_uid;                     /* Low 16 bits of Owner Uid */
    uint32_t i_size_lo;                 /* Size in bytes */
    uint32_t i_atime;                   /* Access time */
    uint32_t i_ctime;                   /* Inode Change time */
    uint32_t i_mtime;                   /* Modification time */
    uint32_t i_dtime;                   /* Deletion Time */
    uint16_t i_gid;                     /* Low 16 bits of Group Id */
    uint16_t i_links_count;             /* Links count */
    uint32_t i_blocks_lo;               /* Blocks count */
    uint32_t i_flags;                   /* File flags */
    union {
        struct {
            uint32_t l_i_version;
        } linux1;
        struct {
            uint32_t h_i_translator;
        } hurd1;
        struct {
            uint32_t m_i_reserved1;
        } masix1;
    } osd1;                             /* OS dependent 1 */
    uint32_t i_block[EXT4_N_BLOCKS];    /* Pointers to blocks */
    uint32_t i_generation;              /* File version (for NFS) */
    uint32_t i_file_acl_lo;             /* File ACL */
    uint32_t i_size_high;
    uint32_t i_obso_faddr;              /* Obsoleted fragment address */
    union {
        struct {
            uint16_t l_i_blocks_high;   /* were l_i_reserved1 */
            uint16_t l_i_file_acl_high;
            uint16_t l_i_uid_high;      /* these 2 fields */
            uint16_t l_i_gid_high;      /* were reserved2[0] */
            uint16_t l_i_checksum_lo;   /* crc32c(uuid+inum+inode) LE */
            uint16_t l_i_reserved;
        } linux2;
        struct {
            uint16_t h_i_reserved1;     /* Obsoleted fragment number/size which are removed in ext4 */
            uint16_t h_i_mode_high;
            uint16_t h_i_uid_high;
            uint16_t h_i_gid_high;
            uint32_t h_i_author;
        } hurd2;
        struct {
            uint16_t h_i_reserved1;     /* Obsoleted fragment number/size which are removed in ext4 */
            uint16_t m_i_file_acl_high;
            uint32_t m_i_reserved2[2];
        } masix2;
    } osd2;                             /* OS dependent 2 */
    uint16_t i_extra_isize;
    uint16_t i_checksum_hi;             /* crc32c(uuid+inum+inode) BE */
    uint32_t i_ctime_extra;             /* extra Change time      (nsec << 2 | epoch) */
    uint32_t i_mtime_extra;             /* extra Modification time(nsec << 2 | epoch) */
    uint32_t i_atime_extra;             /* extra Access time      (nsec << 2 | epoch) */
    uint32_t i_crtime;                  /* File Creation time */
    uint32_t i_crtime_extra;            /* extra FileCreationtime (nsec << 2 | epoch) */
    uint32_t i_version_hi;              /* high 32 bits for 64-bit version */
    uint32_t i_projid;                  /* Project ID */
};

/*
 * Each block (leaves and indexes), even inode-stored has header.
 */
struct ext4_extent_header {
    uint16_t eh_magic;          /* probably will support different formats */
    uint16_t eh_entries;        /* number of valid entries */
    uint16_t eh_max;            /* capacity of store in entries */
    uint16_t eh_depth;          /* has tree real underlying blocks? */
    uint32_t eh_generation;     /* generation of the tree */
};

/*
 * This is index on-disk structure.
 * It's used at all the levels except the bottom.
 */
struct ext4_extent_idx {
    uint32_t ei_block;      /* index covers logical blocks from 'block' */
    uint32_t ei_leaf_lo;    /* pointer to the physical block of the next *
                             * level. leaf or next index could be there */
    uint16_t ei_leaf_hi;    /* high 16 bits of physical block */
    uint16_t ei_unused;
};

/*
 * This is the extent on-disk structure.
 * It's used at the bottom of the tree.
 */
struct ext4_extent {
    uint32_t ee_block;      /* first logical block extent covers */
    uint16_t ee_len;        /* number of blocks covered by extent */
    uint16_t ee_start_hi;   /* high 16 bits of physical block */
    uint32_t ee_start_lo;   /* low 32 bits of physical block */
};

/*
 * Structure of a directory entry
 */
struct ext4_dir_entry {
    uint32_t inode;             /* Inode number */
    uint16_t rec_len;           /* Directory entry length */
    uint16_t name_len;          /* Name length */
    char name[EXT4_NAME_LEN];   /* File name */
};

/*
 * The new version of the directory entry.  Since EXT4 structures are
 * stored in intel byte order, and the name_len field could never be
 * bigger than 255 chars, it's safe to reclaim the extra byte for the
 * file_type field.
 */
struct ext4_dir_entry_2 {
    uint32_t inode;             /* Inode number */
    uint16_t rec_len;           /* Directory entry length */
    uint8_t name_len;           /* Name length */
    uint8_t file_type;
    char name[EXT4_NAME_LEN];   /* File name */
};

#endif //PRACTICA_SOA_EXT4TYPES_H
