#include "Helpers.h"
static int isbg;
static int bgEnabled = 1;
static int prevStatus = 0;
static int isInputRedirect;
static char inputFile[MAX_ARG_LEN];
static char outputFile[MAX_ARG_LEN];
static int isOutputRedirect;
static int isBuiltin;
static char prevDir[MAX_ARG_LEN] = { 0, };
struct processInfo* head = NULL;
pid_t fgPid = VOID_VAL;

/*
Used to determine type of command (builtin/non-builtin), redirection and file info, expansion of PID, and background command.
Flags set and args array updated and prepared for execvp or built in command execution.
*/
void handleFlags(char** args, int* numWords) {
	// Initialie variables
	isBuiltin = (strcmp(args[0], "cd") == 0 || strcmp(args[0], "exit") == 0 || strcmp(args[0], "status") == 0);
	int pidIndex;
	isInputRedirect = 0;
	isOutputRedirect = 0;
	memset(inputFile, '\0', strlen(inputFile));
	memset(outputFile, '\0', strlen(outputFile));

	// Loop through each word
	for (int i = 1; i < *numWords; ++i) {
		// Figure out if input/output redirection, and store file name(s) if eneded
		if (strcmp(args[i], "<") == 0 && !isBuiltin) {
			isInputRedirect = 1;
			strcpy(inputFile, args[i + 1]);
		}
		if (strcmp(args[i], ">") == 0 && !isBuiltin) {
			isOutputRedirect = 1;
			strcpy(outputFile, args[i + 1]);
		}
		// Find and expand all instances of "$$" to process id, update args array
		if ((pidIndex = findDuplicateChar(args[i], '$')) > -1) {
			char* temp = expandPIDVar(args[i], pidIndex);
			free(args[i]);
			args[i] = strdup(temp);
			free(temp);
			temp = NULL;
		}
	}
	// Figure out if this is a background command
	if (strcmp(args[*numWords - 1], "&") == 0) {
		isbg = 1;
		free(args[*numWords - 1]);
		args[*numWords - 1] = NULL;
	}
	else {
		isbg = 0;
	}
	// Update args array to strip out redirection characters if needed
	if (isInputRedirect) {
		stripRedirectChars(args, "<");
	}
	if (isOutputRedirect) {
		stripRedirectChars(args, ">");
	}
}

/************************************************************************************************
********/
/*
Wait for and reap all bg children by looping through list of bg children, then exit shell
*/
int exitShell(char** argsArray) {
	struct processInfo* temp = head;
	struct processInfo* prev;
	int status;
	//while (temp) {
	//	waitpid(temp->pid, &status, 0);
	//	prev = temp;
	//	temp = temp->next != NULL ? temp->next : NULL;
	//	removeProcess(prev->pid, &head);
	//}
	freeParsedArgs(argsArray);
	freeProcessList(head);
	//head = NULL;
	exit(EXIT_SUCCESS);
}
/*
* The status command prints out either the exit status or the terminating signal of the last foreground process ran by the shell. If this command is run before any foreground command is run, then it simply returns the exit status 0.
  Status ignores built-in commands.
*/
int statusShell() {
	// Ignore built in commands & background processes (does not parse &), check if this is the first command
	if (!(prevStatus == VOID_VAL)) {
		if (WIFEXITED(prevStatus)) {
			// if terminated normally, get the exit status via macro
			printf("exit value %d\n", (int)WEXITSTATUS(prevStatus));
			fflush(stdout);
		}
		else if (WIFSIGNALED(prevStatus)) {
			// if terminated by a signal, get the exit signal via macro
			printf("terminated by signal %d\n", (int)WTERMSIG(prevStatus));
			fflush(stdout);
		}
	}
	return EXIT_SUCCESS;
}

/*
The cd command changes the working directory of smallsh.
By itself - with no arguments - it changes to the directory specified in the HOME environment variable.
This command can also take one argument: the path of a directory to change to. It supports both absolute and relative paths.
Returns 0 on success, -1 on error
*/
int cdShell(char** argsArray) {
	if (!argsArray[1] || strcmp(argsArray[1], "~") == 0) {
		// should go home
		getcwd(prevDir, sizeof(prevDir));
		const char* home = getenv("HOME");
		return chdir(home);
	}
	// Previous directory
	else if (strcmp(argsArray[1], "-") == 0) {
		char cwd[MAX_ARG_LEN];
		strcpy(cwd, prevDir);
		getcwd(prevDir, sizeof(prevDir));
		return chdir(cwd);
	}
	else {
		getcwd(prevDir, sizeof(prevDir));
		return chdir(argsArray[1]);
	}
}

