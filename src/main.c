#include <stdio.h>
#include <fcntl.h>
#include <zconf.h>
#include "filesystemIdentifier.h"
#include <string.h>
#include "fat32.h"
#include "ext4.h"

#define GET_INFO "-info"
#define GET_FILE_METADATA "-search"
#define GET_FILE_INFO "-show"

int main(int argc, char *argv[]) {
    if (argc != 3) {
        printf("The invocation format of the executable must be the following: ./executable_name <operation> <volume_name> [<filename>]\n");
        return 1;
    }

    chdir("/Users/jordana/Desktop/Practica_SOA/files"); //Es perque treballo amb CLion

    int fd = open(argv[2], O_RDONLY); //obrir en mode lectura
    if (fd < 0) {
        printf("Error. Filesystem not found.\n");
        return 1;
    }

    int type = getFileSystemType(fd);
    if (type != 2 && type != 12) return 1;

    if (strcmp(argv[1], GET_INFO) == 0) {
        if(type == 2) showBasicFAT32FileSystemInfo(fd);
        else showBasicEXT4FileSystemInfo(fd);
    }else if (strcmp(argv[1], GET_FILE_METADATA) == 0){
        if(argc < 4){
            printf("Error: Incorrect parameter number\n");
            return 1;
        }
    }else if (strcmp(argv[1], GET_FILE_INFO) == 0){
        if(argc < 4){
            printf("Error: Incorrect parameter number\n");
            return 1;
        }
    }else{
        printf("Operation could not be recognized. It must be one of the following: -info | -search | -show\n");
        printf("Remember that the invocation format of the executable must be the following: "
               "./executable_name <operation> <volume_name> [<filename>]\n");
        return 1;
    }

    return 0;
}

