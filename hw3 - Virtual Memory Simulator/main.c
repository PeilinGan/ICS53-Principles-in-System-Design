#include <stdio.h>
#include <unistd.h>
#include <sys/types.h>
#include <fcntl.h>
#include <string.h>
#include <stdlib.h>
#include <stdbool.h>

int FIFO = 0;
int LRU = 1;

void updateMainMemoryPriority(int *mainMemoryPriority, int pageNum)
{
    bool findValue = false;
    int i;
    for (i = 0; i < 4; i++)
    {
        if (findValue)
        {
            mainMemoryPriority[i - 1] = mainMemoryPriority[i];
        }
        else if (mainMemoryPriority[i] == pageNum)
        {
            findValue = true;
        }
    }
    mainMemoryPriority[3] = pageNum;
}

int findAvailablePage(int pageTable[8][3])
{
    int mainMemory[4] = {-1};
    for (int i = 0; i < 8; i++)
    {
        if (pageTable[i][0] == 1)
            mainMemory[pageTable[i][2]] = 1;
    }
    for (int i = 0; i < 4; i++)
    {
        if (mainMemory[i] != 1)
            return i;
    }
    return -1;
}

int main(int argc, char *argv[])
{
    int mainMemory[4][4][2];
    int diskMemory[8][4][2];
    int pageTable[8][3];
    int mainMemoryPriority[4] = {0, 1, 2, 3};
    int i, j;
    int replaceAlg = FIFO;

    if (argc == 2 && !strcmp(argv[1], "LRU"))
        replaceAlg = LRU;

    // mainMemory
    for (i = 0; i < 4; i++)
    {
        for (j = 0; j < 4; j++)
        {
            mainMemory[i][j][0] = i * 4 + j; // addr
            mainMemory[i][j][1] = -1;        // data
        }
    }
    // diskMemory
    for (i = 0; i < 8; i++)
    {
        for (j = 0; j < 4; j++)
        {
            diskMemory[i][j][0] = i * 4 + j;
            diskMemory[i][j][1] = -1;
        }
    }
    // pageTable
    for (i = 0; i < 8; i++)
    {
        pageTable[i][0] = 0;
        pageTable[i][1] = 0;
        pageTable[i][2] = i;
    }

    while (true)
    {
        printf("prompt> ");
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
            printf("test quit\n");
            break;
        }

        if (!strcmp(tokens[0], "read"))
        {
            int virtualAddress = atoi(tokens[1]);

            int entry = (int)virtualAddress / 4;
            int offset = virtualAddress % 4;

            // page hit
            if (pageTable[entry][0] == 1)
            {
                printf("%d\n", mainMemory[pageTable[entry][2]][offset][1]);
                // if it is FIFO, don't update
                if (replaceAlg == LRU)
                    updateMainMemoryPriority(mainMemoryPriority, pageTable[entry][2]);
            }
            else
            {
                printf("A Page Fault Has Occurred\n");
                // get the first avaible page number
                int avaiablePage = findAvailablePage(pageTable);
                if (avaiablePage != -1)
                {
                    // update page table
                    pageTable[entry][0] = 1;
                    pageTable[entry][2] = avaiablePage;
                    printf("available page: %d\n", avaiablePage);

                    // update main memory

                    for (i = 0; i < 4; i++)
                    {
                        mainMemory[avaiablePage][i][1] = diskMemory[entry][i][1];
                    }
                    printf("%d\n", mainMemory[avaiablePage][offset][1]);
                    updateMainMemoryPriority(mainMemoryPriority, avaiablePage);
                }
                else
                {
                    int swapPage = mainMemoryPriority[0];

                    // change the page table's swapPage
                    for (i = 0; i < 8; i++)
                    {
                        if (pageTable[i][0] == 1 && pageTable[i][2] == swapPage)
                        {
                            // write value back to disk memory
                            if (pageTable[i][1] == 1)
                            {
                                for (j = 0; j < 4; j++)
                                {
                                    diskMemory[i][j][1] = mainMemory[swapPage][j][1];
                                }
                            }
                            pageTable[i][0] = 0;
                            pageTable[i][1] = 0;
                            pageTable[i][2] = i;
                            break;
                        }
                    }

                    // change the page table's current virtual address
                    pageTable[entry][0] = 1;
                    pageTable[entry][2] = swapPage;

                    // update main memory
                    for (i = 0; i < 4; i++)
                    {
                        mainMemory[swapPage][i][1] = diskMemory[entry][i][1];
                    }
                    printf("%d\n", mainMemory[swapPage][offset][1]);
                    updateMainMemoryPriority(mainMemoryPriority, swapPage);
                }
            }
        }

        if (!strcmp(tokens[0], "write"))
        {
            int virtualAddress = atoi(tokens[1]);
            int data = atoi(tokens[2]);

            int entry = (int)virtualAddress / 4;
            int offset = virtualAddress % 4;

            // page hit
            if (pageTable[entry][0] == 1)
            {
                mainMemory[pageTable[entry][2]][offset][1] = data; // write to main memory
                pageTable[entry][1] = 1;                           // update the dirty bit

                if (replaceAlg == LRU)
                {
                    updateMainMemoryPriority(mainMemoryPriority, pageTable[entry][2]);
                }
            }
            else
            {
                printf("A Page Fault Has Occurred\n");
                // get the first avaible page number
                int avaiablePage = findAvailablePage(pageTable);

                // still have available page on main memory
                if (avaiablePage != -1)
                {
                    // update page table
                    pageTable[entry][0] = 1;
                    pageTable[entry][2] = avaiablePage;
                    printf("available page: %d\n", avaiablePage);

                    // update main memory
                    for (i = 0; i < 4; i++)
                    {
                        mainMemory[avaiablePage][i][1] = diskMemory[entry][i][1];
                    }
                    mainMemory[avaiablePage][offset][1] = data; // write the value to the main memory
                    pageTable[entry][1] = 1;                    // update the dirty bit
                    updateMainMemoryPriority(mainMemoryPriority, avaiablePage);
                }
                else
                {
                    int swapPage = mainMemoryPriority[0];

                    // change the page table's swapPage
                    for (i = 0; i < 8; i++)
                    {
                        if (pageTable[i][0] == 1 && pageTable[i][2] == swapPage)
                        {
                            // write value back to disk memory
                            if (pageTable[i][1] == 1)
                            {
                                for (j = 0; j < 4; j++)
                                {
                                    diskMemory[i][j][1] = mainMemory[swapPage][j][1];
                                }
                            }
                            pageTable[i][0] = 0;
                            pageTable[i][1] = 0;
                            pageTable[i][2] = i;
                            break;
                        }
                    }

                    // change the page table's current virtual address
                    pageTable[entry][0] = 1;
                    pageTable[entry][2] = swapPage;

                    // update main memory
                    for (i = 0; i < 4; i++)
                    {
                        mainMemory[swapPage][i][1] = diskMemory[entry][i][1];
                    }

                    mainMemory[swapPage][offset][1] = data; // write the value to the main memory
                    pageTable[entry][1] = 1;                // update the dirty bit
                    updateMainMemoryPriority(mainMemoryPriority, swapPage);
                }
            }
        }

        if (!strcmp(tokens[0], "showmain"))
        {
            int ppn = atoi(tokens[1]);
            for (j = 0; j < 4; j++)
            {
                printf("%d:%d\n", mainMemory[ppn][j][0], mainMemory[ppn][j][1]);
            }
        }

        if (!strcmp(tokens[0], "showdisk"))
        {
            int dpn = atoi(tokens[1]);
            for (j = 0; j < 4; j++)
            {
                printf("%d:%d\n", diskMemory[dpn][j][0], diskMemory[dpn][j][1]);
            }
        }
        if (!strcmp(tokens[0], "showptable"))
        {
            for (i = 0; i < 8; i++)
            {
                printf("%d:%d:%d:%d\n", i, pageTable[i][0], pageTable[i][1], pageTable[i][2]);
            }
        }
    }

    return 1;
}