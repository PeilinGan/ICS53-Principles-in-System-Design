# include <stdio.h>
# include <unistd.h>
# include <sys/types.h>
# include <string.h>
# include <stdlib.h>
# include <stdbool.h> 


void adventure() {
    char inputCommand[100] = "default one";

    
    while (true) { 
        printf("prompt> ");
        gets(inputCommand);
        
        // quit command
        if (strcmp(inputCommand, "quit") == 0) {
            return;
        } 
        
        // executable command
        else {
            if (fork() == 0) {
                char const* argv[] = {"./hello", NULL};
                execv(argv[0], argv);
            }
        }

    }

}

int main() {
   adventure();
   return 1;
}