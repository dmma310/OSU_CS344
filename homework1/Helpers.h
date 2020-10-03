#define MAX_LANGUAGES 5
// Structure to hold movie information.
struct movie {
	char* Title;
	int Year;
	char* Languages[MAX_LANGUAGES];
	double Rating;
	struct movie* next;
	int numLanguages;
};

// Poor-man's map data structure. Will use an array of pair's. Better solution is to create a hash map with insert via hash function, etc.
struct pair {
	char* title;
	double rating;
};

// Used in linked list to store year with highest movie rating and its title
struct keyValues {
	int year;
	struct pair titleRating;
	struct keyValues* next;
};

// Creates a linked list of <year, <title, rating>> nodes from a list of movie nodes. All years are unique with the highest rating stored for that year.
struct keyValues* createKeysValueList(struct movie* list);

// Creates a <year, <title, rating>> struct from movie node
struct keyValues* createKeysValue(struct movie* list);

// Display title and highest rating for each year
void printKeysValue(struct keyValues* list);

// Frees the linked list of nodes containing <year, <title, rating>>
void freeKeysValue(struct keyValues* list);

// Process csv file and stores each record in a movie node.
struct movie* processFile(char*, int*);
// Create new movie node
struct movie* createMovie(char*);

// Used to validate user integer input. Assumes valid integer input.
int validateInputInt(const char* menu, const int lbound, const int ubound);

// Flush STDIN buffer
void flushStdin(void);

// Free memory of movies linked list
void freeMovie(struct movie*);
// Copy source to destination, with comparator char removed
void filterChar(char*, char*, char);
void printMoviesByLanguage(struct movie*, char*);

void processMovieLanguages(struct movie*, char*);