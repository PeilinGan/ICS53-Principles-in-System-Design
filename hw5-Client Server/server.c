#include <unistd.h> 
#include <stdio.h> 
#include <sys/socket.h> 
#include <stdlib.h> 
#include <netinet/in.h> 
#include <string.h> 

int main(int argc, char *argv[]) {
    int server_fd, new_socket;
    struct sockaddr_in server_address;
    int server_address_len = sizeof(server_address);
    int port_num = atoi(argv[1]);

    printf("port is %d\n", port_num);
    return 0;



    if ((server_fd = socket(AF_INET, SOCK_STREAM, 0)) == 0) 
    { 
        perror("socket failed"); 
        exit(EXIT_FAILURE); 
    } 

    server_address.sin_family = AF_INET; 
    server_address.sin_addr.s_addr = INADDR_ANY; 
    server_address.sin_port = htons(port_num);

    if (bind(server_fd, (struct sockaddr *)&server_address, sizeof(server_address_len)) < 0) {
        perror("bind failed");
        exit(EXIT_FAILURE);
    }

    if (listen(server_fd, 3) < 0) {
        perror("listen");
        exit(EXIT_FAILURE);
    }
    
    if ((new_socket = accept(server_fd, (struct sockaddr *)&server_address, (socklen_t*)&server_address_len)) < 0) {
        perror("accept");
        exit(EXIT_FAILURE);
    }



    close(server_fd);
    return 0;
}