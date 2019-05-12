#ifndef PRACTICA_SOA_TYPES_H
#define PRACTICA_SOA_TYPES_H

#include <stdint.h>

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