/********************************************************************************************************/
// Redirects IO to appropriate file, and captures success/failure status
int handleRedirection(char** args) {
	int statusIn, statusOut;
	// Handle redirection
	if (isInputRedirect) {
		if (isbg && bgEnabled) {
			statusIn = redirectFd(0, "/dev/null", O_RDONLY, 0);
		}
		else {
			statusIn = redirectFd(0, inputFile, O_RDONLY, 0);
		}
	}
	if (isOutputRedirect) {
		if (isbg && bgEnabled) {
			statusOut = redirectFd(1, "/dev/null", O_WRONLY, 0);
		}
		else {
			statusOut = redirectFd(1, outputFile, O_WRONLY | O_CREAT | O_TRUNC, 0644);
		}
	}
	return statusIn || statusOut;
}

/************************************************************************************************
********/
// Redirects specified to appropriate file, and captures success/failure status
int redirectFd(int currFd, char* fileName, int flags, mode_t mode) {
	// open a new file descriptor
	int newFd = open(fileName, flags, mode);
	// make sure  open()  was successful
	if (newFd == -1) {
		if (currFd == 0) {
			fprintf(stderr, "cannot open %s for input\n", fileName);
			fflush(stdout);
		}
		else if (currFd == 1) {
			fprintf(stderr, "cannot open %s for output\n", fileName);
			fflush(stdout);
		}
		prevStatus = EXIT_FAILURE;
		return prevStatus;
	}

	// Redirect and make sure dup2() was successful
	if (dup2(newFd, currFd) < 0) {
		fprintf(stderr, "dup2() failure\n");
		prevStatus = EXIT_FAILURE;
		return prevStatus;
	}

	// close on exec()
	fcntl(newFd, F_SETFD, FD_CLOEXEC);
	return 0;
}


/********************************************************************************************************/
/*
Waits and reaps any background child if child is done
While loop solves race condition (if all children finish at once, this function is just called once. Need to reap for all children).
 */
void reapBgChild(int sig) {
	while(1) {
		int pidStatus = VOID_VAL;
		pid_t pid = waitpid(-1, &pidStatus, WNOHANG);
		if (pid <= 0) return;
		// Foreground child handled.
		if (pid == fgPid) {
			prevStatus = pidStatus;
			fgPid = VOID_VAL; // Reset fgPid to break out of infinite loop in main
			return;
		}
		// Background child handled. Remove from bg child list if exists, set status, print, and return;
		if (removeProcess(pid, &head)) {
			// No need to capture status of background commands
			printf("\nbackground pid %d is done: ", (int)pid);
			prevStatus = pidStatus;
			statusShell();
			if (WIFEXITED(pidStatus)) write(STDOUT_FILENO, ": ", 2); // new prompt
			fflush(stdout);
		}
	}
}

// Used by foreground child
void sigtstpHandler(int sig) {
	if (fgPid != VOID_VAL) {
		waitpid(fgPid, &prevStatus, 0); // Wait for foreground process to finish
	}
	if (bgEnabled) {
		write(STDOUT_FILENO, "\nEntering foreground - only mode (&is now ignored)\n", 50);
		fflush(stdout);
	}
	else {
		write(STDOUT_FILENO, "\nExiting foreground - only mode (&is now ignored)\n", 49);
		fflush(stdout);
	}

	if (fgPid == VOID_VAL) {
		write(STDOUT_FILENO, "\n: ", 3); // new prompt
		fflush(stdout);
	}
	toggleBit(&bgEnabled);
	fflush(stdout);
	return;
}

// Handle sigint
void sigintHandler(int sig) {
	if (fgPid != VOID_VAL) {
		waitpid(fgPid, &prevStatus, 0); // Wait for foreground process to finish
		printf("terminated by signal %d\n", sig);
		fflush(stdout);
		prevStatus = sig;
	}
	return;
}

/************************************************************************************************
********/

