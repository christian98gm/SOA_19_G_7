#include "fat32.h"

/**
 * VARIABLES
 **/

static unsigned int fd;
static unsigned long first_data_sector;
static unsigned short max_entries_per_cluster;

struct fat_BS fat_boot;
struct fat_extBS_32 fat_boot_ext_32;
char * filename_SHORT;
char * filename_LONG;

/**
 * AUX FUNCTION HEADERS
 **/

void setBasicInfo(int fd_Aux, char * filenameAux);

unsigned short short_little_to_big_endian(unsigned char * to_convert);

unsigned int int_little_to_big_endian(unsigned char * to_convert);

/**
 * FILE METADATA/INFO FUNCTION HEADERS
 **/

int searchFile(unsigned int cluster, int showFile);



unsigned char * getFileInfo(struct dir_entry entry);


/**
 * FILE NAME PARSE HEADERS
 **/

char * formatShortFilename(char * filename);


unsigned char * formatLongFilename(struct dir_entry* entry, int length);

void parse_dir_entry(unsigned char * data,unsigned long * offset, int longname, struct dir_entry * entry, int pos);

/**
 * FUNCTIONS
 **/

void FAT32_showFileSystemInfo(int fd){

    //Go to filesystem start
    lseek(fd, 0, SEEK_SET);

    //Get common fat struct
    struct fat_BS fat_boot;
    read(fd,&fat_boot, sizeof(fat_boot));

    //Get fat32 struct
    struct fat_extBS_32 fat_boot_ext_32;
    read(fd, &fat_boot_ext_32, sizeof(fat_boot_ext_32));

    //Show fat32 info
    VIEW_showFat32MetaData(fat_boot, fat_boot_ext_32);

}

void FAT32_showFileMetadata(int fd_Aux, char * filenameAux) {
    setBasicInfo(fd_Aux, filenameAux);
    if(searchFile(fat_boot_ext_32.root_cluster, 0) == 0) {
        VIEW_fileNotFound();
    }
}

//TODO: FIX
void FAT32_showFileInfo(int fd_Aux, char * filenameAux){
    setBasicInfo(fd_Aux, filenameAux);
    if(searchFile(fat_boot_ext_32.root_cluster, 1) == 0) {
        VIEW_fileNotFound();
    }
}

/**
 * AUX FUNCTION IMPLEMENTATIONS
 **/

void setBasicInfo(int fd_Aux, char * filenameAux) {

    //Get file name
    filename_SHORT = formatShortFilename(filenameAux);
    filename_LONG = filenameAux;    //ASCIItoUTFilename(filenameAux);
    fd = fd_Aux;

    //Get fat strcuts
    lseek(fd, 0, SEEK_SET);
    read(fd, &fat_boot, sizeof(fat_boot));
    read(fd, &fat_boot_ext_32, sizeof(fat_boot_ext_32));

    //Get max entries
    max_entries_per_cluster = (fat_boot.sectors_per_cluster * fat_boot.bytes_per_sector) / FAT32_ENTRY_SIZE;

    //Get first data sector
    first_data_sector = fat_boot.number_reserved_sectors +
                        (fat_boot.number_fat_tables * fat_boot_ext_32.number_sectors_fat_table_32);

}

unsigned int int_little_to_big_endian(unsigned char * to_convert) {
    return to_convert[0] + (to_convert[1] << 8) + (to_convert[2] << 16) + (to_convert[3] << 24);
}

unsigned short short_little_to_big_endian(unsigned char * to_convert) {
    return to_convert[0] + (to_convert[1] << 8);
}

/**
 * FILE METADATA/INFO FUNCTION IMPLEMENTATIONS
 **/

