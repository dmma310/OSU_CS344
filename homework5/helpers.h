#include <unistd.h>
#include <string.h>
#include <sys/types.h>  // ssize_t
#include <sys/socket.h> // send(),recv()
#include <netdb.h>      // gethostbyname()
#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>
#include <sys/types.h> 
#include <sys/wait.h>
#include <sys/socket.h>
#include <netinet/in.h>

#define FULL_MSG_SIZE 140010
#define TMP_BUFF_SIZE 140010
#define SVR_ALLOW_MSG "ok"
#define SVR_BAD_PORT_MSG "@@"
#define MAX_SERVER_CONNECTIONS 5
#define FILE_DELIMITER "\n"
#define ENC_CLI_CONFIRM_MSG "enc_client"
#define DEC_CLI_CONFIRM_MSG "dec_client"
#define ENC_SVR "enc_server"
#define DEC_SVR "dec_server"

void setupAddressStruct(struct sockaddr_in*, int, char*);
int get_file(char*, const char*);
int sendall(int, char*, int*);
int readall(int, char*, int);

int char_to_int(char);
char int_to_char(int);
void encrypt(char*, char*, char*);
void decrypt(char*, char*, char*);