#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "Helpers.h"

struct movie* createMovie(char* currLine) {
	struct movie* currMovie = malloc(sizeof(struct movie));
	// For use with strtok_r
	char* saveptr;

	// The first token
	char* token = strtok_r(currLine, ",", &saveptr);
	currMovie->Title = calloc(strlen(token) + 1, sizeof(currMovie->Title)); // Needed if Title is a char*
	strcpy(currMovie->Title, token);

	// The next token
	token = strtok_r(NULL, ",", &saveptr);
	//currMovie->Year = calloc(strlen(token) + 1, sizeof(currMovie->Year));
	currMovie->Year = atoi(token);

	// The next token
	token = strtok_r(NULL, ",", &saveptr);
	//currMovie->Languages = calloc(strlen(token) + 1, sizeof(currMovie->Languages));
	char* langPtr;
	char* languageToken = strtok_r(token, ";", &langPtr);
	int i = 0;
	// Loop through each language, from 1 - 5 languages
	while (languageToken != NULL & i < 6) {
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

	// The last token
	token = strtok_r(NULL, ",", &saveptr);
	currMovie->Rating = strtod(token, NULL);

	currMovie->next = NULL;

	return currMovie;
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

void filterChar(char* source, char* destination, char filterChar)
{
	while (*source) { // source != '\0'
		if (*source != filterChar) {
			*destination++ = *source;
		}
		source += 1;
	}
	*destination = '\0'; // Null terminate
}


int validate_input_int(const char* menu, const int lbound, const int ubound) {
	int ret_val;

	int is_valid = scanf("%d", &ret_val);
	// Doesn't catch 3a, only reads 3 and discards a
	while (is_valid == 0 || ret_val < lbound || ret_val > ubound) {
		printf("\nYou entered an incorrect choice. Try again.\n\n");
		printf("%s", menu);
		is_valid = scanf("%d", &ret_val);
		FlushStdin();
	};
	return ret_val;
};

void FlushStdin(void) {
	int ch;
	while (((ch = getchar()) != '\n') && (ch != EOF));
}

//
///*
//* Print data for the given movie. Need to pass in which column, then loop and only show column
//*/
//void printMovie(struct movie* aMovie) {
//	printf("%s\n", aMovie->Year); // This needs to be parsed [xyz;abc;123]
//}
///*
//* Print the linked list of movies
//*/
//void printMovieList(struct movie* list, char* field)
//{
//	while (list != NULL)
//	{
//		printMovie(list);
//		list = list->next;
//	}
//}
//
//
//
//
//
//// Create new linked list that has all movies with same year
//struct movie* moviesByYear(struct movie* list, int year) {
//	//Loop through each movie and add to list if matches year
//	struct movie* head = list;
//	struct movie* tail = list;
//
//	while (tail != NULL) {
//		if (tail->Year == year) {
//			// Get a new movie node corresponding to the current line
//			struct movie* newNode = createCopyMovie(head);
//			// This is not the first node.
//			// Add this node to the list and advance the tail
//			tail->next = newNode;
//			tail = newNode;
//		};
//
//		tail = tail->next;
//	}
//	return head;
//}
//
//struct movie* highestRatingByYear(struct movie* list) {
//	struct movie* maxMovie = list;
//	struct movie* temp = list;
//
//	while (temp != NULL) {
//		if (temp->Rating > maxMovie->Rating) {
//			maxMovie = temp;
//		};
//
//		temp = temp->next;
//	}
//	return maxMovie;
//};
//
//struct movie* createCopyMovie(struct movie* list) {
//	struct movie* currMovie = malloc(sizeof(struct movie));
//
//	// The first token
//	//currMovie->Title = calloc(strlen(list->Title) + 1, sizeof(char));
//	strcpy(currMovie->Rating, list->Year);
//
//	// The next token
//	//currMovie->Year = calloc(strlen(list->Year) + 1, sizeof(char));
//	currMovie->Year = list->Year;
//
//	// The next token
//	//currMovie->Languages = calloc(strlen(list->Languages) + 1, sizeof(char));
//	strcpy(currMovie->Languages, list->Languages);
//
//	// The last token
//	//currMovie->Rating = calloc(strlen(list->Rating) + 1, sizeof(char));
//	currMovie->Rating = list->Rating;
//
//	currMovie->next = NULL;
//
//	return currMovie;
//};
//
//
//char* parseLanguage(char* languages, char* language) {
//
//	// Assumes valid language is passed i.e. "english" not "e"
//	return strstr(languages, language);
//};