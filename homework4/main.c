#include "Helpers.h"

char shared_buffer_1[MAX_NUM_LINES * MAX_NUM_LINE_CHARS] = { 0, };
// Number of items in the buffer, shared resource used as state variable to wake/sleep threads
static int count_1 = 0;
static int prod_idx_1 = 0;
static int cons_idx_1 = 0;

// Initiaze mutexes and condition variables
pthread_mutex_t mutex_1 = PTHREAD_MUTEX_INITIALIZER;
// Initialize the condition variables
pthread_cond_t has_item_1 = PTHREAD_COND_INITIALIZER;

/********************************************************************************************************/
char shared_buffer_2[MAX_NUM_LINES * MAX_NUM_LINE_CHARS] = { 0, };
// Number of items in the buffer, shared resource used as state variable to wake/sleep threads
static int count_2 = 0;
static int prod_idx_2 = 0;
static int cons_idx_2 = 0;

// Initiaze mutexes and condition variables
pthread_mutex_t mutex_2 = PTHREAD_MUTEX_INITIALIZER;
// Initialize the condition variables
pthread_cond_t has_item_2 = PTHREAD_COND_INITIALIZER;

/********************************************************************************************************/
char shared_buffer_3[MAX_NUM_LINES * MAX_NUM_LINE_CHARS] = { 0, };
// Number of items in the buffer, shared resource used as state variable to wake/sleep threads
static int count_3 = 0;
static int prod_idx_3 = 0;
static int cons_idx_3 = 0;
static int output_idx_3 = 0;
char outputBuff[OUTPUT_SIZE] = { 0, }; // Used to store contents to write to stdout
static int shared_buffer_idx_3 = 0; // Used to keep track of current index in shared buffer 3

// Initiaze mutexes and condition variables
pthread_mutex_t mutex_3 = PTHREAD_MUTEX_INITIALIZER;
// Initialize the condition variables
pthread_cond_t has_item_3 = PTHREAD_COND_INITIALIZER;

int main(int argc, char* argv[])
{
	pthread_t input_thread, line_separator_thread, plus_sign_thread, output_thread;

	// Create threads
	pthread_create(&input_thread, NULL, input_function, NULL);
	pthread_create(&line_separator_thread, NULL, line_separator_function, NULL);
	pthread_create(&plus_sign_thread, NULL, plus_sign_function, NULL);
	pthread_create(&output_thread, NULL, output_function, NULL);

	// Join threads
	pthread_join(input_thread, NULL);
	pthread_join(line_separator_thread, NULL);
	pthread_join(plus_sign_thread, NULL);
	pthread_join(output_thread, NULL);

	return 0;
}

/*
 * Function reads input from stdin, one line at a time, until max number of lines reached or STOP instruction from file.
 * Puts contents into shared buffer 1.
 * Sets stop flag when done.
*/
void* input_function(void* args)
{
	/*
	* Stop flag that is passed in shared buffer to other threads
	* NOTE: Vavlid input will only consist of ASCII characters from space (decimal 32) to tilde (decimal 126).
	*/
	static char stop_flag = 0;
	char buffer[MAX_NUM_LINE_CHARS] = { 0, }; // Used to store contents of the line read from stdin
	int n = 0;
	// Loop until stop delimiter, nothing left to read, or max number of lines to read reached
	while (fgets(buffer, MAX_NUM_LINE_CHARS, stdin) && n++ < MAX_NUM_LINES && stop_flag != STOP_FLAG_INT_VAL) {
		pthread_mutex_lock(&mutex_1);
		// Check for stop flag in file and break out of loop as needed.
		if ((stop_flag = check_delimiter(buffer)) == STOP_FLAG_INT_VAL) {
			pthread_cond_signal(&has_item_1);
			pthread_mutex_unlock(&mutex_1);
			break;
		}
		else
		{
			put_buff_1(buffer); // Copy buffer contents into shared buffer 1 and update count 1
		}
		// Signal to the consumer that the buffer is no longer empty and unlock. Used by get_buff_1
		pthread_cond_signal(&has_item_1);
		pthread_mutex_unlock(&mutex_1);
	}
	pthread_mutex_lock(&mutex_1);
	// Copy stop flag into shared buffer 1 and update count 1
	shared_buffer_1[prod_idx_1++] = stop_flag;
	count_1 += 1;
	// Signal to the consumer that the buffer is no longer empty. Used by get_buff_1
	pthread_cond_signal(&has_item_1);
	pthread_mutex_unlock(&mutex_1);
	return NULL;
}

// Put contents from buffer into shared_buffer_1. Update count and next location to copy into.
void put_buff_1(char* buffer)
{
	strncpy(shared_buffer_1 + prod_idx_1, buffer, strlen(buffer));
	prod_idx_1 = strlen(shared_buffer_1);
	count_1 += strlen(buffer);
}

// Get contents from shared_buffer_1, one character at a time, and update count and next index to read from.
void get_buff_1(char* buffer, char currChar, char newChar)
{
	// Replace '\n' with ' '
	buffer[cons_idx_1] = shared_buffer_1[cons_idx_1] == currChar ? newChar : shared_buffer_1[cons_idx_1];
	cons_idx_1 += 1;
	count_1 -= 1;
}