int main() {
	// signal handler registration for SIGINT, SIGTSTP, SIGCHLD
	struct sigaction sigintAction = { {0} };   // deal with SIGINT (i.e Ctrl-C)
	sigintAction.sa_handler = sigintHandler;   // use this handler when caught
	sigfillset(&sigintAction.sa_mask);       // block all other signal types
	sigintAction.sa_flags = SA_RESTART;      // restart after signal is captured
	sigaction(SIGINT, &sigintAction, NULL);  // register the struct

	struct sigaction sigtstpAction = { {0} }; // deal with SIGTSTP (i.e Ctrl-Z)
	sigtstpAction.sa_handler = sigtstpHandler;
	sigfillset(&sigtstpAction.sa_mask);
	sigtstpAction.sa_flags = SA_RESTART;
	sigaction(SIGTSTP, &sigtstpAction, NULL);

	struct sigaction sigchldAction = { {0} };
	sigchldAction.sa_handler = reapBgChild;
	sigfillset(&sigchldAction.sa_mask);
	sigchldAction.sa_flags = SA_RESTART;
	sigaction(SIGCHLD, &sigchldAction, NULL);

	const char* CLEAR_SCREEN_ANSI = " \e[1;1H\e[2J";
	write(STDOUT_FILENO, CLEAR_SCREEN_ANSI, 12);

	while (1) {
		// Read and get input, trim any leading/trailing whitespaces
		printf(": ");
		fflush(stdout);
		char args[MAX_ARG_LEN];
		int numWords = 0;
		fgets(args, sizeof(args), stdin);
		trim(args);

		// Check for comment or blank line and ignore. Need to trim leding/trailing whitespaces too.
		if (args[0] == '#' || args[0] == '\n') {
			fflush(stdout);
			continue;
		}

		// Parse arguments into array
		char** argsArray = tokenizeArgs(args, &numWords); // separate each arg into element in array, set cmd variable to 1st string in command line, which is the cmd to run

		// Set flags and update args for redirection, background, expand $$ 
		handleFlags(argsArray, &numWords);

		// Handle non-built-in, should track prevStatus
		if (!isBuiltin) {
			pid_t pid = fork();
			fgPid = (isbg && bgEnabled) ? VOID_VAL : pid; // Update fgPid value if fg process
			if (pid == -1) {
				freeParsedArgs(argsArray);
				freeProcessList(head);
				perror("fork() failure\n");
				exit(EXIT_FAILURE);
			}
			else if (pid == 0) {
				// Check and set redirection. File descriptors do not inherit across forks, so this must be done in child
				if (handleRedirection(argsArray) == 1) {
					if (!isbg) prevStatus = EXIT_FAILURE;
					exit(prevStatus);
				};

				// Set signal handlers
				// Children will ignore SIGCHLD
				struct sigaction sigchldIgnore = { {0} };
				sigchldIgnore.sa_handler = SIG_IGN;
				sigaction(SIGCHLD, &sigchldIgnore, NULL);

				/*
				HANDLE SIGTSTP
				Any child (background/foreground) process must ignore SIGTSTP.
				The parent shell displays a message immediately if it's sitting at the prompt or immediately after any currently running foreground process has terminated.
				The parent then ignores attempts to run a background process (ignore & operator).
				If the user sends SIGTSTP again, the shell displays another message immediately after any currently running foreground process terminates.
				The shell then returns back to the normal condition where the & operator is once again honored.
				*/
				struct sigaction sigtstpIgnore = { {0} };
				sigtstpIgnore.sa_handler = SIG_IGN;
				sigaction(SIGTSTP, &sigtstpIgnore, NULL);
				/*
				HANDLE SIGINT
				The parent and any children running as background processes ignores SIGINT
				A child running as a foreground process terminates itself when it receives SIGINT
				If a child foreground process is killed by a signal, the parent must immediately print out the number of the signal that killed it's foreground child process before prompting the user for the next command.
				*/
				if (isbg && bgEnabled) {
					struct sigaction sigintHandler = { {0} };
					sigintHandler.sa_handler = SIG_IGN;
					sigaction(SIGINT, &sigintHandler, NULL);
				}
				else {
					struct sigaction sigintHandler = { {0} };
					sigintHandler.sa_handler = SIG_DFL;
					sigaction(SIGINT, &sigintHandler, NULL);
				}

				// Execute
				execvp(argsArray[0], argsArray);
				// If error
				printf("%s :no such file or directory\n", argsArray[0]);
				fflush(stdout);
				// No need to capture status of background commands
				if (!isbg) prevStatus = EXIT_FAILURE;
				exit(prevStatus);
			}
			// In parent
			else {
				fflush(stdout);
				// If background process and enabled, add to list and print message
				if (isbg && bgEnabled) {
					head = appendProcess(head, pid);
					printf("background pid is %d\n", (int)pid);
					fflush(stdout);
				} else{
					/*
					Set fgPid and loop and wait until child returns.
					*/
					fgPid = pid;
					int testPid = waitpid(fgPid, &prevStatus, 0);
					fgPid = VOID_VAL;
				}
			}
		}
		// Built-in commands
		else {
			if (isbg) {
				argsArray[--numWords] = NULL; // Overwrite "&" in args
			}
			prevStatus = handleBuiltin(argsArray); // Execute built-in command and return status
		}
		freeParsedArgs(argsArray);
	}
	freeProcessList(head);
	return 0;
}