#include <stdio.h>
#include <stdlib.h>
#include "Helpers.h"
#include <string.h>

int main(int argc, char* argv[])
{
	// Read File
	// Get num lines and file name
	char* menu = "1. Show movies released in the specified year\n\
		2. Show highest rated movie for each year\n\
		3. Show the title and year of release of all movies in a specific language\n\
		4. Exit from the program\n\n\
		Enter a choice from 1 to 4: ";
	int menu_choice;
	char case_choice[1];
	int num_lines = 0;

	argv[1] = "./movie_sample_1.csv";
	struct movie* list = processFile(argv[1], &num_lines);

	printf("Processed file %s and parsed data for %d movies\n", argv[1], num_lines);

	//do {		
	//	menu_choice = validate_input_int(menu, 1, 4);

	//	switch (menu_choice)
	//	{
	//	case 1:
	//	{
	//		printf("Enter the year for which you want to see movies: ");
	//		scanf("%s", case_choice);
	//		// Loop through list and only print out matching year
	//		// No input validation needed
	//		int exists = 0;
	//		struct movie* temp = list;
	//		while (temp != NULL) {
	//			if (temp->Year == atoi(case_choice)){
	//			//if (if temp->Year == atoi(case_choice)) {
	//				printf("%s\n", temp->Title);
	//				exists = 1;
	//			};
	//			temp = temp->next;
	//		};
	//		if (!exists) {
	//			printf("No data about movies released in the year %s\n", case_choice);
	//		}
	//		break;
	//	}
	//	case 2:
	//	{
	//		struct movie* temp = list;
	//		struct movie* maxPerYear;
	//		struct movie* movieByYear;
	//		while (temp != NULL) {
	//			movieByYear = moviesByYear(temp, temp->Year);
	//			maxPerYear = highestRatingByYear(movieByYear);
	//			printf("%d 0.1f %s\n", maxPerYear->Year, maxPerYear->Rating, maxPerYear->Title);
	//			temp = temp->next;
	//	};
	//	break;
	//	}
	//	case 3:
	//	{
	//		printf("Enter the language for which you want to see movies: ");
	//		scanf("%s", case_choice);
	//		// Find case_choice (language) in file. If not found, printf:
	//		struct movie* temp = list;
	//		int exists = 0;
	//		while (temp != NULL) {
	//			if (parseLanguage(temp->Languages, case_choice) != NULL) {
	//				printf("%d %s\n", temp->Year, temp->Title); // NEED TO PARSE THIS
	//				exists = 1;
	//			};
	//			temp = temp->next;
	//		};
	//		if (!exists) {
	//			printf("No data about movies released in %s\n", case_choice);
	//		}
	//		break;
	//	}
	//	case 4:
	//		break;
	//	default:
	//		break;
	//	}
	//} while (menu_choice != 4);

	return 0;
}
