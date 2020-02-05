# include <stdio.h>
# include <string.h>
# include <stdlib.h>
# include <stdbool.h> 

struct dungeon {
    char roomNum[5], description[101], northRoom[5], southRoom[5], eastRoom[5], westRoom[5];
};

struct dungeon *loadFile(char fileName[100]) {
    FILE *inFile;
    char dungeonInput[121];

    const int sizeofArray = 101;
    // need to have extra one to tell that
    struct dungeon *returnArray = (struct dungeon*) malloc(sizeof(struct dungeon) * sizeofArray);

    // load the file
    inFile = fopen(fileName, "r");
    int fileLine = 0;
    while (fgets(dungeonInput, sizeof(dungeonInput), inFile) != NULL) {
        if (dungeonInput[0] == '\n') {
            continue;
        }
        int i = 0;
        bool emptyLine = true;
        char roomNum[5], description[101], northRoom[5], southRoom[5], eastRoom[5], westRoom[5];

        char *token = strtok(dungeonInput, "+");
        while(token) {
            if (i == 0) {
                strcpy(roomNum, token);
                roomNum[strlen(roomNum)-1] = 0;
                emptyLine = false;
            } else if (i == 1) {
                strcpy(description, token);
            } else if (i == 2) {
                char roomTokenCopy[30];
                char *roomToken;
                strcpy(roomTokenCopy, token);
                roomToken = strtok(roomTokenCopy, " ");
                int j = 0;
                while (true) {
                    strcpy(northRoom, roomToken);
                    roomToken = strtok(NULL, " ");
                    strcpy(southRoom, roomToken);
                    roomToken = strtok(NULL, " ");
                    strcpy(eastRoom, roomToken);
                    roomToken = strtok(NULL, "\n");
                    strcpy(westRoom, roomToken);
                    break;
                }
            }
            i++;
            token = strtok(NULL, "+");
        }
        if (emptyLine == false) {
            strcpy(returnArray[fileLine].roomNum, roomNum);
            strcpy(returnArray[fileLine].description, description);
            strcpy(returnArray[fileLine].northRoom, northRoom);
            strcpy(returnArray[fileLine].southRoom, southRoom);
            strcpy(returnArray[fileLine].eastRoom, eastRoom);
            strcpy(returnArray[fileLine].westRoom, westRoom);
            fileLine++;
        }
    }
    fclose(inFile);

    strcpy(returnArray[fileLine].roomNum, "-2");
    strcpy(returnArray[fileLine].description, "It is not a house");
    strcpy(returnArray[fileLine].northRoom, "-1");
    strcpy(returnArray[fileLine].southRoom, "-1");
    strcpy(returnArray[fileLine].eastRoom, "-1");
    strcpy(returnArray[fileLine].westRoom, "-1");

    return returnArray;

}

int arraySize(struct dungeon *dungeonHouseArray) {
    int i = 0;
    for (i = 0; i < sizeof(dungeonHouseArray); i++) {
        if (strcmp(dungeonHouseArray[i].roomNum, "-2") == 0) {
            return i+1;
        }
    }
    return i;
}

int findRoom(struct dungeon *dungeonHouseArray, char roomNumStr[]) {
    int i = 0;

    for (i = 0; i < sizeof(dungeonHouseArray); i++) {
        if (strcmp(dungeonHouseArray[i].roomNum, roomNumStr) == 0) {
            return i;
        }
    }
    return i;
}

void adventure() {
    char inputCommand[100] = "default one";
    char fileName[100];
    struct dungeon *dungeonHouseArray;
    int position = 0;

    while (true) {
        printf("$ ");
        gets(inputCommand);
        char *command;
        if (strncmp("loaddungeon", inputCommand, strlen("loaddungeon")) == 0) {
            command = strtok(inputCommand, " ");
            command = strtok(NULL, " ");
            strcpy(fileName, command);

            dungeonHouseArray = loadFile(fileName); 

            if (arraySize(dungeonHouseArray) == 1) {
                printf("It is a empty file. Please load it again\n");
            } else {
                printf("%s\n", dungeonHouseArray[position].description);
                break;
            }
        } else if (strcmp(inputCommand, "quit") == 0) {
            return ;
        } else {
            printf("this is a wrong command\n");
        }

    }
    
    while (strcmp(inputCommand, "quit") != 0) { 
        printf("$ ");
        gets(inputCommand);

        if (strcmp(inputCommand, "north") == 0) {

            if (strcmp(dungeonHouseArray[position].northRoom, "-1") == 0) {
                printf("You can't go there.\n");
            } else {
                position = findRoom(dungeonHouseArray, dungeonHouseArray[position].northRoom);
                printf("%d %s\n", position + 1,dungeonHouseArray[position].description);
            } 

        } else if (strcmp(inputCommand, "south") == 0) {

            if (strcmp(dungeonHouseArray[position].southRoom, "-1") == 0) {
                printf("You can't go there.\n");
            } else {
                position = findRoom(dungeonHouseArray, dungeonHouseArray[position].southRoom);
                printf("%d %s\n", position + 1,dungeonHouseArray[position].description);
            } 

        } else if (strcmp(inputCommand, "east") == 0) {

            if (strcmp(dungeonHouseArray[position].eastRoom, "-1") == 0) {
                printf("You can't go there.\n");
            } else {
                position = findRoom(dungeonHouseArray, dungeonHouseArray[position].eastRoom);
                printf("%d %s\n", position + 1,dungeonHouseArray[position].description);
            } 

        } else if (strcmp(inputCommand, "west") == 0) {

            if (strcmp(dungeonHouseArray[position].westRoom, "-1") == 0) {
                printf("You can't go there.\n");
            } else {
                position = findRoom(dungeonHouseArray, dungeonHouseArray[position].westRoom);
                printf("%d %s\n", position + 1,dungeonHouseArray[position].description);
            } 

        } else if (strcmp(inputCommand, "quit") == 0) {
            break;  
        } else if (strncmp("loaddungeon", inputCommand, strlen("loaddungeon")) == 0) {
            printf("can not load multiple times\n");
        } else {
            printf("this is a wrong command\n");
        }
        
    }

    free(dungeonHouseArray);
}

int main() {
   printf("hello\n");
   return 1;
}