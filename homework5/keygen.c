#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <time.h>

// Generate random cipher key with possible defined characters. Length determined by user at bash shell prompt.
int main(int argc, char* argv[]) {
	srand(time(NULL));

	if (argc < 2) { //print usage error if no second arg (length of key)
		fprintf(stderr, "Usage: <%s> <length>\n", argv[0]);
		exit(0);
	}

	int length = atoi(argv[1]); // Get length and cast to int
	char key[length + 1]; // Add extra space for \0
	char* arr = " ABCDEFGHIJKLMNOPQRSTUVWXYZ"; // All possible cipher characters per assignment instructions

	// Create cipher randomly chosen from arr. Modulo addition used.
	for (int i = 0; i < length; ++i)
		key[i] = arr[random() % 27];

	key[length] = '\0'; // set last char to \0

	// Prints key and newline to stdout. NOTE: redirection (if applicable) is handled by bash shell
	printf("%s\n", key);
	return 0;
}