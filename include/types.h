#ifndef PRACTICA_SOA_TYPES_H
#define PRACTICA_SOA_TYPES_H

//Strcut utils
#define MAX_NAME 16
#define MAX_DATE 64

//Basic Fat struct
typedef struct fat_BS
{
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
typedef struct fat_extBS_32
{
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
	long reservedBlocks;
	long freeBlocks;
	long totalBlocks;
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

#endif //PRACTICA_SOA_TYPES_H
