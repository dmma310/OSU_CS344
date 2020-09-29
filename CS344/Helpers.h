struct movie {
	char Title[20];
	int Year; // int??
	char Languages[106];
	double Rating; // double??
	struct movie* next;

};

int validate_input_int(const char* menu, const int lbound, const int ubound);

//struct movie* highestRatingByYear(struct movie*);

struct movie* processFile(char*, int*);

//void printMovie(struct movie*);
//
//void printMovieList(struct movie*, char*);

struct movie* createMovie(char*);

//struct movie* moviesByYear(struct movie*, int);
//
//struct movie* createCopyMovie(struct movie*);
//
//char* parseLanguage(char*, char*);

void FlushStdin(void);