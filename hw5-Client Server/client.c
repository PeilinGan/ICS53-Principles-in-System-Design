#include <unistd.h>
#include <stdio.h>
#include <sys/socket.h>
#include <stdlib.h>
#include <netinet/in.h>
#include <netdb.h>
#include <string.h>
#include <arpa/inet.h>

int MAXLINE = 200;

int main(int argc, char *argv[])
{
    int client_socket;
    struct sockaddr_in server_address;
    struct hostent *hp;
    struct in_addr ip_addr;

    // memset(&server_address, '0', sizeof(server_address));

    // int server_address_len = sizeof(server_address);

    int port_num = atoi(argv[2]);

    if ((client_socket = socket(AF_INET, SOCK_STREAM, 0)) < 0)
    {
        perror("socket failed");
        exit(EXIT_FAILURE);
    }


    server_address.sin_family = AF_INET;
    server_address.sin_port = htons(port_num);

    
    hp = gethostbyname(argv[1]);
    ip_addr = *(struct in_addr *)(hp->h_addr);
    char * ip_address = inet_ntoa(ip_addr);
    if (inet_pton(AF_INET, ip_address, &server_address.sin_addr) <= 0)
    {
        printf("\nInvalid address/ Address not supported \n");
        return -1;
    }


    // server_address.sin_addr.s_addr = INADDR_ANY;
    if (connect(client_socket, (struct sockaddr *)&server_address, sizeof(server_address)) < 0)
    {
        printf("\nConnection Failed \n");
        exit(EXIT_FAILURE);
    }

    size_t n;
    char buf[MAXLINE];
    while (fgets(buf, MAXLINE, stdin) != NULL)
    {
        char format[256];
        format[0] = (unsigned) strlen(buf);
        format[1] = '\0';
        strcat(format,buf);

        write(client_socket, format, strlen(format));
        read(client_socket, buf, MAXLINE);
        int i;
        int str_len = (int) buf[0];
        char res[str_len + 1];
        for(i = 0;i < str_len; i++){
            res[i] = buf[i+1];
        }
        res[i] = '\0';
        fputs(res, stdout);
        printf("\n");
    }

    close(client_socket);
    return 0;
}