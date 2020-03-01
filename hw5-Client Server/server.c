#include <unistd.h>
#include <stdio.h>
#include <sys/socket.h>
#include <stdlib.h>
#include <netinet/in.h>
#include <string.h>
int MAXLINE = 200;

void echo(int connfd)
{
    size_t n;
    printf("server received %d bytes\n", (int)n);
    char buf[MAXLINE];
    while ((n = read(connfd, buf, MAXLINE)) != 0)
    {
        write(connfd, buf, n);
    }
}

int main(int argc, char *argv[])
{
    int server_fd, new_socket;
    struct sockaddr_in server_address;
    int server_address_len = sizeof(server_address);
    int port_num = atoi(argv[2]);

    if ((server_fd = socket(AF_INET, SOCK_STREAM, 0)) == 0)
    {
        perror("socket failed");
        exit(EXIT_FAILURE);
    }

    server_address.sin_family = AF_INET;
    server_address.sin_addr.s_addr = INADDR_ANY;
    server_address.sin_port = htons(port_num);

    if (bind(server_fd, (struct sockaddr *)&server_address, server_address_len) < 0)
    {
        perror("bind failed");
        exit(EXIT_FAILURE);
    }

    if (listen(server_fd, 1) < 0)
    {
        perror("listen");
        exit(EXIT_FAILURE);
    }
    printf("The server is listening on port %d\n", port_num);

    if ((new_socket = accept(server_fd, (struct sockaddr *)&server_address, (socklen_t *)&server_address_len)) < 0)
    {
        perror("accept");
        exit(EXIT_FAILURE);
    }

    while (1)
    {
        // if ((new_socket = accept(server_fd, (struct sockaddr *)&server_address, (socklen_t *)&server_address_len)) < 0)
        // {
        //     perror("accept");
        //     exit(EXIT_FAILURE);
        // }
        break;
    }

    printf("here1\n");

    close(new_socket);
    close(server_fd);
    return 0;
}