#include <fcntl.h>

#include "argval.h"
#include "view.h"
#include "identifier.h"

#define FILES_PATH "./files/"
#define MAX_PATH 64

int main(int argc, char * argv[]) {
	
	//Check args
	int operation = ARGVAL_validate(argc, argv);
    if(!operation) {
		return 1;
	}

    //chdir("/Users/jordana/Desktop/SOA_19_G_7/files");

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
			//TODO: Get file metadata
			break;
		case GET_FILE_INFO_CODE:
			//TODO: Get file data
			break;
	}

	close(fd);
    return 0;
	
}
