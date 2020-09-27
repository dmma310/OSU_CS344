struct movie {
	char* Title;
	int Year;
	char* Languages[5];
	double Rating;
	struct movie* next;

};

struct movie* processFile(char*, int*);

struct movie* createMovie(char*);


int validate_input_int(const char* menu, const int lbound, const int ubound);

void FlushStdin(void);

//struct movie* moviesByYear(struct movie*, int);
//
//struct movie* createCopyMovie(struct movie*);
//
//char* parseLanguage(char*, char*);

//void printMovie(struct movie*);
//
//void printMovieList(struct movie*, char*);
//struct movie* highestRatingByYear(struct movie*);
