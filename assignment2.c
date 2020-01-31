#include <stdio.h>
#include <unistd.h>
#include <sys/types.h>
#include <fcntl.h>
#include <string.h>
#include <stdlib.h>
#include <stdbool.h>

int main()
{
    //    adventure();
    int pid_array[1000];
    int bg_child = 0;
    while (true)
    {
        printf("prompt: ");
        char inputCommand[250];
        gets(inputCommand);
        char *token_p;
        char *tokens[10];

        token_p = strtok(inputCommand, " ");
        int index = 0;
        tokens[0] = token_p;

        while (token_p != NULL)
        {
            index += 1;
            token_p = strtok(NULL, " ");
            tokens[index] = token_p;
        }
        if (!strcmp(tokens[0], "quit"))
        {
            // printf("quit");
            break;
        }

        int pid;
        if ((pid = fork()) == 0)
        {
            for (int i = index - 1; i >= 0; i--)
            {
                if (strcmp(tokens[i], ">") == 0)
                {
                    //output redirect
                    int fd1;
                    if ((fd1 = creat(tokens[i + 1], 0644)) < 0)
                    {
                        perror("Couldn't open the output file");
                        exit(0);
                    }

                    dup2(fd1, STDOUT_FILENO); // 1 here can be replaced by STDOUT_FILENO
                    close(fd1);

                    printf("output file: %s\n", tokens[i + 1]);
                }
                if (strcmp(tokens[i], "<") == 0)
                {
                    //input
                    printf("input file: %s\n", tokens[i + 1]);

                    // fdo is file-descriptor
                    int fd0;
                    if ((fd0 = open(tokens[i + 1], O_RDONLY, 0)) < 0)
                    {
                        perror("Couldn't open input file");
                        exit(0);
                    }
                    // dup2() copies content of fdo in input of preceeding file
                    dup2(fd0, 0); // STDIN_FILENO here can be replaced by 0
                    close(fd0);   // necessary
                    // gets(inputCommand);
                    // printf("this is the command: %s \n", inputCommand);
                }
            }
            char *argv[] = {tokens[0], NULL};
            execvp(argv[0], argv);
            exit(0);
        }

        //foreground, wait for child process
        else if (strcmp(tokens[index - 1], "&") == 0)
        {
            pid_array[bg_child] = pid;
            bg_child++;
        }
        else
        {
            wait(&pid);
        }

        //background, do nothing just continue runing.
    }

    return 1;
}