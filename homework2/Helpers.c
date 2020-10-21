#include "Helpers.h"

// Returns 1 if val1 > val2
int maxComparator(const int val1, const int val2) {
	return val1 > val2;
}

// Returns 1 if val2 > val1
int minComparator(const int val1, const int val2) {
	return val2 > val1;
}

// Return name CSV file with desired properties
// Source: https://repl.it/@cs344/35statexamplec#main.c
// Does not recursively search sub-directories
char* getCsvFile(char* path, int init, int(*comparator)(const int p1, const int p2)) {
	DIR* currDir = opendir(path);
	struct dirent* aDir;
	struct stat dirStat;
	char* fileName = calloc(MAX_FILE_NAME_CHARS * sizeof(char), sizeof(char));
	int currSize = init; // Set init val depending on max or min file desired
	int suffixMatches;
	int prefixMatches;

	char p[256];
	while ((aDir = readdir(currDir))) {
		// Match last 4 characters
		suffixMatches = strncmp(SUFFIX, aDir->d_name + strlen(aDir->d_name) - strlen(SUFFIX), strlen(SUFFIX));
		// Match beginning of file name with movies_
		prefixMatches = strncmp(PREFIX, aDir->d_name, strlen(PREFIX));

		// Search for files with matching prefix and suffix
		// Keep track of largest/smallest file size and name
		if (prefixMatches == 0 && suffixMatches == 0) {
			strcpy(p, path);
			stat(strcat(p,aDir->d_name), &dirStat);
			if (comparator(dirStat.st_size, currSize) > 0) {
				currSize = dirStat.st_size;
				strcpy(fileName, aDir->d_name);
			}
			memset(p, '\0', 256);
		}
	}
	free(aDir);
	closedir(currDir);
	return fileName;
}

// Find file in given path with given name, returns found or not
// Source: https://repl.it/@cs344/35statexamplec#main.c
// Does not recursively search sub-directories, expects file extension in name
int findFile(char* path, char* name) {
	DIR* currDir = opendir(path);
	struct dirent* aDir;
	while ((aDir = readdir(currDir))) {
		// Search for files with matching name
		if (strcmp(name, aDir->d_name) == 0) {
			free(currDir);
			return 1;
		}
	}
	free(currDir);
	return 0;
}


// Create directory in current directory named <fileName>.<ONID>.<random_integer> with specified permissions
char* createRandDirinCurr(mode_t mode) {
	char* name = calloc(20, sizeof(char));
	int num = (rand() % (MAX_RAND_INT - MIN_RAND_INT + 1)) + MIN_RAND_INT; // Generate random int
	char buffer[sizeof(int) * 4 + 1];
	sprintf(buffer, "%d", num); // Convert random int to string

	strcat(name, ONID);
	strcat(name, ".movies.");
	strcat(name, buffer);
	if (mkdir(name, mode) == 0) {
		printf("Created directory with name %s\n\n", name);
		return name;
	}
	else {
		printf("Error creating directory.\n");
		free(name);
		return NULL;
	}
}


// Create & return new movie node
struct movie* createMovie(char* currLine) {
	struct movie* currMovie = malloc(sizeof(struct movie));
	// For use with strtok_r
	char* savePtr;

	/*
	Read and tokenize the line. Expect the following format:
	Title<char*>;Year<int>;Languages[language1<char*>;language2<char*>;...language5<char*>];Rating<double>
	*/

	// Process and store the 1st token, which is the movie title.
	char* token = strtok_r(currLine, ",", &savePtr);
	//currMovie->Title = calloc(strlen(token) + 1, sizeof(currMovie->Title)); // Needed if Title is a char*
	currMovie->Title = calloc(MAX_TITLE_LEN, sizeof(currMovie->Title)); // Needed if Title is a char*
	strcpy(currMovie->Title, token);

	// Process and store the next token, which is the movie year. 
	token = strtok_r(NULL, ",", &savePtr);
	currMovie->Year = atoi(token);

	currMovie->next = NULL;

	return currMovie;
};

// Free memory of movies linked list
void freeMovie(struct movie* list) {
	struct movie* temp;
	while (list) {
		temp = list;
		list = list->next;
		free(temp->Title);
		free(temp);
	}
};

// Process csv file. No validation performed (assume correct number of columns, correct format, etc.).
// Use low-level OS calls open, read, write
struct movie* processFile(char* filePath, int* numLines)
{
	int fd;
	size_t nread;

	// The head of the linked list
	struct movie* head = NULL;
	// The tail of the linked list
	struct movie* tail = NULL;

	// Source: https://canvas.oregonstate.edu/courses/1784217/pages/exploration-files?module_item_id=19893088
	if ((fd = open(filePath, O_RDONLY)) == -1) {
		printf("Open() failed on \"%s\"\n", filePath);
		perror("Error");
		close(fd);
		return head;
	}

	// Skip header
	char* readBuffer = calloc(MAX_READ_BUFFER * sizeof(char), sizeof(char));
	int i = 0;
	// Reads as one string, how to stop reading at \n. This is the header. Zero out the buffer. Note string includes \r\n at end. FD points to next char now.
	if ((nread = read(fd, &readBuffer[i], 1))) {
		while ((nread = read(fd, &readBuffer[i], 1))) {
			if (readBuffer[i] == '\n') {
				memset(readBuffer, '\0', MAX_READ_BUFFER * sizeof(char));
				i = 0;
				break;
			}
			++i;
		}
	}
	else {
		printf("Read() failed on \"%s\"\n", filePath);
		perror("Error");
		free(readBuffer);
		close(fd);
		return head;
	}

