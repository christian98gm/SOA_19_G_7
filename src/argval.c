#include "argval.h"

/**
 * FUNCTIONS
 **/

int ARGVAL_validate(int argc, char * argv[]) {
	
	switch(argc) {
		
		case 3:
			if(strcmp(argv[1], GET_INFO)) {
				VIEW_undefinedOperation(argv[1]);
				return NO_OP_CODE;
			}
			return GET_INFO_CODE;
			
		case 4:
			if(!strcmp(argv[1], GET_FILE_METADATA)) {
				return GET_FILE_METADATA_CODE;
			} else if(!strcmp(argv[1], GET_FILE_INFO)) {
				return GET_FILE_INFO_CODE;
			} else {
				VIEW_undefinedOperation(argv[1]);
				return NO_OP_CODE;
			}
			
		default:
		    VIEW_wrongFormat();
			return NO_OP_CODE;
			
	}
	
}
