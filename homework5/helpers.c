#include "helpers.h"

// Set up the address struct
void setupAddressStruct(struct sockaddr_in* address,
    int portNumber,
    char* hostname) {

    // Clear out the address struct
    memset((char*)address, '\0', sizeof(*address));

    // The address should be network capable
    address->sin_family = AF_INET;
    // Store the port number
    address->sin_port = htons(portNumber);

    // Figure out if server or client
    if (strcmp(hostname, "INADDR_ANY") == 0) {
        address->sin_addr.s_addr = INADDR_ANY;
    }
    else {
        // Get the DNS entry for this host name
        struct hostent* hostInfo = gethostbyname(hostname);
        if (hostInfo == NULL) {
            fprintf(stderr, "CLIENT: ERROR, no such host\n");
            exit(0);
        }
        // Copy the first IP address from the DNS entry to sin_addr.s_addr
        memcpy((char*)&address->sin_addr.s_addr,
            hostInfo->h_addr_list[0],
            hostInfo->h_length);
    }
}


// Source: https://www.geeksforgeeks.org/c-program-to-count-the-number-of-characters-in-a-file/
// Extract file contents into array, one character at a time. Note only space (' ') and A-Z are valid.
int get_file(char* storage_arr, const char* file_name) {
    int count = 0;
    // Open the file 
    FILE* fp = fopen(file_name, "r");

    // Check if file exists 
    if (fp == NULL) {
        fprintf(stderr, "Could not open file %s.\n", file_name);
        return count;
    }

    // Extract characters from file and store in character c. Store all contents in array. 
    for (char c = getc(fp); c != EOF && c != '\n'; c = getc(fp)) {
        if (!isupper(c) && c != ' ') {
            fclose(fp);
            memset(storage_arr, 0, FULL_MSG_SIZE);
            return -1;
        }
        storage_arr[count++] = c;
    }
    storage_arr[count] = '\0';

    // Close the file 
    fclose(fp);
    return count;
}

// Converts character to integer, returns [0-26] from either ' ' or A-Z since these are the only valid characters for this.
int char_to_int(char c) {
    if (c == ' ') {
        return 26;
    }
    else {
        return (c - 'A');
    }
    return 0;
}

// Converts integer to character, returns either ' ' or A-Z since these are the only valid characters for this.
char int_to_char(int i) {
    if (i == 26) {
        return ' ';
    }
    else {
        return (i + 'A');
    }
}

// Encrypt msg text using key text
void encrypt(char* full_msg, char* msg_text, char* key_text) {
    int num, i = 0;
    while (msg_text[i] != '\0') {
        // Convert characters to integers
        num = (char_to_int(msg_text[i]) + char_to_int(key_text[i])) % 27;
        // Convert integer back into character and store
        full_msg[i++] = int_to_char(num);
    }
    full_msg[i++] = '\n';
    full_msg[i] = '\0';
}

// Decrypt text using key text
void decrypt(char* full_msg, char* msg_text, char* key_text) {
    int num, i = 0;
    while (msg_text[i] != '\0') {
        // Convert characters to integers
        num = char_to_int(msg_text[i]) - char_to_int(key_text[i]);
        if (num < 0) { // Handle negative case
            num += 27;
        }
        full_msg[i++] = int_to_char(num);
    }
    full_msg[i++] = '\n';
    full_msg[i] = '\0';
}

// Source: https://beej.us/guide/bgnet/html/#sendall
// Sends all buf contents up to len to socket_fd
int sendall(int socket_fd, char* buf, int* len)
{
    int total = 0;        // how many bytes we've sent
    int bytes_left = *len; // how many we have left to send
    int n = 0;

    while (total < *len) {
        n = send(socket_fd, buf + total, bytes_left, 0);
        if (n == -1) { break; }
        total += n;
        bytes_left -= n;
    }

    *len = total; // return number actually sent here

    return n == -1 ? -1 : 0; // return -1 on failure, 0 on success
}

// Read all contents from socket_fd into buf, up to len
int readall(int socket_fd, char* buf, int len)
{
    int total = 0;        // how many bytes we've received
    int n = 0;
    char tmp_buf[TMP_BUFF_SIZE] = { 0, };
    //Read until same number of bytes read as sent. Result is the full cypher message returned in buf
    while (total < len) {
        // Read data from the socket, leaving \0 at end
        n = recv(socket_fd, tmp_buf, TMP_BUFF_SIZE - 1, 0);
        if (n == -1) { break; }
        strcat(buf, tmp_buf);
        memset(tmp_buf, 0, TMP_BUFF_SIZE);
        total += n;
    }
    return n == -1 ? -1 : 0; // return -1 on failure, 0 on success
}