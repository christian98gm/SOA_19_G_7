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
int searchFile(unsigned long cluster);

struct fat_BS fat_boot;
struct fat_extBS_32 fat_boot_ext_32;
unsigned int max_entries_per_cluster;
unsigned int fd;
unsigned long first_data_sector;
unsigned char* filename;

int asciiToUTF8(unsigned char* out, int *outlen, const unsigned char* in, int *inlen);
int UTF8ToUTF16BE(unsigned char* outb, int *outlen,const unsigned char* in, int *inlen);

unsigned char* ASCIItoUTFilename(){
    int length_file_utf8 = strlen(filename);
    unsigned char* filenameutf8 = malloc(sizeof(char)*length_file_utf8);
    asciiToUTF8(filenameutf8,&length_file_utf8,(unsigned char*)filename, &length_file_utf8);

    unsigned char* filenameutf16 = malloc(sizeof(char)*length_file_utf8*2);
    int length_file_utf16 = length_file_utf8*2;
    UTF8ToUTF16BE(filenameutf16, &length_file_utf16,filenameutf8, &length_file_utf8);
}
//El boot sector esta inclos en el nombre de reserved sectors
int FAT32_showFileInfo(int fd_Aux, char* filenameAux) {
    lseek(fd, 0, SEEK_SET);
    read(fd, &fat_boot, sizeof(fat_boot));
    read(fd, &fat_boot_ext_32, sizeof(fat_boot_ext_32));

    filename = filenameAux;
    fd = fd_Aux;

    max_entries_per_cluster = (fat_boot.sectors_per_cluster * fat_boot.bytes_per_sector) / 32;
    first_data_sector = fat_boot.number_reserved_sectors +
                                      (fat_boot.number_fat_tables * fat_boot_ext_32.number_sectors_fat_table_32);
    searchFile(fat_boot_ext_32.root_cluster);
}
void parse_dir_entry(unsigned char *data, int *offset, int longname, dir_entry *entry);

int searchFile(unsigned long cluster){

    unsigned long first_sector = ((cluster - 2) * fat_boot.sectors_per_cluster) + first_data_sector;

    unsigned char entries_cluster[max_entries_per_cluster][FAT32_ENTRY_SIZE];
    lseek(fd, first_sector * fat_boot.bytes_per_sector,SEEK_SET);
    read(fd,entries_cluster,sizeof(entries_cluster));

    dir_entry entry;
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
                if ((ATTR_DIRECTORY & entry.attributes) != 0) {
                    if(searchFile(entry.first_cluster)) return 1;
                } else {
                    if(!entry.isLongSpecialFilename){
                        for(int i = 0; strlen(entry.filename); i++){
                            filename[i] = tolower(filename[i]);
                        }
                    }else{
                        filename = ASCIItoUTFilename(filename);
                    }
                    if(!strcmp((char*)filename, entry.filename)){
                        //TODO: PRINT INFO
                        return 1;
                    }
                }
            }
        }
        offset = 0;
        num_entry_directory_in_cluster++;
        if(num_entry_directory_in_cluster == max_entries_per_cluster){
            unsigned long fat_offset = cluster * 4; // x4 perque son 4 bytes per entrada. Cada entrada FAT equival a un cluster
            unsigned long fat_sector = fat_boot.number_reserved_sectors + (fat_offset / fat_boot.bytes_per_sector); //Sector FAT
            unsigned long fat_sector_offset = fat_offset % fat_boot.bytes_per_sector; //
            lseek(fd,fat_sector + fat_sector_offset , SEEK_SET); //Em poso al sector de la FAT
            unsigned long next_cluster;
            read(fd,&next_cluster, sizeof(next_cluster));
            lseek(fd, (next_cluster & 0x0FFFFFFF) * fat_boot.bytes_per_sector,SEEK_SET);
            num_entry_directory_in_cluster = 0;
        }
    }
    return 0;
}

unsigned short short_little_to_big_endian(unsigned short to_convert);
unsigned int int_little_to_big_endian(unsigned int to_convert);


void parse_dir_entry(unsigned char *data, int *offset, int longname, dir_entry *entry){
    if(!longname){
        if(!offset){
            entry->filename = (char*) malloc(sizeof(char) * FAT32_SHORT_VOLUMENAME_MAX + sizeof(char));
            memcpy(entry->filename, data, FAT32_SHORT_VOLUMENAME_MAX);
            entry->filename[FAT32_SHORT_VOLUMENAME_MAX] = '\0';
        } else{
            entry->filename = (char*) realloc(entry->filename,*offset + sizeof(char));
            entry->filename[*offset] = '\0';
        }
        entry->attributes = (unsigned char) data[0x0B];
        entry->first_cluster = short_little_to_big_endian((unsigned short) data[0x14])<<8;
        entry->first_cluster += short_little_to_big_endian((unsigned short) data[0x1A]);
        entry->file_size = int_little_to_big_endian((unsigned short) data[0x1C]);
        entry->isLongSpecialFilename = 0;
    }else{
        entry->filename = (char*) realloc(entry->filename,*offset * sizeof(char) + sizeof(char) * FAT32_LONG_NAME_SEGMENT);
        memcpy(&entry->filename[*offset], &data[1],10);
        memcpy(&entry->filename[*offset+10], &data[14],12);
        memcpy(&entry->filename[*offset+22], &data[28],4);
        entry->isLongSpecialFilename = 1;
        *offset += FAT32_LONG_NAME_SEGMENT;
    }
}



