#ifndef PRACTICA_SOA_TYPES_H
#define PRACTICA_SOA_TYPES_H

//Strcut utils
#define MAX_NAME 16
#define MAX_DATE 64

#define EXT4_NDIR_BLOCKS 12
#define EXT4_IND_BLOCK EXT4_NDIR_BLOCKS
#define EXT4_DIND_BLOCK (EXT4_IND_BLOCK + 1)
#define EXT4_TIND_BLOCK (EXT4_DIND_BLOCK + 1)
#define EXT4_N_BLOCKS (EXT4_TIND_BLOCK + 1)

//Basic Fat struct
typedef struct fat_BS{
    unsigned char 		bootjmp[3];
    unsigned char 		oem_name[8];
    unsigned short 	    bytes_per_sector;
    unsigned char		sectors_per_cluster;
    unsigned short		number_reserved_sectors;
    unsigned char		number_fat_tables;
    unsigned short		number_entries_32bits_root_12_16;
    unsigned short		total_number_sectors_small;
    unsigned char		media_type;
    unsigned short		number_sectors_fat_table_12_16;
    unsigned short		sectors_per_track;
    unsigned short		head_side_count;
    unsigned int 		hidden_sector_count;
    unsigned int 		total_number_sectors_big;

}__attribute__((packed)) fat_BS_t;//S'afegeix l'atribut de packed per evitar padding

//Fat32 struct
typedef struct fat_extBS_32{
    //extended fat32 stuff
    unsigned int		number_sectors_fat_table_32;
    unsigned short		extended_flags;
    unsigned short		fat_version;
    unsigned int		root_cluster;
    unsigned short		fat_info;
    unsigned short		backup_BS_sector;
    unsigned char 		reserved_0[12];
    unsigned char		drive_number;
    unsigned char 		reserved_1;
    unsigned char		boot_signature;
    unsigned int 		volume_id;
    unsigned char		volume_label[11];
    unsigned char		fat_type_label[8];

}__attribute__((packed)) fat_extBS_32_t;




//Super block structure
struct ext_super_block {
/*00*/unsigned int	s_inodes_count;		/* Inodes count */
    unsigned int	s_blocks_count_lo;	/* Blocks count */
    unsigned int	s_r_blocks_count_lo;	/* Reserved blocks count */
    unsigned int	s_free_blocks_count_lo;	/* Free blocks count */
/*10*/	unsigned int	s_free_inodes_count;	/* Free inodes count */
    unsigned int	s_first_data_block;	/* First Data Block */
    unsigned int	s_log_block_size;	/* Block size */
    unsigned int	s_log_cluster_size;	/* Allocation cluster size */
/*20*/	unsigned int	s_blocks_per_group;	/* # Blocks per group */
    unsigned int	s_clusters_per_group;	/* # Clusters per group */
    unsigned int	s_inodes_per_group;	/* # Inodes per group */
    unsigned int	s_mtime;		/* Mount time */
/*30*/	unsigned int	s_wtime;		/* Write time */
    unsigned short	s_mnt_count;		/* Mount count */
    unsigned short	s_max_mnt_count;	/* Maximal mount count */
    unsigned short	s_magic;		/* Magic signature */
    unsigned short	s_state;		/* File system state */
    unsigned short	s_errors;		/* Behaviour when detecting errors */
    unsigned short	s_minor_rev_level;	/* minor revision level */
/*40*/	unsigned int	s_lastcheck;		/* time of last check */
    unsigned int	s_checkinterval;	/* max. time between checks */
    unsigned int	s_creator_os;		/* OS */
    unsigned int	s_rev_level;		/* Revision level */
/*50*/	unsigned short	s_def_resuid;		/* Default uid for reserved blocks */
    unsigned short	s_def_resgid;		/* Default gid for reserved blocks */
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
    unsigned int	s_first_ino;		/* First non-reserved inode */
    unsigned short  s_inode_size;		/* size of inode structure */
    unsigned short	s_block_group_nr;	/* block group # of this superblock */
    unsigned int	s_feature_compat;	/* compatible feature set */
/*60*/	unsigned int	s_feature_incompat;	/* incompatible feature set */
    unsigned int	s_feature_ro_compat;	/* readonly-compatible feature set */
/*68*/	unsigned char	s_uuid[16];		/* 128-bit uuid for volume */
/*78*/	char	s_volume_name[16];	/* volume name */
/*88*/	char	s_last_mounted[64] ;	/* directory where last mounted */
/*C8*/	unsigned int	s_algorithm_usage_bitmap; /* For compression */
    /*
     * Performance hints.  Directory preallocation should only
     * happen if the EXT4_FEATURE_COMPAT_DIR_PREALLOC flag is on.
     */
    unsigned char	s_prealloc_blocks;	/* Nr of blocks to try to preallocate*/
    unsigned char	s_prealloc_dir_blocks;	/* Nr to preallocate for dirs */
    unsigned short	s_reserved_gdt_blocks;	/* Per group desc for online growth */
    /*
     * Journaling support valid if EXT4_FEATURE_COMPAT_HAS_JOURNAL set.
     */
/*D0*/	unsigned char	s_journal_uuid[16];	/* uuid of journal superblock */
/*E0*/	unsigned int	s_journal_inum;		/* inode number of journal file */
    unsigned int	s_journal_dev;		/* device number of journal file */
    unsigned int	s_last_orphan;		/* start of list of inodes to delete */
    unsigned int	s_hash_seed[4];		/* HTREE hash seed */
    unsigned char	s_def_hash_version;	/* Default hash version to use */
    unsigned char	s_jnl_backup_type;
    unsigned short  s_desc_size;		/* size of group descriptor */
/*100*/	unsigned int	s_default_mount_opts;
    unsigned int	s_first_meta_bg;	/* First metablock block group */
    unsigned int	s_mkfs_time;		/* When the filesystem was created */
    unsigned int	s_jnl_blocks[17];	/* Backup of the journal inode */
    /* 64bit support valid if EXT4_FEATURE_COMPAT_64BIT */
/*150*/	unsigned int	s_blocks_count_hi;	/* Blocks count */
    unsigned int	s_r_blocks_count_hi;	/* Reserved blocks count */
    unsigned int	s_free_blocks_count_hi;	/* Free blocks count */
    unsigned short	s_min_extra_isize;	/* All inodes have at least # bytes */
    unsigned short	s_want_extra_isize; 	/* New inodes should reserve # bytes */
    unsigned int	s_flags;		/* Miscellaneous flags */
    unsigned short  s_raid_stride;		/* RAID stride */
    unsigned short  s_mmp_update_interval;  /* # seconds to wait in MMP checking */
    unsigned long  s_mmp_block;            /* Block for multi-mount protection */
    unsigned int s_raid_stripe_width;    /* blocks on all data disks (N*stride)*/
    unsigned char	s_log_groups_per_flex;  /* FLEX_BG group size */
    unsigned char	s_checksum_type;	/* metadata checksum algorithm used */
    unsigned char	s_encryption_level;	/* versioning level for encryption */
    unsigned char	s_reserved_pad;		/* Padding to next 32bits */
    unsigned long	s_kbytes_written;	/* nr of lifetime kilobytes written */
    unsigned int	s_snapshot_inum;	/* Inode number of active snapshot */
    unsigned int	s_snapshot_id;		/* sequential ID of active snapshot */
    unsigned long	s_snapshot_r_blocks_count; /* reserved blocks for active snapshot's future use */
    unsigned int	s_snapshot_list;	/* inode number of the head of the on-disk snapshot list */

}__attribute__((packed));