	// Generate linked list of movie nodes by reading the file character by character. Once a newline character is detected, that is one row of data that we can use to create a node (only title and year data).
	while ((nread = read(fd, &readBuffer[i], 1))) {
		if (readBuffer[i++] == '\n') {
			readBuffer[i - 1] = '\0'; // null out \n
			readBuffer[i - 2] = '\0'; // null out \r
			// Create a new movie node corresponding to the current line
			struct movie* newNode = createMovie(readBuffer);

			// Is this the first node in the linked list?
			if (!head)
			{
				// Set the head and the tail to this new node
				head = newNode;
				tail = newNode;
			}
			else
			{
				// This is not the first node.
				// Add this node to the list and advance the tail.
				tail->next = newNode;
				tail = newNode;
			}
			numLines += 1;
			// Clear out the buffer for reuse
			memset(readBuffer, '\0', MAX_READ_BUFFER * sizeof(char));
			i = 0;
		}
	}
	free(readBuffer);
	close(fd);
	return head;
}

// Used to validate user integer input. Assumes valid integer input.
int validateInputInt(const char* menu, const int lbound, const int ubound) {
	int retVal;
	printf("%s", menu);
	int isValid = scanf("%d", &retVal);
	// Reprompt user until valid integer is entered.
	// Doesn't catch number then character such as 3a, only reads 3 and discards a
	while (isValid == 0 || retVal < lbound || retVal > ubound) {
		printf("\nYou entered an incorrect choice. Try again.\n\n");
		flushStdin();
		printf("%s", menu);
		isValid = scanf("%d", &retVal);
	};
	flushStdin();
	return retVal;
};

// Flush STDIN buffer
// Inspiration here: https://stackoverflow.com/questions/17318886/fflush-is-not-working-in-linux
void flushStdin(void) {
	int ch;
	while (((ch = getchar()) != '\n') && (ch != EOF));
}

// Bubble sort a linked list of movie nodes. This only swaps data, not pointers
// Improvement: Pass a comparator function that allows us to sort ascending, descending, etc.
// Source: https://www.geeksforgeeks.org/c-program-bubble-sort-linked-list/
void sortMovies(struct movie* list, int(*comparator)(const int p1, const int p2)) {
	struct movie* outerNode;
	struct movie* firstSortedNode = NULL;
	int swapped; // Used to break out of while loop once no swaps are needed
	int tempYr;

	if (!list) {
		return;
	}

	char* tempTitle = calloc(MAX_TITLE_LEN * sizeof(outerNode->Title), sizeof(outerNode->Title));

	do {
		outerNode = list;
		swapped = 0;
		while (outerNode->next != firstSortedNode) {
			if (comparator(outerNode->Year, outerNode->next->Year)) {
				swapped = 1;
				strcpy(tempTitle, outerNode->Title);
				strcpy(outerNode->Title, outerNode->next->Title);
				strcpy(outerNode->next->Title, tempTitle);
				tempYr = outerNode->Year;
				outerNode->Year = outerNode->next->Year;
				outerNode->next->Year = tempYr;
			}
			outerNode = outerNode->next;
		}
		firstSortedNode = outerNode; // Used to separate sorted (back) vs unsorted portion
	} while (swapped);
	free(tempTitle);
}

// Creates text files from given path and linked list of movies
void createTxtFiles(char* dirName, struct movie* list) {
	int fd;
	char* fName = calloc(strlen(dirName) + 12, sizeof(char)); //Used to store full path name of text file
	char yearBuffer[4] = { 0, };
	char titleBuffer[MAX_FILE_NAME_CHARS];
	struct movie* temp = list;

	while (temp) {
		// Open newly created directory and create/append to text file
		strcpy(fName, "./");
		strcat(fName, dirName);
		strcat(fName, "/");
		sprintf(yearBuffer, "%d", temp->Year);
		strcat(fName, yearBuffer);
		strcat(fName, ".txt");
		fd = open(fName, O_WRONLY | O_CREAT | O_APPEND, 0640);

		// Loop through csv file and generate a file for each unique year.
		// Within the file for a year, write the titles of all the movies released in that year, one on each line
		memset(titleBuffer, '\0', sizeof(titleBuffer));
		strcpy(titleBuffer, temp->Title);
		strcat(titleBuffer, "\n");
		write(fd, titleBuffer, strlen(temp->Title) + 1);
		// Check if same year
		while (temp->next && temp->Year == temp->next->Year) {
			memset(titleBuffer, '\0', sizeof(titleBuffer));
			strcpy(titleBuffer, temp->next->Title);
			strcat(titleBuffer, "\n");
			write(fd, titleBuffer, strlen(temp->next->Title) + 1);
			temp = temp->next;
		}

		temp = temp->next;
		memset(titleBuffer, '\0', sizeof(titleBuffer));
		memset(fName, '\0', strlen(fName));
		memset(yearBuffer, '\0', sizeof(yearBuffer));
		close(fd);
	}
	free(fName);
	freeMovie(list);
	free(dirName);
	close(fd);
}

// Print movies
void printMovie(struct movie* list) {
	struct movie* temp = list;
	while (temp) {
		printf("%d\t%s\n", temp->Year, temp->Title);
		temp = temp->next;
	}
}