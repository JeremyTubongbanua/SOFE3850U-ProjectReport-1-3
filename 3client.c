#define PORT 8080
#define IP_ADDRESS "192.168.0.114"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <netdb.h>

#define BUFFER_SIZE 256

int main(int argc, char *argv[]) {
    int sock = 0, valread;
    struct sockaddr_in serv_addr;
    char *message = "Hello world! (from client)";
    char buffer[BUFFER_SIZE] = {0};

    if ((sock = socket(AF_INET, SOCK_STREAM, 0)) < 0) {
        printf("\n Socket creation error \n");
        return -1;
    }
    serv_addr.sin_family = AF_INET;
    serv_addr.sin_port = htons(PORT);
    if (inet_pton(AF_INET, IP_ADDRESS, &serv_addr.sin_addr) <= 0) {
        printf("\nInvalid address/ Address not supported \n");
        return -1;
    }
    if (connect(sock, (struct sockaddr *)&serv_addr, sizeof(serv_addr)) < 0) {
        printf("\nConnection Failed \n");
        return -1;
    }

    // take file name from argv[1]
    char *file_name;
    if (argc < 2) {
        printf("Usage: %s <file_name>\n", argv[0]);
        return 1;
    }
    file_name = argv[1];

    // send the file name to the server
    send(sock, file_name, strlen(file_name), 0);
    printf("Sent: \"%s\"\n", file_name);

    // receive the file from the server
    // the server sends 100 bytes of the file at a time. keep receiving until the EOF
    while ((valread = read(sock, buffer, BUFFER_SIZE)) > 0) {
        printf("Received (valread=%d): \"%.*s\"\n", valread, valread, buffer);
    }

    // close the socket
    close(sock);
    return 0;
}