//Block group descriptor struct
struct ext4_group_desc
{
    unsigned int	bg_block_bitmap_lo;	/* Blocks bitmap block */
    unsigned int	bg_inode_bitmap_lo;	/* Inodes bitmap block */
    unsigned int	bg_inode_table_lo;	/* Inodes table block */
    unsigned short	bg_free_blocks_count;	/* Free blocks count */
    unsigned short	bg_free_inodes_count;	/* Free inodes count */
    unsigned short	bg_used_dirs_count;	/* Directories count */
    unsigned short	bg_flags;		/* EXT4_BG_flags (INODE_UNINIT, etc) */
    unsigned int	bg_reserved[2];		/* Likely block/inode bitmap checksum */
    unsigned short  bg_itable_unused;	/* Unused inodes count */
    unsigned short  bg_checksum;		/* crc16(sb_uuid+group+desc) */
    unsigned int	bg_block_bitmap_hi;	/* Blocks bitmap block MSB */
    unsigned int	bg_inode_bitmap_hi;	/* Inodes bitmap block MSB */
    unsigned int	bg_inode_table_hi;	/* Inodes table block MSB */
    unsigned short	bg_free_blocks_count_hi;/* Free blocks count MSB */
    unsigned short	bg_free_inodes_count_hi;/* Free inodes count MSB */
    unsigned short	bg_used_dirs_count_hi;	/* Directories count MSB */
    unsigned short	bg_itable_unused_hi;	/* Unused inodes count MSB */
    unsigned int	bg_reserved2[3];
}__attribute__((packed));

