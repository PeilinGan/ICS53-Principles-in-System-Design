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
    char buf[MAXLINE];
    while ((n = read(connfd, buf, MAXLINE)) != 0)
    {
        printf("server received %d bytes\n", (int)n);
        write(connfd, buf, n);
    }
}

struct database
{
    char type[10];
    char game_id[20];
    char home_team[10];
    char away_team[10];
    char week[5];
    char season[5];
    char home_score[5];
    char away_score[5];

};

// char read_csv(char *filename)
// {
//     FILE *fp = fopen(filename, "r");
//     char buf[1024];

//     if (!fp)
//     {
//         printf("Can't open file\n");
//         return 1;
//     }

//     while (fgets(buf, 1024, fp))
//     {
//         printf("%s\n", buf);
//     }

//     fclose(fp);
// }


int main(int argc, char *argv[])
{
    int server_fd, new_socket, clientlen;
    struct sockaddr_in server_address;
    int server_address_len = sizeof(server_address);
    int port_num = atoi(argv[2]);
    // read_csv
    printf("%s\n", argv[1]);
    FILE *fp = fopen(argv[1], "r");
    char buf[1024];
    struct database db[400];
    if (!fp)
    {
        printf("Can't open file\n");
        return 1;
    }

    while (fgets(buf, 1024, fp))
    {

    }

    fclose(fp);

    if ((server_fd = socket(AF_INET, SOCK_STREAM, 0)) < 0)
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
    // listen_fd = server_fd
    if (listen(server_fd, 3) < 0)
    {
        perror("listen");
        exit(EXIT_FAILURE);
    }
    printf("The server is listening on port %d\n", port_num);

    while (1)
    {
        clientlen = sizeof(struct sockaddr_storage);
        if ((new_socket = accept(server_fd, (struct sockaddr *)&server_address, (socklen_t *)&server_address_len)) < 0)
        {
            perror("accept");
            exit(EXIT_FAILURE);
        }
        echo(new_socket);
        close(new_socket);
    }

    printf("end\n");

    close(server_fd);
    return 0;
}