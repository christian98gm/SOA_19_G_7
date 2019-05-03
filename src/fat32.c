#include "fat32.h"

void FAT32_showFileSystemInfo(int fd){
	
	//Go to filesystem start
    lseek(fd, 0, SEEK_SET);
	
	//Get common Fat struct
    struct fat_BS fat_boot;
    read(fd,&fat_boot, sizeof(fat_boot));
	
	//Get Fat32 struct
    struct fat_extBS_32 fat_boot_ext_32;
    read(fd, &fat_boot_ext_32, sizeof(fat_boot_ext_32));
	
	//Show Fat32 info
	VIEW_showFat32MetaData(fat_boot, fat_boot_ext_32);
	
}
int searchFile(unsigned long cluster, int showFile);

struct fat_BS fat_boot;
struct fat_extBS_32 fat_boot_ext_32;
static unsigned short max_entries_per_cluster;
static unsigned int fd;
static unsigned long first_data_sector;
char* filename_SHORT;
char* filename_LONG;

char* formatShortFilename(char *filename);


//El boot sector esta inclos en el nombre de reserved sectors
void FAT32_showFileInfo(int fd_Aux, char* filenameAux) {
    filename_SHORT = formatShortFilename(filenameAux);
    filename_LONG = filenameAux;//ASCIItoUTFilename(filenameAux);
    fd = fd_Aux;

    lseek(fd, 0, SEEK_SET);
    read(fd, &fat_boot, sizeof(fat_boot));
    read(fd, &fat_boot_ext_32, sizeof(fat_boot_ext_32));

    max_entries_per_cluster = (fat_boot.sectors_per_cluster * fat_boot.bytes_per_sector) / FAT32_ENTRY_SIZE;
    first_data_sector = fat_boot.number_reserved_sectors +
                                      (fat_boot.number_fat_tables * fat_boot_ext_32.number_sectors_fat_table_32);
    if(searchFile(fat_boot_ext_32.root_cluster,0) == 0) VIEW_showFat32FileMetadata(NULL,0);
}
char* formatShortFilename(char *filename){
    char* filAux =  malloc(sizeof(char)*FAT32_SHORT_VOLUMENAME_MAX+sizeof(char));
    for(int i = 0; i < FAT32_SHORT_VOLUMENAME_MAX +1;i++) filAux[i] = ' ';

    int dotpos;
    for(int i = 0; i < FAT32_SHORT_VOLUMENAME_MAX +1;i++){
        if(filename[i] == '.'){
            dotpos = i + 1;
            break;
        }
        filAux[i] = toupper(filename[i]);
    }
    filAux[FAT32_SHORT_VOLUMENAME_MAX] = '\0';
    for(int i = 0; i < 3; i++){
        filAux[8+i] = toupper(filename[dotpos++]);
    }
    return filAux;
}
void parse_dir_entry(unsigned char *data, int *offset, int longname, struct dir_entry *entry);

