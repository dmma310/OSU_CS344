#include "helpers.h"

int main(int argc, char* argv[]) {
    int listen_socket_fd, connected_client_fd, pid_status, chars_rd = 0;
    const int is_reusable = 1;
    int ok_len = strlen(SVR_ALLOW_MSG), cli_id_len = strlen(ENC_CLI_CONFIRM_MSG);
    struct sockaddr_in server_address, client_address;
    socklen_t size_of_client_info;
    pid_t pid;

    /*****************Establish listening*****************/
    // Check usage & args
    if (argc < 2) {
        fprintf(stderr, "USAGE: <%s> <port>\n", argv[0]);
        exit(1);
    }

    // Set up the server address struct using localhost; argv[1] is port number
    setupAddressStruct(&server_address, atoi(argv[1]), "INADDR_ANY");


    // Create a socket to connect to
    listen_socket_fd = socket(AF_INET, SOCK_STREAM, 0);

    if (listen_socket_fd < 0) {
        fprintf(stderr, "SERVER: ERROR opening socket\n");
        exit(1);
    }

    // Bind the socket to to the port and begin listening
    if (bind(listen_socket_fd, (struct sockaddr*)&server_address, sizeof(server_address)) < 0) {
        close(listen_socket_fd);
        fprintf(stderr, "SERVER: ERROR on binding\n");
        exit(1);
    }
    listen(listen_socket_fd, MAX_SERVER_CONNECTIONS);

    /*****************Accept client connections*****************/
    while (1) {
        // Accept client connection
        size_of_client_info = sizeof(client_address);
        if ((connected_client_fd = accept(listen_socket_fd, (struct sockaddr*)&client_address, &size_of_client_info)) < 0) {
            close(listen_socket_fd);
            fprintf(stderr, "SERVER: ERROR cannot accept\n");
            exit(1);
        }

        // Use child to process request
        pid = fork();
        switch (pid)
        {
            case -1:
            {
                close(listen_socket_fd);
                fprintf(stderr, "SERVER: ERROR could not fork.\n");
                exit(1);
            }
            // Inside child process
            case 0:
            {
                char buffer[FULL_MSG_SIZE] = { 0, };
                char full_encrypted_msg[FULL_MSG_SIZE];
                char plain_text[FULL_MSG_SIZE] = { 0, };
                char key_text[FULL_MSG_SIZE] = { 0, };
                int buffer_len = FULL_MSG_SIZE;

                /*****************Check correct client connecting*****************/
                // Check whether correct client is connecting or not, and send response.
                if (readall(connected_client_fd, buffer, cli_id_len) == -1) {
                    exit(1);
                }

                if (strcmp(buffer, ENC_CLI_CONFIRM_MSG) != 0) {
                    int len = strlen(SVR_BAD_PORT_MSG);
                    if (sendall(connected_client_fd, SVR_BAD_PORT_MSG, &len) == -1) {
                        fprintf(stderr, "SERVER: ERROR cannot write to client\n");
                    }
                    exit(1);
                }
                // Everything is ok to proceed, so send ok response to client
                if ((sendall(connected_client_fd, SVR_ALLOW_MSG, &ok_len)) == -1) {
                    fprintf(stderr, "SERVER: ERROR cannot write to client\n");
                }
                // Get length of plain text + key file
                memset(buffer, 0, FULL_MSG_SIZE);
                chars_rd = 0;

                /*****************Get file length*****************/
                // Get file length
                while ((chars_rd = recv(connected_client_fd, buffer, FULL_MSG_SIZE, 0)) == 0);
                buffer_len = atoi(buffer); // convert ASCII to int
                chars_rd = 0;

                // Tell client to proceed
                if ((sendall(connected_client_fd, SVR_ALLOW_MSG, &ok_len)) == -1) {
                    fprintf(stderr, "SERVER: ERROR cannot write to client\n");
                }
                memset(buffer, 0, FULL_MSG_SIZE);

                /*****************Get plain text + key text*****************/
                if (readall(connected_client_fd, buffer, buffer_len) == -1) {
                    exit(1);
                }
                /*****************Process plain text and key text*****************/
                // Separate plain text and key text files
                char* rest;
                char* token = strtok_r(buffer, FILE_DELIMITER, &rest);
                strcpy(plain_text, token);
                token = strtok_r(NULL, FILE_DELIMITER, &rest);
                strcpy(key_text, token);
                // Produce and get encrypted message
                encrypt(full_encrypted_msg, plain_text, key_text);
                int full_encrypted_msg_len = strlen(full_encrypted_msg);

                /*****************Send encrypted message to client*****************/
                int encrypt_msg_len = strlen(full_encrypted_msg);
                if ((sendall(connected_client_fd, full_encrypted_msg, &encrypt_msg_len)) == -1) {
                    fprintf(stderr, "SERVER: ERROR cannot write to client\n");
                }
                exit(0);
            }
            default:
            {
                // Reap zombie process
                pid_t actual_pid = waitpid(pid, &pid_status, WNOHANG);
            }
        }
        close(connected_client_fd);
        size_of_client_info = 0;
    }

    close(listen_socket_fd);
    return 0;
}

