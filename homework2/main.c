#include "Helpers.h"
#define MAX_FILE_NAME_CHARS 50
#define PATH "./"

int main() {
	int userChoice;
	int caseChoice;
	int numLines = 0;
	int selectionComplete;
	char cwd[2048] = PATH; // Keep track of current working directory
	char* welcomeString = "1. Select file to process\n2. Exit the program\nEnter a choice 1 or 2: ";
	char* caseString = "\nWhich file you want to process?\nEnter 1 to pick the largest file\nEnter 2 to pick the smallest file\nEnter 3 to specify the name of a file\n\nEnter a choice from 1 to 3: ";
	srand(time(0));

	do {
		// Prompt user until quit is chosen.
		userChoice = 0;
		userChoice = validateInputInt(welcomeString, 1, 2);
		selectionComplete = 0; // Track whether we need to repeat inner loop or not (case 3)
		if (userChoice == 1) {
			while (!selectionComplete) {
				caseChoice = validateInputInt(caseString, 1, 3);
				switch (caseChoice) {
				case 1:
				{
					/*
					* Find the largest file with the extension csv in the current directory whose name starts with the movies_ and automatically process it.
					If tie, pick any of the files with the extension csv starting with movies_ that have the largest size.
					*/
					char* fileName = calloc(MAX_FILE_NAME_CHARS, sizeof(char));

					// Get name of largest CSV file
					if ((fileName = getCsvFile(cwd, _SC_INT_MIN, &maxComparator))) {

						// Process file and create linked list of nodes that contain title, year info.
						printf("Now processing the chosen file named %s\n", fileName);
						strcat(cwd, fileName);
						struct movie* list = processFile(cwd, &numLines);
						// Sort list by year
						sortMovies(list, &maxComparator);
						//printMovie(list);

						// Create new directory with ONID.movies.rand_num with permissions to rwxr-x---
						char* dirName = calloc(20, sizeof(char));
						dirName = createRandDirinCurr(0755);
						if (!dirName) {
							printf("Error creating directory.\n");
						}
						// Create Files in Directory
						createTxtFiles(dirName, list);
					}
					else {
						printf("No file found.\n");
					}
					free(fileName);
					memset(cwd, '\0', sizeof(cwd));
					strcpy(cwd, PATH);
					selectionComplete = 1;
					break;
				}
				case 2:
				{
					/*
					* Find the smallest file with the extension csv in the current directory whose name starts with the movies_ and automatically process it.
					If tie, pick any of the files with the extension csv starting with movies_ that have the largest size.
					*/
					char* fileName = calloc(MAX_FILE_NAME_CHARS, sizeof(char));
					// Get name of largest CSV file
					if ((fileName = getCsvFile(cwd, _SC_INT_MAX, &minComparator))) {

						// Process file and create linked list of nodes that contain title, year info.
						printf("Now processing the chosen file named %s\n", fileName);
						strcat(cwd, fileName);
						struct movie* list = processFile(cwd, &numLines);
						// Sort list by year
						sortMovies(list, &minComparator);
						//printMovie(list);

						// Create new directory with ONID.movies.rand_num with permissions to rwxr-x---
						char* dirName = calloc(20, sizeof(char));
						dirName = createRandDirinCurr(0755);
						if (!dirName) {
							printf("Error creating directory.\n");
						}
						// Create Files in Directory
						createTxtFiles(dirName, list);
					}
					else {
						printf("No file found.\n");
					}
					free(fileName);
					memset(cwd, '\0', sizeof(cwd));
					strcpy(cwd, PATH);
					selectionComplete = 1;
					break;
				}
				case 3:
				{
					char* fileName = calloc(MAX_FILE_NAME_CHARS, sizeof(char));
					printf("Enter the complete file name: ");
					scanf("%s", fileName); // No input validation
					printf("Now processing the chosen file named %s\n", fileName);

					// Check if file exists. If not, continue inner loop
					if (findFile(PATH, fileName)) {
						// Create new directory with ONID.movies.rand_num with permissions to rwxr-x---
						char* dirName = calloc(20, sizeof(char));
						dirName = createRandDirinCurr(0755);
						if (!dirName) {
							printf("Error creating directory.\n");
						}
						strcat(cwd, fileName);
						struct movie* list = processFile(cwd, &numLines);
						// Sort list by year
						sortMovies(list, &minComparator);
						// Create Files in Directory
						createTxtFiles(dirName, list);
						selectionComplete = 1;
					}
					else {
						printf("The file %s was not found. Try again\n", fileName);
						selectionComplete = 0;
					}
					free(fileName);
					memset(cwd, '\0', sizeof(cwd));
					strcpy(cwd, PATH);
					break;
				}
				default:
					break;
				}
			}
		}
	} while (userChoice != 2);
	return 0;
}