unsigned char* formatLongFilename(unsigned char *val, int length){
    int j = 0;
    unsigned char* aux = malloc(1);
    for(int i = 0; i < length; i++){
        if(val[i] != '\0' && val[i] != 255){
            aux[j] = val[i];
            aux = realloc(aux, j+1);
            j++;
        }
    }
    aux[j] = '\0';
    free(val);
    return aux;
}
int searchFile(unsigned long cluster, int showFile){

    unsigned long first_sector = (cluster-2 * fat_boot.sectors_per_cluster) + first_data_sector;
    unsigned char entries_cluster[max_entries_per_cluster][FAT32_ENTRY_SIZE];
    lseek(fd, first_sector * fat_boot.bytes_per_sector,SEEK_SET);
    read(fd,entries_cluster,sizeof(entries_cluster));
    struct dir_entry entry;
    unsigned char first_byte;
    int num_entry_directory_in_cluster = 0;
    int offset = 0;

    while(1){
        first_byte = entries_cluster[num_entry_directory_in_cluster][0];
        if (first_byte == FAT32_ENTRY_END) {
            break;
        } else if (first_byte == FAT32_ENTRY_FREE) {
            //IGNORE
        }  else {
            unsigned char attr_long =  entries_cluster[num_entry_directory_in_cluster][0x0B];
            if(attr_long  == ATTR_LONG_NAME) {
               parse_dir_entry(entries_cluster[num_entry_directory_in_cluster], &offset, 1, &entry);
            }else{//SHORT NAME
                parse_dir_entry(entries_cluster[num_entry_directory_in_cluster], &offset, 0, &entry);
                if ((ATTR_DIRECTORY & entry.attributes) != 0 && entry.filename[0] != '.') { //DIRECTORY AND NOW SELF-BACK REFERENCE
                    if(searchFile(entry.first_cluster,0)){
                       free(entry.filename);
                       return 1;
                    }
                } else if((ATTR_HIDDEN & entry.attributes) == 0 && entry.filename[0] != '.'){ //NO HIDDEN
                    char* filAux;
                    if(entry.isLongSpecialFilename == 0){
                        filAux = filename_SHORT;
                    }else{
                        entry.filename = formatLongFilename(entry.filename, offset);
                        filAux = filename_LONG;
                    }
                    if(strcmp((char*)filAux, (char*)entry.filename) == 0){
                        if(showFile == 0) VIEW_showFat32FileMetadata(&entry,1);
                   //     else getFileInfo(entry.first_cluster);
                        free(entry.filename);
                        return 1;
                    }

                }
                offset = 0;
            }
        }
        num_entry_directory_in_cluster++;
        if(num_entry_directory_in_cluster == max_entries_per_cluster){
            unsigned long  whereToGo = (fat_boot.number_reserved_sectors * fat_boot.bytes_per_sector + cluster*4);
            lseek(fd, whereToGo,SEEK_SET);
            unsigned int next_cluster;
            read(fd,&next_cluster, sizeof(next_cluster));
            if(next_cluster >= 0xFFFFFF8) return 0;
            cluster = next_cluster;
            first_sector = ((cluster-2 & 0x0FFFFFFF) * fat_boot.sectors_per_cluster) + first_data_sector;
            lseek(fd, first_sector * fat_boot.bytes_per_sector,SEEK_SET);
            read(fd,entries_cluster,sizeof(entries_cluster));
            num_entry_directory_in_cluster = 0;
        }
    }
    free(entry.filename);
    return 0;
}

unsigned short short_little_to_big_endian(unsigned char* to_convert);
unsigned int int_little_to_big_endian(unsigned char* to_convert);


void parse_dir_entry(unsigned char *data, int *offset, int longname, struct dir_entry *entry){
    if(longname == 0){
        if(*offset == 0){
            entry->filename = (unsigned char*) malloc(sizeof(char) * FAT32_SHORT_VOLUMENAME_MAX + sizeof(char));
            memcpy(entry->filename, data, FAT32_SHORT_VOLUMENAME_MAX);
            entry->filename[FAT32_SHORT_VOLUMENAME_MAX] = '\0';
            entry->isLongSpecialFilename = 0;
        } else{
            entry->filename = (unsigned char*) realloc(entry->filename, *offset + sizeof(char));
            entry->filename[*offset] = '\0';
        }
        entry->attributes = (unsigned char) data[0x0B];
        entry->date_created = short_little_to_big_endian(&data[0x10]);
        entry->first_cluster = short_little_to_big_endian(&data[0x14])<<8;
        entry->first_cluster += short_little_to_big_endian(&data[0x1A]);
        entry->size_in_bytes = int_little_to_big_endian(&data[0x1C]);

    }else{
        entry->filename = (unsigned char*) realloc(entry->filename,*offset * sizeof(char) + sizeof(char) * FAT32_LONG_NAME_SEGMENT);
        memcpy(&entry->filename[*offset], &data[1],10);
        memcpy(&entry->filename[*offset+10], &data[14],12);
        memcpy(&entry->filename[*offset+22], &data[28],4);
        entry->isLongSpecialFilename = 1;
        *offset += FAT32_LONG_NAME_SEGMENT;
    }
}



unsigned int int_little_to_big_endian(unsigned char* to_convert){
    return to_convert[0]+(to_convert[1]<<8)+(to_convert[2]<<16)+(to_convert[3]<<24);
}


unsigned short short_little_to_big_endian(unsigned char* to_convert){
    return to_convert[0]+(to_convert[1]<<8);
}



