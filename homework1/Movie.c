#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "Helpers.h"
#define MOVIE_IN_ONE_YEAR 1
#define ALL_MOVIES_BY_YEAR 2
#define ALL_MOVIES_BY_LANGUAGE 3
#define EXIT_PROGRAM 4


int main(int argc, char* argv[])
{
	// Read File, get num lines and file name
	char* menu = "1. Show movies released in the specified year\n2. Show highest rated movie for each year\n3. Show the title and year of release of all movies in a specific language\n4. Exit from the program\n\nEnter a choice from 1 to 4: ";
	int menuChoice;
	int numLines = 0;

	//char cwd[2048];
	//getcwd(cwd, sizeof(cwd));
	//strcat(cwd, "/vsprojects/homework1/");
	//argv[1] = "movie_sample_1.csv";
	//strcat(cwd, argv[1]);
	//printf("%s\n", cwd);
	//struct movie* list = processFile(cwd, &numLines);
	struct movie* list = processFile(argv[1], &numLines);

	printf("Processed file %s and parsed data for %d movies\n\n", argv[1], numLines);

	// Provide user with menu options until user decides to quit
	do {
		menuChoice = validateInputInt(menu, 1, 4);

		switch (menuChoice)
		{
		case MOVIE_IN_ONE_YEAR:
		{
			char* s = "Enter the year for which you want to see movies: ";
			flushStdin();
			// No input validation needed
			int caseChoice = validateInputInt(s, 1000, 9999);
			int exists = 0;
			// Loop through list and only print out matching year
			struct movie* temp = list;
			while (temp != NULL) {
				if (temp->Year == caseChoice) {
					printf("%s\n", temp->Title);
					exists = 1;
				};
				temp = temp->next;
			};
			if (!exists) {
				printf("No data about movies released in the year %d\n", caseChoice);
			}
			printf("\n");
			break;
		}
		case ALL_MOVIES_BY_YEAR:
		{
			flushStdin();
			struct keyValues* temp = createKeysValueList(list);
			printKeysValue(temp);
			freeKeysValue(temp);
		break;
		}
		case ALL_MOVIES_BY_LANGUAGE:
		{
			char caseChoice[20];
			flushStdin();
			printf("Enter the language for which you want to see movies: ");
			scanf("%s", caseChoice);
			// Find caseChoice (language) in file. If not found, printf:
			// No input validation needed
			// Exact match needed i.e. 'English' != 'english'
			struct movie* temp = list;
			int exists = 0;
			while (temp != NULL) {
				for (int i = 0; i < temp->numLanguages; ++i) {
					if (strcmp(temp->Languages[i], caseChoice) == 0) {
						printf("%d %s\n", temp->Year, temp->Title); // NEED TO PARSE THIS
						exists = 1;
						break;
					}
				}
				temp = temp->next;
			};
			if (!exists) {
				printf("No data about movies released in %s\n", caseChoice);
			}
			break;
			printf("\n\n");
		}
		case 4:
			break;
		default:
			break;
		}
	} while (menuChoice != EXIT_PROGRAM);

	// Free memory/
	freeMovie(list);

	return 0;
}