int searchFile(unsigned int cluster, int showFile) {

    unsigned long first_sector = ((cluster - 2) * fat_boot.sectors_per_cluster) + first_data_sector;
    unsigned char entries_cluster[max_entries_per_cluster][FAT32_ENTRY_SIZE];
    lseek(fd, first_sector * fat_boot.bytes_per_sector, SEEK_SET);
    read(fd, entries_cluster, sizeof(entries_cluster));

    struct dir_entry entry;
    unsigned char first_byte;
    unsigned long num_entry_directory_in_cluster = 0;
    unsigned long offset = 0;
    unsigned long  whereToGo;

    while(1) {

        first_byte = entries_cluster[num_entry_directory_in_cluster][0];
        if(first_byte == FAT32_ENTRY_END) {
            break;
        } else if (first_byte == FAT32_ENTRY_FREE) {
            //IGNORE
        } else {

            unsigned char attr_long =  entries_cluster[num_entry_directory_in_cluster][0x0B];
            if(attr_long  == ATTR_LONG_NAME) {
                parse_dir_entry(entries_cluster[num_entry_directory_in_cluster], &offset, 1, &entry, entries_cluster[num_entry_directory_in_cluster][0]);
            } else {//SHORT NAME

                parse_dir_entry(entries_cluster[num_entry_directory_in_cluster], &offset, 0, &entry,0);
                if((ATTR_DIRECTORY & entry.attributes) != 0 && entry.filename[0] != '.') { //DIRECTORY AND NOW SELF-BACK REFERENCE
                    if(searchFile(entry.first_cluster, showFile)){
                        free(entry.filename);
                        return 1;
                    }
                } else if((ATTR_HIDDEN & entry.attributes) == 0 && entry.filename[0] != '.'){ //NO HIDDEN

                    char * filAux;
                    if(entry.isLongSpecialFilename == 0){
                        filAux = filename_SHORT;
                    } else {
                        entry.filename = formatLongFilename(&entry, offset);
                        filAux = filename_LONG;
                    }

                    if(strcmp((char *) filAux, (char *) entry.filename) == 0) {

                        if(showFile == 0) {
                            struct FileMetaData fileMetaData;
                            fileMetaData.size = (uint64_t) entry.size_in_bytes;
                            DATE_getShortDateFromBytes(entry.date_created, fileMetaData.createdAt);
                            VIEW_fileFound(fileMetaData);
                        } else {
                            getFileInfo(entry);

                        }


                        return 1;

                    }


                }
                free(entry.filename);
                entry.total_pos = 0;
                offset = 0;

            }

        }

        num_entry_directory_in_cluster++;
        if(num_entry_directory_in_cluster == max_entries_per_cluster) {
            whereToGo = fat_boot.number_reserved_sectors * fat_boot.bytes_per_sector + (cluster*4);
            lseek(fd, whereToGo,SEEK_SET);
            read(fd,&cluster, sizeof(cluster));

            if(cluster >= 0x0FFFFFF8 || cluster == 0x0FFFFFF7) {
                break;
            }

            first_sector = (((cluster & 0x0FFFFFFF) -2) * fat_boot.sectors_per_cluster) + first_data_sector;
            lseek(fd, first_sector * fat_boot.bytes_per_sector,SEEK_SET);
            read(fd, entries_cluster, sizeof(entries_cluster));
            num_entry_directory_in_cluster = 0;

        }
    }
    return 0;

}

unsigned char * getFileInfo(struct dir_entry entry) {

    if(entry.first_cluster == 0) return NULL;

    unsigned long size_cluster =  fat_boot.sectors_per_cluster*fat_boot.bytes_per_sector;
    unsigned long num_clusters = entry.size_in_bytes/size_cluster;
    unsigned long offset_last = entry.size_in_bytes % size_cluster;

    unsigned char* file_info = malloc(size_cluster);
    unsigned long  whereToGo;
    unsigned int cluster =  entry.first_cluster &  0x0FFFFFFF ;
    unsigned long first_sector;

    first_sector = (((cluster & 0x0FFFFFFF) -2)* fat_boot.sectors_per_cluster) + first_data_sector;
    lseek(fd, first_sector * fat_boot.bytes_per_sector,SEEK_SET);
    unsigned long i;
    VIEW_showStartFile();
    for(i = 0; i < num_clusters;i++){
        read(fd,file_info,sizeof(unsigned char)*size_cluster);
        VIEW_showFileFragment(file_info,sizeof(unsigned char)*size_cluster);
        whereToGo = fat_boot.number_reserved_sectors * fat_boot.bytes_per_sector + (cluster*4);
        lseek(fd, whereToGo,SEEK_SET);
        read(fd,&cluster, sizeof(cluster));
        first_sector = (((cluster & 0x0FFFFFFF) -2) * fat_boot.sectors_per_cluster) + first_data_sector;
        lseek(fd, first_sector * fat_boot.bytes_per_sector,SEEK_SET);
    }
    read(fd,file_info,offset_last);
    VIEW_showFileFragment(file_info,offset_last);
    VIEW_showEndFile();
    return file_info;

}

