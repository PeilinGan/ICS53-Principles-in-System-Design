#include <unistd.h>
#include <stdio.h>
#include <sys/socket.h>
#include <stdlib.h>
#include <netinet/in.h>
#include <string.h>
int MAXLINE = 200;
int DBMAXLINE = 400;

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

char* findArray(struct database *db, char game_id[], char field[]) {
    int i;
    for (i = 0; i < DBMAXLINE; i++) {
        if (strcmp(db[i].type, "None") == 0) 
            return "unknown"; // can't find

        if (strcmp(db[i].game_id, game_id) == 0) {
            if (strcmp(field, "type") == 0) 
                return db[i].type;
            else if (strcmp(field, "game_id") == 0)
                return db[i].game_id;
            else if (strcmp(field, "home_team") == 0)
                return db[i].home_team;
            else if (strcmp(field, "away_team") == 0)
                return db[i].away_team;
            else if (strcmp(field, "week") == 0)
                return db[i].week;
            else if (strcmp(field, "season") == 0)
                return db[i].season;
            else if (strcmp(field, "home_score") == 0)
                return db[i].home_score;
            else if (strcmp(field, "away_score") == 0)
                return db[i].away_score;
            else   
                return "unknown";
        }
    }
    return "unknown";
}

void echo(int connfd, struct database *db)
{

    size_t n;
    char buf[MAXLINE];
    while ((n = read(connfd, buf, MAXLINE)) != 0)
    {
        // printf("server received %d bytes\n", (int)n);
        
        // parse res 
        int i;
        int str_len = (int) buf[0];
        char client_res[str_len + 1];
        for(i = 0;i < str_len; i++){
            client_res[i] = buf[i+1];
        }
        client_res[i] = '\0';

        char * res = "";
        char *token_p;
        char *tokens[15];
        token_p = strtok(client_res, " ");
        int index = 0;
        tokens[0] = token_p;
        char format[256];
        while (token_p != NULL)
        {
            index += 1;
            token_p = strtok(NULL, " ");
            tokens[index] = token_p;     
        }
        if (index == 2) {
            tokens[1][strlen(tokens[1]) - 1] = 0;
            res = findArray(db, tokens[0], tokens[1]);
            int res_length = strlen(res);
            format[0] = (unsigned) res_length;
            format[1] = '\0';
            if(strcmp(res, "unknown")){
                printf("%s %s\n",tokens[0],tokens[1]);
            }
            strcat(format, res);
        }
        else {
            res = "unknown";
            format[0] = (unsigned) strlen(res);
            format[1] = '\0';
            strcat(format, res);
        }

        write(connfd, format, strlen(format));
    }
}


int main(int argc, char *argv[])
{
    int server_fd, new_socket, clientlen;
    struct sockaddr_in server_address;
    int server_address_len = sizeof(server_address);
    int port_num = atoi(argv[2]);
    int i = 0;
    // read_csv
    FILE *fp = fopen(argv[1], "r");
    char buf[1024];
    struct database db[DBMAXLINE];

    if (!fp)
    {
        printf("Can't open file\n");
        return 1;
    }

    while (fgets(buf, 1024, fp))
    {
        if (i == 0) {
            i++;
            continue;
        }
        // printf("%s\n", buf);
        char *token = strtok(buf, ",");
        strcpy(db[i].type, token);
        token = strtok(NULL, ",");
        strcpy(db[i].game_id, token);
        token = strtok(NULL, ",");
        strcpy(db[i].home_team, token);
        token = strtok(NULL, ",");
        strcpy(db[i].away_team, token);
        token = strtok(NULL, ",");
        strcpy(db[i].week, token);
        token = strtok(NULL, ",");
        strcpy(db[i].season, token);
        token = strtok(NULL, ",");
        strcpy(db[i].home_score, token);
        token = strtok(NULL, ",");
        strcpy(db[i].away_score, token);
        db[i].away_score[strlen(db[i].away_score) - 1] = 0;
        i++;
    }
    strcpy(db[i].type, "None");
    fclose(fp);

    // Socker Setup
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
    printf("server started\n");


    while (1)
    {
        clientlen = sizeof(struct sockaddr_storage);
        if ((new_socket = accept(server_fd, (struct sockaddr *)&server_address, (socklen_t *)&server_address_len)) < 0)
        {
            perror("accept");
            exit(EXIT_FAILURE);
        }
        echo(new_socket, db);
        close(new_socket);
    }
    close(server_fd);
    return 0;
}