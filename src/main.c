#include <fcntl.h>

#include "argval.h"
#include "identifier.h"
#include "ext4.h"
#include "fat32.h"
#include "view.h"

/**
 * CONSTANTS
 **/

#define FILES_PATH "./files/"
#define MAX_PATH 128

/**
 * MAIN
 **/

int main(int argc, char * argv[]) {

	//Check args
	int operation = ARGVAL_validate(argc, argv);
    if(!operation) {
		return 1;
	}
    
    //CLION dir redirect
    chdir("/Users/jordana/Desktop/SOA_19_G_7");

	//Get volume full path
	char file[MAX_PATH];
	strcpy(file, FILES_PATH);
	strcat(file, argv[2]);
	
	//Open volume to read
    int fd = open(file, O_RDONLY);
    if(fd < 0) {
        VIEW_missingVolume(argv[2]);
        return 1;
    }

	//Get filesystem type
    int type = FSID_getFileSystemType(fd);
    if(type != FAT32_FS && type != EXT4_FS) {
		VIEW_undefinedFileSystem(FSID_getFileSystemName(type));
		close(fd);
		return 1;
	}

	//Check operation
	switch(operation) {
		case GET_INFO_CODE:
			if(type == FAT32_FS) {
				FAT32_showFileSystemInfo(fd);
			} else {
				EXT4_showFileSystemInfo(fd);
			}
			break;
		case GET_FILE_METADATA_CODE:
		    if(type == FAT32_FS) {
                FAT32_showFileMetadata(fd, argv[3]);
            } else {
		        EXT4_showFileMetadata(fd, argv[3]);
		    }
			break;
		case GET_FILE_INFO_CODE:
		    if(type == FAT32_FS) {
                FAT32_showFileInfo(fd, argv[3]);
            } else {
                //EXT4_showFileInfo(fd, argv[3]);
		    }
			break;
	}

	close(fd);
    return 0;
	
}
