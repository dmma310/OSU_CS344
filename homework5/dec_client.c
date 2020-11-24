#include "helpers.h"

int main(int argc, char* argv[]) {
    int socket_fd, num_key_text_chars, num_cipher_text_chars;
    struct sockaddr_in server_address;
    char cipher_text[FULL_MSG_SIZE] = { 0, };
    char key_text[FULL_MSG_SIZE] = { 0, };
    char buffer[FULL_MSG_SIZE];
    char full_plain_msg[FULL_MSG_SIZE];
    int is_reusable = 1, rd_len = strlen(SVR_ALLOW_MSG);


    /*****************Establish connection*****************/
    // Check usage & args
    if (argc < 3) {
        fprintf(stderr, "USAGE: <%s> <plaintext_file_name> <key_file_name> <port>\n", argv[0]);
        exit(1);
    }

    // Create a socket to connect to
    socket_fd = socket(AF_INET, SOCK_STREAM, 0);
    if (socket_fd < 0) {
        fprintf(stderr, "CLIENT: ERROR opening socket\n");
        exit(1);
    }

    // Set up the server address struct using localhost; argv[3] is port number, argv[1] is hostname
    setupAddressStruct(&server_address, atoi(argv[3]), "localhost");

    setsockopt(socket_fd, SOL_SOCKET, SO_REUSEADDR, &is_reusable, sizeof(int)); // make socket reusable

    // Connect to server
    if (connect(socket_fd, (struct sockaddr*)&server_address, sizeof(server_address)) < 0) {
        fprintf(stderr, "CLIENT: ERROR connecting\n");
        close(socket_fd);
        exit(1);
    }

    // Store contents of plain text and key text files into arrays, and check for valid characters and lengths
    num_cipher_text_chars = get_file(cipher_text, argv[1]);
    num_key_text_chars = get_file(key_text, argv[2]);
    if (num_key_text_chars == -1 || num_cipher_text_chars == -1) {
        fprintf(stderr, "%s: input contains bad characters\n", DEC_CLI_CONFIRM_MSG);
        close(socket_fd);
        exit(1);
    }
    if (num_cipher_text_chars > num_key_text_chars) { // If message length is greater than key length
        fprintf(stderr, "Error: key '%s' is too short\n", argv[2]);
        close(socket_fd);
        exit(1);
    }

    /*****************Send ID message*****************/
    /*Establish communication to server.
    * Send message to server indicating the source of this connection, wait for ok confirmation.
    * If ok, send data length.
    */

    int len = strlen(DEC_CLI_CONFIRM_MSG);
    if (sendall(socket_fd, DEC_CLI_CONFIRM_MSG, &len) == -1) {
        close(socket_fd);
        fprintf(stderr, "CLIENT: ERROR writing to socket\n");
        exit(1);
    }
    if (readall(socket_fd, buffer, rd_len) == -1) {
        close(socket_fd);
        fprintf(stderr, "CLIENT: ERROR reading from socket\n");
        exit(1);
    }

    if (strcmp(buffer, SVR_BAD_PORT_MSG) == 0 || strcmp(buffer, SVR_ALLOW_MSG) != 0) { //check if allowed to connect.
        close(socket_fd);
        fprintf(stderr, "Error: could not contact %s on port %d\n", DEC_SVR, atoi(argv[3]));
        exit(2);
    }

    /*****************Send file length*****************/
    // Send file length
    int file_len = num_cipher_text_chars + num_key_text_chars + 2; // Add 2 for \n after each file
    memset(buffer, 0, FULL_MSG_SIZE);
    sprintf(buffer, "%d", file_len);
    if (sendall(socket_fd, buffer, &file_len) == -1) {
        close(socket_fd);
        fprintf(stderr, "CLIENT: ERROR writing to socket\n");
        exit(1);
    }
    memset(buffer, 0, FULL_MSG_SIZE);

    // Wait for ok response from server
    if (readall(socket_fd, buffer, rd_len) == -1) {
        close(socket_fd);
        fprintf(stderr, "CLIENT: ERROR reading from socket\n");
        exit(1);
    }

    if (strcmp(buffer, SVR_BAD_PORT_MSG) == 0 || strcmp(buffer, SVR_ALLOW_MSG) != 0) { //check if allowed to         close(socket_fd);
        fprintf(stderr, "Error: could not contact %s on port %d\n", DEC_SVR, atoi(argv[3]));
        exit(2);
    }

    /*****************Send file contents*****************/
    // Send single file with contents <plain_text>\n<key_text\0>\n
    memset(buffer, 0, FULL_MSG_SIZE);
    strcat(buffer, cipher_text);
    strcat(buffer, "\n");
    strcat(buffer, key_text);
    strcat(buffer, "\n");

    if (sendall(socket_fd, buffer, &file_len) == -1) {
        close(socket_fd);
        fprintf(stderr, "CLIENT: ERROR writing to socket\n");
        exit(1);
    }
    /*****************Get plain text message from server*****************/
    rd_len = num_cipher_text_chars + 1;
    if (readall(socket_fd, full_plain_msg, rd_len) == -1) {
        close(socket_fd);
        fprintf(stderr, "CLIENT: ERROR reading from socket\n");
        exit(1);
    }
    printf(full_plain_msg); // Send full plain text message to stdout

    // Close the socket
    close(socket_fd);
    return 0;
}