struct ext4_inode {
    unsigned short	i_mode;		/* File mode */
    unsigned short	i_uid;		/* Low 16 bits of Owner Uid */
    unsigned int	i_size_lo;	/* Size in bytes */
    unsigned int	i_atime;	/* Access time */
    unsigned int	i_ctime;	/* Inode Change time */
    unsigned int	i_mtime;	/* Modification time */
    unsigned int	i_dtime;	/* Deletion Time */
    unsigned short	i_gid;		/* Low 16 bits of Group Id */
    unsigned short	i_links_count;	/* Links count */
    unsigned int	i_blocks_lo;	/* Blocks count */
    unsigned int	i_flags;	/* File flags */
    union {
        struct {
            unsigned int  l_i_version;
        } linux1;
        struct {
            unsigned int  h_i_translator;
        } hurd1;
        struct {
            unsigned int  m_i_reserved1;
        } masix1;
    } osd1;				/* OS dependent 1 */
    unsigned int	i_block[EXT4_N_BLOCKS];/* Pointers to blocks */
    unsigned int	i_generation;	/* File version (for NFS) */
    unsigned int	i_file_acl_lo;	/* File ACL */
    unsigned int	i_size_high;
    unsigned int	i_obso_faddr;	/* Obsoleted fragment address */
    union {
        struct {
            unsigned short	l_i_blocks_high; /* were l_i_reserved1 */
            unsigned short	l_i_file_acl_high;
            unsigned short	l_i_uid_high;	/* these 2 fields */
            unsigned short	l_i_gid_high;	/* were reserved2[0] */
            unsigned int	l_i_reserved2;
        } linux2;
        struct {
            unsigned short	h_i_reserved1;	/* Obsoleted fragment number/size which are removed in ext4 */
            unsigned short	h_i_mode_high;
            unsigned short	h_i_uid_high;
            unsigned short	h_i_gid_high;
            unsigned int	h_i_author;
        } hurd2;
        struct {
            unsigned short	h_i_reserved1;	/* Obsoleted fragment number/size which are removed in ext4 */
            unsigned short	m_i_file_acl_high;
            unsigned int	m_i_reserved2[2];
        } masix2;
    } osd2;				/* OS dependent 2 */
    unsigned short	i_extra_isize;
    unsigned short	i_pad1;
    unsigned int  i_ctime_extra;  /* extra Change time      (nsec << 2 | epoch) */
    unsigned int  i_mtime_extra;  /* extra Modification time(nsec << 2 | epoch) */
    unsigned int  i_atime_extra;  /* extra Access time      (nsec << 2 | epoch) */
    unsigned int  i_crtime;       /* File Creation time */
    unsigned int  i_crtime_extra; /* extra FileCreationtime (nsec << 2 | epoch) */
    unsigned int  i_version_hi;	/* high 32 bits for 64-bit version */
}__attribute__((packed));

/*
 * This is the extent on-disk structure.
 * It's used at the bottom of the tree.
 */
struct ext4_extent {
    unsigned long	ee_block;	/* first logical block extent covers */
    unsigned int	ee_len;		/* number of blocks covered by extent */
    unsigned int	ee_start_hi;	/* high 16 bits of physical block */
    unsigned long	ee_start_lo;	/* low 32 bits of physical block */
}__attribute__((packed));

/*
 * This is index on-disk structure.
 * It's used at all the levels except the bottom.
 */
struct ext4_extent_idx {
    unsigned long	ei_block;	/* index covers logical blocks from 'block' */
    unsigned long	ei_leaf_lo;	/* pointer to the physical block of the next *
				 * level. leaf or next index could be there */
    unsigned int	ei_leaf_hi;	/* high 16 bits of physical block */
    unsigned int	ei_unused;
}__attribute__((packed));

/*
 * Each block (leaves and indexes), even inode-stored has header.
 */
struct ext4_extent_header {
    unsigned int	eh_magic;	/* probably will support different formats */
    unsigned int	eh_entries;	/* number of valid entries */
    unsigned int	eh_max;		/* capacity of store in entries */
    unsigned int	eh_depth;	/* has tree real underlying blocks? */
    unsigned long	eh_generation;	/* generation of the tree */
}__attribute__((packed));

//Ext4 inode metadata
struct InodeInfo {
	int inodeSize;
	int totalInodes;
	int firstInode;
	int inodesGroup;
	int freeInodes;
};

//Ext4 block metadata
struct BlockInfo {
	double blockSize;
    long long reservedBlocks;
    long long freeBlocks;
    long long totalBlocks;
	int firstBlock;
	int blockGroup;
	int fragsGroup;
};

//Ext4 volume metadata
struct VolumeInfo {
	char volumeName[MAX_NAME];
	char lastCheck[MAX_DATE];
	char lastMount[MAX_DATE];
	char lastWritten[MAX_DATE];
};

//Ext4 metadata
struct Ext4MetaData {
	struct InodeInfo inode;
	struct BlockInfo block;
	struct VolumeInfo volume;
};

//FAT32 dir entry
typedef struct dir_entry{
    unsigned char *filename;
    unsigned char attributes;
    unsigned int first_cluster;
    unsigned int size_in_bytes;
    unsigned short date_created;
    unsigned short isLongSpecialFilename;
} __attribute__((packed)) dir_entry_aux;
#endif //PRACTICA_SOA_TYPES_H
