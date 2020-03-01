#include <unistd.h> 
#include <stdio.h> 
// #include <sys/socket.h> 
#include <stdlib.h> 
// #include <netinet/in.h> 
#include <string.h> 

int main(int argc, char *argv[]) {
    int server_fd;
    int portNum = atoi(argv[2]);

    printf("port is %d\n", portNum);
    return 0;



    if ((server_fd = socket(AF_INET, SOCK_STREAM, 0)) == 0) 
    { 
        perror("socket failed"); 
        exit(EXIT_FAILURE); 
    } 

    address.sin_family = AF_INET; 
    address.sin_addr.s_addr = INADDR_ANY; 
    address.sin_port = htons( portNum );

    if (bind(server_fd, (struct sockaddr *)&address, sizeof(address))<0) {
            perror("bind failed");
            exit(EXIT_FAILURE);
    }

    close(server_fd);
    return 0;
}