#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <unistd.h>

#define BUFLEN 1024

int echod(int sd)
{
    char *bp, buf[BUFLEN];
    memset(buf, 0, BUFLEN); // zero out buffer
    int n, bytes_to_read;
    while((n = read(sd, buf, BUFLEN)))
    {
        write(sd, buf, n); 
    }
    close(sd);
    return(0);
} 

int main()
{
    int sd, new_sd;
    struct sockaddr_in server, client;
    int client_len;

    sd = socket(AF_LOCAL, SOCK_STREAM, 0);
    bind(sd, (struct sockaddr *)&server, sizeof(server));         // binds socket to IP and port
    listen(sd, 5);                                                // puts socket into passive mode and accepts data of size 5
    new_sd = accept(sd, (struct sockaddr *)&client, &client_len); // accepts connection from client

    switch (fork())
    {
    case 0: /* child */
        (void) close(sd);
        exit(echod(new_sd));
    default: /* parent */
        (void) close(new_sd);
        break;
    case -1:
        fprintf(stderr, "fork: error\n");
    }
}