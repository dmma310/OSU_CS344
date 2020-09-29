#include <stdio.h>
#include <stdlib.h>
#include "Helpers.h"
#include <string.h>
#define MOVIE_IN_ONE_YEAR 1
#define ALL_MOVIES_BY_YEAR 2
#define ALL_MOVIES_BY_LANGUAGE 3
#define EXIT_PROGRAM 4


int main(int argc, char* argv[])
{
	// Read File
	// Get num lines and file name
	char* menu = "1. Show movies released in the specified year\n\ 2. Show highest rated movie for each year\n\ 3. Show the title and year of release of all movies in a specific language\n\ 4. Exit from the program\n\n\ Enter a choice from 1 to 4: ";
	int menuChoice;
	int caseChoice;
	int numLines = 0;

	char cwd[2048];
	getcwd(cwd, sizeof(cwd));
	strcat(cwd, "/vsprojects/homework1/");
	argv[1] = "movie_sample_1.csv";
	strcat(cwd, argv[1]);
	printf("%s\n", cwd);
	struct movie* list = processFile(cwd, &numLines);
	//struct movie* list = processFile(argv[1], &numLines);

	printf("Processed file %s and parsed data for %d movies\n\n", argv[1], numLines);

	do {		
		menuChoice = validateInputInt(menu, 1, 4);

		switch (menuChoice)
		{
		case MOVIE_IN_ONE_YEAR:
		{
			char* s = "Enter the year for which you want to see movies: ";
			caseChoice = validateInputInt(s, 1900, 2020);
			//printf("Enter the year for which you want to see movies: ");
			//scanf("%d", caseChoice); // Assume valid user input
			// No input validation needed
			int exists = 0;
			// Loop through list and only print out matching year
			struct movie* temp = list;
			while (temp != NULL) {
				if (temp->Year == caseChoice){
					printf("%s\n", temp->Title);
					exists = 1;
				};
				temp = temp->next;
			};
			if (!exists) {
				printf("No data about movies released in the year %s\n", caseChoice);
			}
			break;
		}
		//case ALL_MOVIES_BY_YEAR:
		//{
		//	struct movie* temp = list;
		//	struct movie* maxPerYear;
		//	struct movie* movieByYear;
		//	while (temp) {
		//		movieByYear = moviesByYear(temp, temp->Year);
		//		maxPerYear = highestRatingByYear(movieByYear);
		//		printf("%d 0.1f %s\n", maxPerYear->Year, maxPerYear->Rating, maxPerYear->Title);
		//		temp = temp->next;
		//};
		//break;
		//}
		//case ALL_MOVIES_BY_LANGUAGE:
		//{
		//	printf("Enter the language for which you want to see movies: ");
		//	scanf("%s", caseChoice);
		//	// Find caseChoice (language) in file. If not found, printf:
		//	struct movie* temp = list;
		//	int exists = 0;
		//	while (temp != NULL) {
		//		if (parseLanguage(temp->Languages, caseChoice) != NULL) {
		//			printf("%d %s\n", temp->Year, temp->Title); // NEED TO PARSE THIS
		//			exists = 1;
		//		};
		//		temp = temp->next;
		//	};
		//	if (!exists) {
		//		printf("No data about movies released in %s\n", caseChoice);
		//	}
		//	break;
		//}
		case 4:
			break;
		default:
			break;
		}
	} while (menuChoice != EXIT_PROGRAM);

	freeMovie(list);

	return 0;
}
