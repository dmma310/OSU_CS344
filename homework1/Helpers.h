struct movie {
	char* Title;
	int Year;
	char* Languages[5];
	double Rating;
	struct movie* next;

};

struct movie* processFile(char*, int*);

struct movie* createMovie(char*);


int validateInputInt(const char* menu, const int lbound, const int ubound);

void flushStdin(void);

void freeMovie(struct movie*);

//struct movie* moviesByYear(struct movie*, int);
//
//struct movie* createCopyMovie(struct movie*);
//
//char* parseLanguage(char*, char*);

//void printMovie(struct movie*);
//
//void printMovieList(struct movie*, char*);
//struct movie* highestRatingByYear(struct movie*);
