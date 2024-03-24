#define PORT 8080
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#define BUFFER_SIZE 256

int main()
{
    while (1)
    {
        int server_fd, new_socket;
        struct sockaddr_in address;
        int addrlen = sizeof(address);
        int tr = 1;
        char *message = "Hello world! (from server)";

        if ((server_fd = socket(AF_INET, SOCK_STREAM, 0)) == 0)
        {
            perror("socket failed");
            exit(EXIT_FAILURE);
        }
        address.sin_family = AF_INET;
        address.sin_addr.s_addr = INADDR_ANY;
        address.sin_port = htons(PORT);
        if (setsockopt(server_fd, SOL_SOCKET, SO_REUSEADDR, &tr, sizeof(int)) < 0)
        {
            perror("setsockopt(SO_REUSEADDR) failed");
            exit(EXIT_FAILURE);
        }
        if (bind(server_fd, (struct sockaddr *)&address, sizeof(address)) < 0)
        {
            perror("bind failed");
            exit(EXIT_FAILURE);
        }
        printf("Listening...\n");
        if (listen(server_fd, 5) < 0)
        {
            perror("listen");
            exit(EXIT_FAILURE);
        }
        if ((new_socket = accept(server_fd, (struct sockaddr *)&address, (socklen_t *)&addrlen)) < 0)
        {
            perror("accept");
            exit(EXIT_FAILURE);
        }
        // the client will send a text file like "mississipi.txt"
        // send client 100 bytes of the file at a time, until the EOF
        
        // first, read the file name that was sent through the TCP connection
        char buffer[BUFFER_SIZE] = {0};
        int valread = read(new_socket, buffer, BUFFER_SIZE);
        printf("Received (valread=%d): \"%.*s\"\n", valread, valread, buffer);

        // now open the file
        FILE *file = fopen(buffer, "r");
        if (file == NULL)
        {
            perror("fopen");
            exit(EXIT_FAILURE);
        }

        // read the file 100 bytes at a time and send it to the client
        while (1)
        {
            char buffer[BUFFER_SIZE] = {0};
            int valread = fread(buffer, 1, BUFFER_SIZE, file);
            if (valread == 0)
            {
                break;
            }
            send(new_socket, buffer, valread, 0);
        }

        // close the file and the sockets
        fclose(file);
        close(server_fd);
        close(new_socket);
    }
    return 0;
}
