#define MAX_LANGUAGES 5
struct movie {
	char* Title;
	int Year;
	char* Languages[MAX_LANGUAGES];
	double Rating;
	struct movie* next;
	int numLanguages;
};

// Poor-man's map data structure. Will use an array of tuple's. Better solution is to create a hash map with insert via hash function, etc.
struct tuple {
	int year;
	double rating;
};

struct keysValue{
	char* title;
	struct tuple yearRating;
	struct keysValue* next;
};

// create keysValue pointer, list pointer
// while list != NULL
//	if keysValue pointer is NULL
//		create first node of <title, <year, rating>> with list value, set next pointer to NULL
//	else
//		loop through each keysValue
//			if list->year exists in keysValues->yearRating[key]
//				compare ratings
//				replace if current > existing
//			else
//				create new node of <title, <year, rating>> with list value
//				set current->next = new node
//				new node->next = NULL
// return pointer to tuple linked list
// print and then free

struct keysValue* createKeysValueList(struct movie* list);
struct keysValue* createKeysValue(struct movie* list);
//struct keysValue* setHighestRating(struct keysValue* kv); // find and replace
void printKeysValue(struct keysValue* list);
void freeKeysValue(struct keysValue* list);







struct movie* processFile(char*, int*);

struct movie* createMovie(char*);

void filterChar(char*, char*, char);

int validateInputInt(const char* menu, const int lbound, const int ubound);

void flushStdin(void);

void freeChars(struct movie*);
void freeMovie(struct movie*);