#define MAX_LANGUAGES 5

// Structure to hold movie information
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
	int year;
	double rating;
};

// Node used to hold movie with highest rating in given year
struct keysValue{
	char* title;
	struct pair yearRating;
	struct keysValue* next;
};

// Creates a linked list of <year, <title, rating>> nodes from a list of movie nodes. All years are unique with the highest rating stored for that year.
struct keysValue* createKeysValueList(struct movie* list);

// Creates a <year, <title, rating>> struct from movie node
struct keysValue* createKeysValue(struct movie* list);

// Display title and highest rating for each year
void printKeysValue(struct keysValue* list);

// Frees the linked list of nodes containing <year, <title, rating>>
void freeKeysValue(struct keysValue* list);

// Process csv file and stores each record in a movie node.
struct movie* processFile(char*, int*);

// Create a movie node and return a pointer to it.
struct movie* createMovie(char*);

// Copy all characters from source to destination except comparator
void filterChar(char*, char*, char);

// Validates user integer input bounds. Assumes valid integer input.
int validateInputInt(const char* menu, const int lbound, const int ubound);

// Flush stdin buffer
void flushStdin(void);

// Frees the array of character pointers (languages) in the movie structure
void freeChars(struct movie*);
// Frees the linked list of movie nodes
void freeMovie(struct movie*);