/**
 * FILE NAME PARSE HEADERS
 **/

char * formatShortFilename(char *filename){

    char * filAux =  malloc(sizeof(char) * FAT32_SHORT_VOLUMENAME_MAX + sizeof(char));
    for(int i = 0; i < FAT32_SHORT_VOLUMENAME_MAX + 1; i++) {
        filAux[i] = ' ';
    }

    int dotpos = -1;
    for(int i = 0; i < FAT32_SHORT_VOLUMENAME_MAX + 1; i++) {
        if(filename[i] == '.') {
            dotpos = i + 1;
            break;
        }
        if(filename[i] == '\0') {
            break;
        }
        filAux[i] = toupper(filename[i]);
    }

    filAux[FAT32_SHORT_VOLUMENAME_MAX] = '\0';
    if(dotpos != -1) {
        for(int i = 0; i < 3; i++) {
            filAux[8+i] = toupper(filename[dotpos++]);
        }
    }

    return filAux;

}

unsigned char * formatLongFilename(struct dir_entry* entry, int length) {

    unsigned char* filen = malloc(length + 2);
    int min = 99999;
    int pos_min = 0;
    for(int i = 0; i < entry->total_pos; i++){
        for(int j = 0; j < entry->total_pos; j++){
            if(min > entry->pos[j]){
                min = entry->pos[j];
                pos_min = j;
            }
        }
        memcpy(&filen[26*i], &(entry->filename[pos_min*26]), 26);
        entry->pos[pos_min] = 9999;
        min = 9999;
        pos_min = 0;
    }
    unsigned char* ptr = malloc(length);
   int j = 0;
    for(int i = 0; i < length; i++) {
        if(filen[i] != '\0' && filen[i] != 255) {
            ptr = realloc( ptr, j + 3);
            ptr[j] = filen[i];
            j++;
        }
    }

    ptr[j] = '\0';
    free(filen);
    free(entry->filename);
    return ptr;

}

void parse_dir_entry(unsigned char * data,unsigned long * offset, int longname, struct dir_entry * entry, int pos){

    if(longname == 0) {

        if(*offset == 0) {
            entry->filename = (unsigned char *) malloc (sizeof(char) * FAT32_SHORT_VOLUMENAME_MAX + sizeof(char));
            memcpy(entry->filename, data, FAT32_SHORT_VOLUMENAME_MAX);
            entry->filename[FAT32_SHORT_VOLUMENAME_MAX] = '\0';
            entry->isLongSpecialFilename = 0;
        } else {
            entry->filename = (unsigned char *) realloc(entry->filename, *offset + sizeof(char));
            entry->filename[*offset] = '\0';
        }

        entry->attributes = (unsigned char) data[0x0B];
        entry->date_created = short_little_to_big_endian(&data[0x10]);
        entry->first_cluster = short_little_to_big_endian(&data[0x14]) << 8;
        entry->first_cluster += short_little_to_big_endian(&data[0x1A]);
        entry->size_in_bytes = int_little_to_big_endian(&data[0x1C]);

    } else {
        if(*offset == 0){
            entry->filename = (unsigned char *) malloc(*offset * sizeof(char) + sizeof(char) * FAT32_LONG_NAME_SEGMENT);
        }else{
            entry->filename = (unsigned char *) realloc(entry->filename, *offset * sizeof(char) + sizeof(char) * FAT32_LONG_NAME_SEGMENT);
        }

        memcpy(&entry->filename[*offset], &data[1], 10);
        memcpy(&entry->filename[*offset + 10], &data[14], 12);
        memcpy(&entry->filename[*offset + 22], &data[28], 4);
        entry->isLongSpecialFilename = 1;
        *offset += FAT32_LONG_NAME_SEGMENT;
        entry->pos[entry->total_pos] = pos;
        entry->total_pos = entry->total_pos +1;
    }

}
