#include "Helpers.h"

// Get index of char in array
int getArrayElemIndex(char** argsArray, char* element) {
	int i = 0;
	while (strcmp(argsArray[i++], element) != 0);
	return i - 1;
}

// Update args array by replacing "< fileName" or "> fileName"
void stripRedirectChars(char** argsArray, char* c) {
	int index = getArrayElemIndex(argsArray, c);
	while (argsArray[index + 2]) {
		argsArray[index] = realloc(argsArray[index], strlen(argsArray[index + 2]));
		strcpy(argsArray[index], argsArray[index + 2]);
		index += 1;
	};

	free(argsArray[index]);
	argsArray[index] = NULL;
	free(argsArray[index + 1]);
	argsArray[index + 1] = NULL;
}

// Toggles bit
void toggleBit(int* bit) {
	*bit = *bit ^ 1;
}

// Source: https://stackoverflow.com/questions/53311499/c-replace-in-string-with-id-from-getpid
// Get all occurences of $$ and replace with pid
char* expandPIDVar(char* str, int index) {
	/* Modify the existing string to be a pattern string for snprintf */
	char* formatString = strdup(str); // Copy string
	int newLen = strlen(str) + 11;
	char* pidString = calloc(newLen, sizeof(char));

	// Replace "$$" with "%d"
	formatString[index] = '%';
	formatString[index + 1] = 'd';
	snprintf(pidString, newLen - 1, formatString, (int)getpid()); // Replace "%d" with pid
	free(formatString);
	formatString = NULL;

	// Find all other occurences
	int idx;
	while ((idx = findDuplicateChar(pidString, '$')) > -1) {
		formatString = strdup(pidString);
		newLen = strlen(pidString) + 11;
		pidString = realloc(pidString, newLen);
		formatString[idx] = '%';
		formatString[idx + 1] = 'd';
		snprintf(pidString, newLen - 1, formatString, (int)getpid());
		free(formatString);
		formatString = NULL;
	}
	return pidString;
}

// Find and return location of 1st instance of $
int findDuplicateChar(char* str, char searchChar) {
	for (int i = 0; i < strlen(str) - 1; ++i) {
		if (str[i] == searchChar && str[i + 1] == searchChar) {
			return i;
		}
	}
	return -1;
}

// Parse args into separate elements of an array
char** tokenizeArgs(char* args, int* numWords) {
	char** argsArray = calloc(MAX_NUM_ARGS, sizeof(char*)); // Create array of NULL elements
	char* rest;
	// First element is the command
	char* token = strtok_r(args, ARG_DELIMITER, &rest);
	argsArray[*numWords] = calloc(strlen(token) + 1, sizeof(char));
	strcpy(argsArray[(*numWords)++], token);

	// Get each arg as an element into the args array.
	while ((token = strtok_r(NULL, ARG_DELIMITER, &rest))) {
		argsArray[*numWords] = calloc(strlen(token) + 1, sizeof(char));
		strcpy(argsArray[(*numWords)++], token);
	}

	return argsArray;
}

// Trim leading and trailing whitespace. String must be modifiable.
// Source: https://stackoverflow.com/questions/122616/how-do-i-trim-leading-trailing-whitespace-in-a-standard-way
void trim(char* str) {
	char* p = str;
	int l = strlen(p);
	int i = 0;
	if (l == 0) return;

	while (isspace(p[l - 1])) p[--l] = 0;
	while (*p && (p[i++] == '\0' || isspace(*p)) && i <= l) ++p, --l;

	if (i == l) {
		memmove(str, "\n\0", 2);
	}
	else {
		memmove(str, p, l + 1);
	}
}

/************************************************************************************************
********/

// Determine which built-in command to run
int handleBuiltin(char** argsArray) {
	if (strcmp(argsArray[0], "cd") == 0) {
		return cdShell(argsArray);
	}
	else if (strcmp(argsArray[0], "exit") == 0) {
		return exitShell(argsArray);
	}
	else if (strcmp(argsArray[0], "status") == 0) {
		return statusShell();
	}
	else {
		return 0;
	}
}

/********************************************************************************************************/
// Create linked list node to track background process id
struct processInfo* createProcess(pid_t pid) {
	struct processInfo* pInfo = malloc(sizeof(struct processInfo));
	pInfo->pid = pid;
	pInfo->next = NULL;
	return pInfo;
}

// Add a new process node to linked list. If does not exist, create and return one.
struct processInfo* appendProcess(struct processInfo* head, pid_t pid)
{
	struct processInfo* temp = createProcess(pid);
	if (!head) {
		temp->next = NULL;
		return temp;
	}
	struct processInfo* iter = head;
	while (iter->next) {
		iter = iter->next;
	}
	iter->next = temp;
	return head;
}

// Source: https://www.geeksforgeeks.org/linked-list-set-3-deleting-node/
// Remove linked list node
int removeProcess(pid_t pid, struct processInfo** head) {
	// Store head node 
	struct processInfo* temp = *head;
	struct processInfo* prev = NULL;

	// If head node itself is the node to be deleted 
	if (temp != NULL && temp->pid == pid)
	{
		*head = temp->next; // Change head 
		free(temp);
		return 1;
	}

	// Search for the key to be deleted and keep track of the previous node as we 
	while (temp != NULL && temp->pid != pid)
	{
		prev = temp;
		temp = temp->next;
	}

	// If we couldn't find the process 
	if (temp == NULL)
		return 0;

	// Update pointers and free memory 
	prev->next = temp->next;
	free(temp);
	return 1;
}

// Free linked list
void freeProcessList(struct processInfo* head) {
	if (head) {
		struct processInfo* currNode, * nextNode;
		currNode = head;
		head->next = NULL;
		while (currNode) {
			nextNode = currNode->next;
			free(currNode);
			currNode = nextNode;
		}
	}
}

// Free arguments array
void freeParsedArgs(char** args) {
	for (int i = 0; i < MAX_NUM_ARGS; ++i) {
		free(args[i]);
	}
	free(args);
	args = NULL;
}

