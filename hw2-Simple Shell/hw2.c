#include <stdio.h>
#include <unistd.h>
#include <sys/types.h>
#include <fcntl.h>
#include <string.h>
#include <stdlib.h>
#include <stdbool.h>

int main()
{
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
            int status;
            for (int i = 0; i < bg_child; i++)
            {
                kill(pid_array[i], SIGINT);
            }
            for (int i = 0; i < bg_child; i++)
            {
                int wpid = wait(&status);
            }
            break;
        }

        int pid;
        if ((pid = fork()) == 0)
        {
            for (int i = index - 1; i >= 0; i--)
            {
                if (strcmp(tokens[i], ">") == 0)
                {

                    int fd1;
                    if ((fd1 = creat(tokens[i + 1], 0644)) < 0)
                    {
                        perror("Couldn't open the output file");
                        exit(0);
                    }

                    dup2(fd1, STDOUT_FILENO);
                    close(fd1);

                    printf("output file: %s\n", tokens[i + 1]);
                }
                if (strcmp(tokens[i], "<") == 0)
                {
                    int fd0;
                    if ((fd0 = open(tokens[i + 1], O_RDONLY, 0)) < 0)
                    {
                        perror("Couldn't open input file");
                        exit(0);
                    }
                    dup2(fd0, 0);
                    close(fd0);
                }
            }
            char *argv[] = {tokens[0], NULL};
            execv(argv[0], argv);
            exit(0);
        }
        else if (strcmp(tokens[index - 1], "&") == 0)
        {
            pid_array[bg_child] = pid;
            bg_child++;
        }
        else
        {
            wait(&pid);
        }
    }

    return 1;
}