/*
 Function gets contents from shared buffer 1, replaces all '\n' with ' ', and places new contents into shared buffer 2.
*/
void* line_separator_function(void* args)
{
	char tmp_buff[MAX_NUM_LINES * MAX_NUM_LINE_CHARS] = { 0, };
	// Continue consuming until the STOP_DELIMITER is seen and no more items to consume
	while (1)
	{
		// Lock mutex_1 before checking waiting until the shared buffer has data.      
		pthread_mutex_lock(&mutex_1);
		// Wait until there is an item in shared buffer 1 (input_function signals this).
		while (count_1 <= 0) {
			pthread_cond_wait(&has_item_1, &mutex_1);
		}
		// Get contents from shared_buffer_1 into temp buffer. Also replaces '\n' with ' '.
		get_buff_1(tmp_buff, '\n', ' ');
		pthread_mutex_unlock(&mutex_1);
		
		// Lock mutex_2 and put contents of temp buffer into shared_buffer_2.      
		pthread_mutex_lock(&mutex_2);
		put_buff_2(tmp_buff);
		pthread_cond_signal(&has_item_2);
		pthread_mutex_unlock(&mutex_2);

		// Check buffer for stop flag, and break out of loop as needed.
		if (tmp_buff[cons_idx_1 - 1] == STOP_FLAG_INT_VAL) {
			break;
		}
	}
	return NULL;
}

// Put contents from buffer into shared_buffer_2, one character at a time. Update count and next location to copy into.
void put_buff_2(char* buffer)
{
	shared_buffer_2[prod_idx_2] = buffer[prod_idx_2];
	prod_idx_2 += 1;
	count_2 += 1;
}

// Get contents from shared_buffer_2, one character at a time, and update count and next index to read from.
void get_buff_2(char* buffer)
{
	buffer[cons_idx_2] = shared_buffer_2[cons_idx_2];
	cons_idx_2 += 1;
	count_2 -= 1;
}

/*
 Function gets contents from shared buffer 2, replaces all '++' with '^', and places new contents into shared buffer 3.
*/
void* plus_sign_function(void* args)
{
	char tmp_buff[MAX_NUM_LINES * MAX_NUM_LINE_CHARS] = { 0, };
	// Continue consuming until the STOP_DELIMITER is seen and no more items to consume.
	while (1)
	{
		// Lock the mutex 2 before checking if the buffer has data      
		pthread_mutex_lock(&mutex_2);
		// Wait until there is an item in shared buffer 1 (lineSeparator function signals this).
		while (count_2 <= 0) {
			pthread_cond_wait(&has_item_2, &mutex_2);
		}
		get_buff_2(tmp_buff); // Get contents from shared buffer 2 into temp buffer for processing
		pthread_mutex_unlock(&mutex_2);


		// Lock  mutex 3 before checking if the buffer has data      
		pthread_mutex_lock(&mutex_3);
		// Put into shared buffer
		put_buff_3(tmp_buff); // Put temp buffer into shared buffer 3
		pthread_cond_signal(&has_item_3);
		pthread_mutex_unlock(&mutex_3);

		// Check buffer for stop flag, and break out of loop as needed.
		if (tmp_buff[cons_idx_2 - 1] == STOP_FLAG_INT_VAL) {
			break;
		}
	}
	return NULL;
}

// Put contents from buffer into shared_buffer_3, one character at a time. Update count and next location to copy into.
void put_buff_3(char* buffer)
{
	shared_buffer_3[shared_buffer_idx_3] = buffer[prod_idx_3];
	if (prod_idx_3 > 0 && buffer[prod_idx_3] == '+' && shared_buffer_3[shared_buffer_idx_3 - 1] == '+') {
		shared_buffer_3[shared_buffer_idx_3 - 1] = '^';
		shared_buffer_3[shared_buffer_idx_3] = '\0';
		prod_idx_3 += 1;
	}
	else
	{
		shared_buffer_3[shared_buffer_idx_3++] = buffer[prod_idx_3++];
		count_3 += 1;
	}
}

/*
 Function gets contents from shared buffer 3 into temp buffer, and writes to stdout 80 characters (plus \n) at a time.
*/
void* output_function(void* args) {
	char buff[MAX_NUM_LINES * MAX_NUM_LINE_CHARS] = { 0, };
	while (1) {
		pthread_mutex_lock(&mutex_3);

		while (count_3 <= 0) {
			pthread_cond_wait(&has_item_3, &mutex_3);
		}
		produce_output(buff);
		pthread_mutex_unlock(&mutex_3);

		// Check buffer for stop flag, and break out of loop as needed.
		if (buff[cons_idx_3 - 1] == STOP_FLAG_INT_VAL) {
			break;
		}
	}
	return NULL;
}

// Get contents from shared_buffer_3, writes to stdout if needed, and updates count 3
void produce_output(char* buffer)
{
	char oBuff[OUTPUT_SIZE] = { 0, };
	buffer[cons_idx_3++] = shared_buffer_3[output_idx_3++];
	count_3 -= 1;
	// Write if 80 characters reached.
	if (cons_idx_3 == OUTPUT_SIZE - 1) {
		memcpy(oBuff, buffer, strlen(buffer));
		oBuff[strlen(oBuff)] = '\n';
		fwrite(oBuff, sizeof(char), OUTPUT_SIZE, stdout);
		fflush(stdout);
		cons_idx_3 = 0;
		memset(buffer, 0, MAX_NUM_LINES * MAX_NUM_LINE_CHARS);
		memset(oBuff, 0, OUTPUT_SIZE);
	}
}