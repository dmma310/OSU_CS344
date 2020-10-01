#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "Helpers.h"

// Create a movie node and return a pointer to it.
struct movie* createMovie(char* currLine) {
	struct movie* currMovie = malloc(sizeof(struct movie));
	currMovie->numLanguages = 0;
	// For use with strtok_r
	char* savePtr;

	// The first token read from the csv file
	char* token = strtok_r(currLine, ",", &savePtr);
	currMovie->Title = calloc(strlen(token) + 1, sizeof(currMovie->Title)); // Needed if Title is a char*
	strcpy(currMovie->Title, token);

	// The next token read from the csv file
	token = strtok_r(NULL, ",", &savePtr);
	currMovie->Year = atoi(token);

	// The next token read from the csv file
	token = strtok_r(NULL, ",", &savePtr);

	// Process the languages
	// Expect languages to be this format: [language1;language2;...language5]
	char* langPtr;
	char* languageToken = strtok_r(token, ";", &langPtr);
	int i = 0;

	// Loop through each language and dynamically allocate and copy to an array of char pointers
	while (languageToken != NULL && i < MAX_LANGUAGES) {
		currMovie->Languages[i] = calloc(strlen(languageToken) + 1, sizeof(currMovie->Languages[i]));
		int s = strlen(languageToken) - 1;

		// Handle the following cases:
		// If one language, format is [some_language]
		if (languageToken[0] == '[' && languageToken[s] == ']') {
			// Copy from 2nd character on. Also don't copy last character
			memcpy(currMovie->Languages[i], &languageToken[1], s - 1);
			break;
		}
		// If it's the first language we are processing
		else if (languageToken[0] == '[') {
			filterChar(languageToken, currMovie->Languages[i], '[');
		}
		// If it's the last language we are processing
		else if (languageToken[s] == ']') {
			filterChar(languageToken, currMovie->Languages[i], ']');
		}
		// All other cases
		else {
			strcpy(currMovie->Languages[i], languageToken);
		}
		languageToken = strtok_r(NULL, ";", &langPtr);
		i += 1;
	}
	currMovie->numLanguages = i;

	// The last token read from the csv file
	token = strtok_r(NULL, ",", &savePtr);
	currMovie->Rating = strtod(token, NULL);

	currMovie->next = NULL;


	return currMovie;
};

// Frees the array of character pointers (languages) in the movie structure
void freeChars(struct movie* m) {
	for (int i = 0; i < m->numLanguages; ++i) {
		free(m->Languages[i]);
	}
}

// Frees the linked list of movie nodes
void freeMovie(struct movie* list) {
	struct movie* temp;
	while ((temp = list) != NULL) {
		list = list->next;
		// Free languages
		freeChars(temp);
		free(temp);
	}
	list = NULL;
};

// Process csv file and stores each record in a movie node.
// No validation is performed (i.e. valid format, number of fields, etc.)
// Expect file to be Title, Year, Languages, Rating Value
struct movie* processFile(char* filePath, int* numLines)
{
	// Open the specified file for reading only
	FILE* movieFile = fopen(filePath, "r");

	char* currLine = NULL;
	size_t len = 0;
	size_t nread;

	// The head of the linked list
	struct movie* head = NULL;
	// The tail of the linked list
	struct movie* tail = NULL;

	// Ignore 1st row (header)
	len = 256;
	if ((nread = getline(&currLine, &len, movieFile)) == -1) {
		printf("Empty file\n");
		return head;
	}
	// Read the file line by line
	while ((nread = getline(&currLine, &len, movieFile)) != -1)
	{
		// Get a new movie node corresponding to the current line
		struct movie* newNode = createMovie(currLine);

		// Is this the first node in the linked list?
		if (head == NULL)
		{
			// This is the first node in the linked link
			// Set the head and the tail to this node
			head = newNode;
			tail = newNode;
		}
		else
		{
			// This is not the first node.
			// Add this node to the list and advance the tail
			tail->next = newNode;
			tail = newNode;
		}
		*numLines += 1;
	}
	free(currLine);
	fclose(movieFile);
	return head;
}

// Copy all characters from source to destination except comparator
void filterChar(char* source, char* destination, char comparator)
{
	while (*source) { // source != '\0'
		// Increment destination to next char in array only if source does not equal comparator, else skip by incrementing source
		if (*source != comparator) {
			*destination++ = *source;
		}
		source += 1;
	}
	*destination = '\0'; // Null terminate
}

// Validates user integer input bounds. Assumes valid integer input.
int validateInputInt(const char* menu, const int lbound, const int ubound) {
	int retVal;
	printf("%s", menu);
	int isValid = scanf("%d", &retVal);
	// Doesn't catch number then character such as 3a, only reads 3 and discards a
	while (isValid == 0 || retVal < lbound || retVal > ubound) {
		printf("\nYou entered an incorrect choice. Try again.\n\n");
		printf("%s", menu);
		isValid = scanf("%d", &retVal);
		flushStdin();
	};
	return retVal;
};

// Flush stdin buffer
void flushStdin(void) {
	int ch;
	while (((ch = getchar()) != '\n') && (ch != EOF));
}

// Creates a linked list of <year, <title, rating>> nodes from a list of movie nodes. All years are unique with the highest rating stored for that year.
struct keysValue* createKeysValueList(struct movie* list) {
	struct keysValue* head = NULL;
	struct keysValue* tail = NULL;
	struct movie* movieList = list;
	int exists;

	// Loop through each movie node and create and add <year, <title, rating>> node to list if unique.
	// Else swap info for existing node if new rating is larger.
	while (movieList != NULL) {
		// Create first node in list
		if (head == NULL) {
			struct keysValue* newNode = createKeysValue(movieList);
			head = newNode;
			tail = newNode;
		}
		else {
			exists = 0;
			// Try to find matching/existing movie year, and replace with highest rating if needed
			while (tail->next) {
				// If node with existing year exists, set highest rating
				if ((movieList->Year == tail->yearRating.year)) {
					if (movieList->Rating > tail->yearRating.rating) {
						tail->title = movieList->Title;
						tail->yearRating.rating = movieList->Rating;
					}
					exists = 1;
					break;
				}
				tail = tail->next;
			}
			// Create new node and add to list since current year does not exist
			if (!exists) {
				struct keysValue* newNode = createKeysValue(movieList);
				tail->next = newNode;
			}
			tail = head; // rest tail to the beginning
		}
		movieList = movieList->next;
	}
	return head;
}

// Creates a <year, <title, rating>> struct from movie node
struct keysValue* createKeysValue(struct movie* list) {
	struct keysValue* temp = malloc(sizeof(struct keysValue));
	temp->title = list->Title;
	temp->yearRating.rating = list->Rating;
	temp->yearRating.year = list->Year;
	temp->next = NULL;
	return temp;
}

// Display title and highest rating for each year
void printKeysValue(struct keysValue* list) {
	struct KeysValue* head = list;
	while (list) {
		printf("%d %0.1f %s\n", list->yearRating.year, list->yearRating.rating, list->title);
		list = list->next;
	}
	list = head;
}

// Frees the linked list of nodes containing <year, <title, rating>>
void freeKeysValue(struct keysValue* list) {
	struct keysValue* temp;
	while ((temp = list) != NULL) {
		list = list->next;
		free(temp);
	}
	list = NULL;
};