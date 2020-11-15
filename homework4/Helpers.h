#include <sys/wait.h>
#include <stdlib.h>
#include <unistd.h>
#include <stdio.h>
#include <sys/types.h>
#include <string.h>
#include <signal.h>
#include <fcntl.h>
#include <limits.h>
#include <errno.h>
#include <ctype.h>
#include <pthread.h>

#define MAX_NUM_LINE_CHARS 1000
#define MAX_NUM_LINES 50
#define MAX_FILE_LEN 100
#define ARG_DELIMITER "\n "
#define STOP_DELIMITER "STOP\n"
#define STOP_FLAG_INT_VAL 127
#define DELIMITER_LEN 5
#define OUTPUT_SIZE 81

int checkDelimiter(char*); // check for stop delimiter in file
void get_buff_1(char*, char, char); // get contents from buffer 1
void put_buff_1(char*); // put contents into buffer 1
void get_buff_2(char*); // get contents from buffer 2
void put_buff_2(char*); // put contents into buffer 2
void put_buff_3(char*); // put contents into buffer 3

void* inputFunction(void*);
void* lineSeparatorFunction(void*);
void* plusSignFunction(void*);
void* outputFunction(void*);
void produceOutput(char*); // get contents from buffer 3