unsigned int int_little_to_big_endian(unsigned int to_convert){
    unsigned char *aux = (unsigned char*)&to_convert;
    return aux[0]+(aux[1]<<8)+(aux[2]<<16)+(aux[3]<<24);
}


unsigned short short_little_to_big_endian(unsigned short to_convert){
    unsigned char *aux = (unsigned char*)&to_convert;
    return aux[0]+(aux[1]<<8);
}

/**
 * asciiToUTF8:
 * @out:  a pointer to an array of bytes to store the result
 * @outlen:  the length of @out
 * @in:  a pointer to an array of ASCII chars
 * @inlen:  the length of @in
 *
 * Take a block of ASCII chars in and try to convert it to an UTF-8
 * block of chars out.
 * Returns 0 if success, or -1 otherwise
 * The value of @inlen after return is the number of octets consumed
 *     as the return value is positive, else unpredictiable.
 * The value of @outlen after return is the number of ocetes consumed.
 */

int asciiToUTF8(unsigned char* out, int *outlen, const unsigned char* in, int *inlen) {
    unsigned char* outstart = out;
    const unsigned char* base = in;
    const unsigned char* processed = in;
    unsigned char* outend = out + *outlen;
    const unsigned char* inend;
    unsigned int c;
    int bits;

    inend = in + (*inlen);
    while ((in < inend) && (out - outstart + 5 < *outlen)) {
        c= *in++;

        /* assertion: c is a single UTF-4 value */
        if (out >= outend)
            break;
        if      (c <    0x80) {  *out++=  c;                bits= -6; }
        else {
            *outlen = out - outstart;
            *inlen = processed - base;
            return(-1);
        }

        for ( ; bits >= 0; bits-= 6) {
            if (out >= outend)
                break;
            *out++= ((c >> bits) & 0x3F) | 0x80;
        }
        processed = (const unsigned char*) in;
    }
    *outlen = out - outstart;
    *inlen = processed - base;
    return(0);
}

static int xmlLittleEndian = 1;
/**
 * UTF8ToUTF16BE:
 * @outb:  a pointer to an array of bytes to store the result
 * @outlen:  the length of @outb
 * @in:  a pointer to an array of UTF-8 chars
 * @inlen:  the length of @in
 *
 * Take a block of UTF-8 chars in and try to convert it to an UTF-16BE
 * block of chars out.
 *
 * Returns the number of byte written, or -1 by lack of space, or -2
 *     if the transcoding failed.
 */
int UTF8ToUTF16BE(unsigned char* outb, int *outlen,const unsigned char* in, int *inlen){
    unsigned short* out = (unsigned short*) outb;
    const unsigned char* processed = in;
    unsigned short* outstart= out;
    unsigned short* outend;
    const unsigned char* inend= in+*inlen;
    unsigned int c, d;
    int trailing;
    unsigned char *tmp;
    unsigned short tmp1, tmp2;

    if (in == NULL) {
        /*
	 * initialization, add the Byte Order Mark
	 */
        if (*outlen >= 2) {
            outb[0] = 0xFE;
            outb[1] = 0xFF;
            *outlen = 2;
            *inlen = 0;
#ifdef DEBUG_ENCODING
            xmlGenericError(xmlGenericErrorContext,
		    "Added FEFF Byte Order Mark\n");
#endif
            return(2);
        }
        *outlen = 0;
        *inlen = 0;
        return(0);
    }
    outend = out + (*outlen / 2);
    while (in < inend) {
        d= *in++;
        if      (d < 0x80)  { c= d; trailing= 0; }
        else if (d < 0xC0)  {
            /* trailing byte in leading position */
            *outlen = out - outstart;
            *inlen = processed - in;
            return(-2);
        } else if (d < 0xE0)  { c= d & 0x1F; trailing= 1; }
        else if (d < 0xF0)  { c= d & 0x0F; trailing= 2; }
        else if (d < 0xF8)  { c= d & 0x07; trailing= 3; }
        else {
            /* no chance for this in UTF-16 */
            *outlen = out - outstart;
            *inlen = processed - in;
            return(-2);
        }

        if (inend - in < trailing) {
            break;
        }

        for ( ; trailing; trailing--) {
            if ((in >= inend) || (((d= *in++) & 0xC0) != 0x80))  break;
            c <<= 6;
            c |= d & 0x3F;
        }

        /* assertion: c is a single UTF-4 value */
        if (c < 0x10000) {
            if (out >= outend)  break;
            if (xmlLittleEndian) {
                tmp = (unsigned char *) out;
                *tmp = c >> 8;
                *(tmp + 1) = c;
                out++;
            } else {
                *out++ = c;
            }
        }
        else if (c < 0x110000) {
            if (out+1 >= outend)  break;
            c -= 0x10000;
            if (xmlLittleEndian) {
                tmp1 = 0xD800 | (c >> 10);
                tmp = (unsigned char *) out;
                *tmp = tmp1 >> 8;
                *(tmp + 1) = (unsigned char) tmp1;
                out++;

                tmp2 = 0xDC00 | (c & 0x03FF);
                tmp = (unsigned char *) out;
                *tmp = tmp2 >> 8;
                *(tmp + 1) = (unsigned char) tmp2;
                out++;
            } else {
                *out++ = 0xD800 | (c >> 10);
                *out++ = 0xDC00 | (c & 0x03FF);
            }
        }
        else
            break;
        processed = in;
    }
    *outlen = (out - outstart) * 2;
    *inlen = processed - in;
    return(0);
}






