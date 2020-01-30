# include <stdio.h>
# include <unistd.h>
# include <sys/types.h>
# include <string.h>
# include <stdlib.h>
# include <stdbool.h> 

int check_index(char * str, char dilimeter) {
    char * p = strchr(str, dilimeter);
    if(p == NULL && str[0] != '<')
        return -1;
    if(p == NULL)
        return -1;
    return (int) (p - str);
}

void adventure() {
    char inputCommand[100] = "default one";
    char * split_command; 
    char currentCommand[100] = "e";
    char previousCommand[100] = "e";
    printf("prompt> ");
    gets(inputCommand);
    int larrow = check_index(inputCommand,'<');
    int rarrow = check_index(inputCommand,'>');
    // printf("%d, %d\n", larrow, rarrow);
    // 判断arrow并且split一次，取出previous command
    if((larrow < 0) && (rarrow < 0)) {
        currentCommand[0] = 'e';
    } else if (larrow < 0) {
        currentCommand[0] = '>';
        printf("this is current command: %s\n", currentCommand);
        split_command = strtok(inputCommand, currentCommand);
    } else if (rarrow < 0) {
        currentCommand[0] = '<';
        printf("this is current command: %s\n", currentCommand);
        split_command = strtok(inputCommand, currentCommand);
    } else if (larrow < rarrow) {
        currentCommand[0] = '<';
        printf("this is current command: %s\n", currentCommand);
        split_command = strtok(inputCommand, currentCommand);
    }
    else if( larrow > rarrow){
        currentCommand[0] = '>';
        printf("this is current command: %s\n", currentCommand);
        split_command = strtok(inputCommand, currentCommand);
    }
    printf("%s\n",split_command);

    // 根据 previous command，执行当前的inputcommand
    if (strcmp(previousCommand, "e") == 0) {
        printf("exeute\n");
    }

    // 根据 current找到的arrow，在token一次
    if (strcmp(currentCommand, "e") == 0) {
        printf("get return\n");
        return;
    } else {
        //printf("current");
        split_command = strtok(NULL, currentCommand);
        printf("split_command %s",split_command);
    }
    
    strcpy(previousCommand, currentCommand);

    while(true){
        int larrow = check_index(split_command,'<');
        int rarrow = check_index(split_command,'>');

        // 判断arrow并且split一次，取出previous command
        if((larrow < 0) && (rarrow < 0)) {
            
        } else if (larrow < 0) {
            currentCommand[0] = '>';
            split_command = strtok(NULL, currentCommand);
        } else if (rarrow > 0) {
            currentCommand[0] = '<';
            split_command = strtok(NULL, currentCommand);
        } else if (larrow < rarrow) {
            currentCommand[0] = '<';
            split_command = strtok(NULL,currentCommand);
        }
        else if( larrow > rarrow){
            currentCommand[0] = '>';
            split_command = strtok(NULL, currentCommand);
        }
        printf("%s\n",split_command);

        // 根据 previous command，执行当前的inputcommand
        if (strcmp(previousCommand, "e") == 0) {
            printf("exeute\n");
        } else if (strcmp(previousCommand, ">") == 0) {
            printf("do the writing\n");
        } else if (strcmp(previousCommand, "<") == 0) {
            printf("do the reading\n");
        }

        // 根据 current找到的arrow，在token一次
        if (strcmp(currentCommand, "e") == 0) {
            printf("get return\n");
            return;
        } else {
            split_command = strtok(NULL, currentCommand);
        }
        
        strcpy(previousCommand, currentCommand);


        break;

    }

    
    // while (true) { 

    //     printf("prompt> ");
    //     gets(inputCommand);
        
    //     while(true)
    //     // quit command
    //     if (strcmp(inputCommand, "quit") == 0) {
    //         char * test = "heoll > ere.txt";
    //         // test = strtok(test,"<");
    //         int index;
    //         index = (int) (strchr(test,'>')- test);
    //         printf("%d",index);
    //         return;
    //     } 
        
    //     // executable command
    //     else {
    //         int pid;
    //         if ((pid = fork()) == 0) {
    //             char * argv[] = {inputCommand, NULL};
    //             execv(argv[0], argv);
    //             exit(0);
    //         }
    //         else{
    //              wait(&pid);
    //         }
    //     }

    // }

}

int main() {
   adventure();
   return 1;
}