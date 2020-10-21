#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <dirent.h>
#include <string.h>
#include <time.h>
#include <unistd.h>
#include <time.h>
#define MAX_READ_BUFFER 2048
#define MAX_TITLE_LEN 100
#define MAX_FILE_NAME_CHARS 50
#define PREFIX "movies_"
#define SUFFIX ".csv"
#define MIN_RAND_INT 0
#define MAX_RAND_INT 99999
#define ONID "made"// Structure to hold movie information.

// Holds movie info
struct movie {
	char* Title;
	int Year;
	struct movie* next;
};

// Returns name of csv file with desired properties (prefix match + suffix match + max or min)
char* getCsvFile(char* path, int init, int(*comparator)(const int p1, const int p2));

// Processes csv file and stores each record in a movie node. Returns head of resulting linked list
struct movie* processFile(char*, int*);

// Creates new movie node
struct movie* createMovie(char*);

// Used to validate user integer input. Assumes valid integer input.
int validateInputInt(const char* menu, const int lbound, const int ubound);

// Flush STDIN buffer
void flushStdin(void);

// Frees memory of movies linked list
void freeMovie(struct movie*);

// Create directory in current directory with given permissions
char* createRandDirinCurr(mode_t mode);

// Sort movie nodes
void sortMovies(struct movie*, int(*comparator)(const int p1, const int p2));

// Check if file exists
int findFile(char*, char*);


// Comparator functions used to extract max/min elements
int maxComparator(const int, const int);
int minComparator(const int, const int );

// Creates text files from given path and linked list of movies
void createTxtFiles(char*, struct movie*);

// Print movies
void printMovie(struct movie*);