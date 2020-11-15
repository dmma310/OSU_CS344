#include "Helpers.h"

/********************************************************************************************************/
/*
* Checks whether we reached the stop delimiter, set the global stop_flag variable
*/
int check_delimiter(char* buffer) {
	// Check for STOP delimiter and set stop flag
	if (strlen(buffer) == DELIMITER_LEN) {
		if (strcmp(buffer, STOP_DELIMITER) == 0) {
			return STOP_FLAG_INT_VAL;
		}
	}
	return 0;
}