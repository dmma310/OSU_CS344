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
#define MAX_NUM_ARGS 512
#define MAX_ARG_LEN 100
#define ARG_DELIMITER "\n "
#define MAX_CMD_LINE_LENGTH 2048
#define VOID_VAL (int)INT_MIN

// Holds process info
struct processInfo {
	pid_t pid;
	struct processInfo* next;
};

int exitShell(char**); // Wait forand reap all bg children by looping through list of bg children, then exit shell
/*
* The status command prints out either the exit status or the terminating signal of the last foreground process ran by the shell. If this command is run before any foreground command is run, then it simply returns the exit status 0.
  Status ignores built-in commands.
*/
int statusShell();
/*
The cd command changes the working directory of smallsh.
By itself - with no arguments - it changes to the directory specified in the HOME environment variable.
This command can also take one argument: the path of a directory to change to. It supports both absolute and relative paths.
Returns 0 on success, -1 on error
*/
int cdShell(char**);

int handleRedirection(char**); // Redirects IO to appropriate file, and captures success/failure status
/*
Used to determine type of command (builtin/non-builtin), redirection and file info, expansion of PID, and background command.
Flags set and args array updated and prepared for execvp or built in command execution.
*/
void handleFlags(char**, int*);

int redirectFd(int, char*, int, mode_t); // Redirects specified to appropriate file, and captures success/failure status


int getArrayElemIndex(char**, char*); // Get index of char in array
void stripRedirectChars(char**, char*); // Update args array by replacing "< fileName" or "> fileName"
void toggleBit(int*); // Toggles bit
char* expandPIDVar(char*, int); // Get all occurences of $$ and replace with pid
// Find and return location of 1st instance of $
int findDuplicateChar(char*, char); // Find and return location of 1st instance of $
char** tokenizeArgs(char* args, int* numWords); // Parse args into separate elements of an array
int handleBuiltin(char**); // Determine which built-in command to run

struct processInfo* createProcess(pid_t pid); // Create linked list node to track background process id
struct processInfo* appendProcess(struct processInfo* head, pid_t pid); // Add a new process node to linked list. If does not exist, create and return one.

int removeProcess(pid_t, struct processInfo**); // Remove linked list node
void freeProcessList(struct processInfo*); // Free linked list
void freeParsedArgs(char**); // Free arguments array
void trim(char*); // Trim leading and trailing whitespace. String must be modifiable.