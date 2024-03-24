#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <unistd.h>

#define BUFLEN 1024

int main()
{
    int sd;
    struct sockaddr_in server, client;
    int client_len;

    sd = socket(AF_LOCAL, SOCK_STREAM, 0);
    connect(sd, (struct sockaddr *)&server, sizeof(server));

    char sbuf[BUFLEN], rbuf[BUFLEN];
    memset(sbuf, 0, BUFLEN);
    memset(rbuf, 0, BUFLEN);
    int n, i, bytes_to_read;
    char *bp;
    printf("Transmit: \n");
    while (n = read(0, sbuf, BUFLEN))
    {                       /* get user message */
        write(sd, sbuf, n); /* send it out */
        printf("Receive: \n");
        bp = rbuf;
        bytes_to_read = n;
        while ((i = read(sd, bp, bytes_to_read)) > 0)
        {
            bp += i;
            bytes_to_read -= i;
        }
        write(1, rbuf, n);
        printf("Transmit: \n");
        memset(sbuf, 0, BUFLEN);
        memset(rbuf, 0, BUFLEN);
    }
    close(sd);
}