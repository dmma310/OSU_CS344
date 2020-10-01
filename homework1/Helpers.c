#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "Helpers.h"

struct movie* createMovie(char* currLine) {
	struct movie* currMovie = malloc(sizeof(struct movie));
	currMovie->numLanguages = 0;
	// For use with strtok_r
	char* savePtr;

	// The first token
	char* token = strtok_r(currLine, ",", &savePtr);
	currMovie->Title = calloc(strlen(token) + 1, sizeof(currMovie->Title)); // Needed if Title is a char*
	strcpy(currMovie->Title, token);

	// The next token
	token = strtok_r(NULL, ",", &savePtr);
	//currMovie->Year = calloc(strlen(token) + 1, sizeof(currMovie->Year));
	currMovie->Year = atoi(token);

	// The next token
	token = strtok_r(NULL, ",", &savePtr);
	//currMovie->Languages = calloc(strlen(token) + 1, sizeof(currMovie->Languages));
	char* langPtr;
	char* languageToken = strtok_r(token, ";", &langPtr);
	int i = 0;
	// Loop through each language, from 1 - 5 languages
	while (languageToken != NULL && i < MAX_LANGUAGES) {
		currMovie->Languages[i] = calloc(strlen(languageToken) + 1, sizeof(currMovie->Languages[i]));
		int s = strlen(languageToken) - 1;
		// If one language, format is [some_language]
		if (languageToken[0] == '[' && languageToken[s] == ']') {
			// Copy from 2nd character on. Also don't copy last character
			memcpy(currMovie->Languages[i], &languageToken[1], s - 1);
			break;
		}
		// 1st language
		else if (languageToken[0] == '[') {
			filterChar(languageToken, currMovie->Languages[i], '[');
		}
		// Last language
		else if (languageToken[s] == ']') {
			filterChar(languageToken, currMovie->Languages[i], ']');
		}
		else {
			strcpy(currMovie->Languages[i], languageToken);
		}
		languageToken = strtok_r(NULL, ";", &langPtr);
		i += 1;
	}
	currMovie->numLanguages = i;

	// The last token
	token = strtok_r(NULL, ",", &savePtr);
	currMovie->Rating = strtod(token, NULL);

	currMovie->next = NULL;


	return currMovie;
};

void freeChars(struct movie* m) {
	for (int i = 0; i < m->numLanguages; ++i) {
		free(m->Languages[i]);
	}
}

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

void filterChar(char* source, char* destination, char comparator)
{
	while (*source) { // source != '\0'
		if (*source != comparator) {
			*destination++ = *source;
		}
		source += 1;
	}
	*destination = '\0'; // Null terminate
}


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

void flushStdin(void) {
	int ch;
	while (((ch = getchar()) != '\n') && (ch != EOF));
}

struct keysValue* createKeysValueList(struct movie* list) {
	struct keysValue* head = NULL;
	struct keysValue* tail = NULL;
	struct movie* movieList = list;
	int exists;

	while (movieList != NULL) {
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
			// Create new node since current year does not exist
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

struct keysValue* createKeysValue(struct movie* list) {
	struct keysValue* temp = malloc(sizeof(struct keysValue));
	temp->title = list->Title;
	temp->yearRating.rating = list->Rating;
	temp->yearRating.year = list->Year;
	temp->next = NULL;
	return temp;
}

void printKeysValue(struct keysValue* list) {
	struct KeysValue* head = list;
	while (list) {
		printf("%d %0.1f %s\n", list->yearRating.year, list->yearRating.rating, list->title);
		list = list->next;
	}
	list = head;
}

void freeKeysValue(struct keysValue* list) {
	struct keysValue* temp;
	while ((temp = list) != NULL) {
		list = list->next;
		free(temp);
	}
	list = NULL;
};