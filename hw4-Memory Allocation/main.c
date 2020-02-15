#include <stdio.h>
#include <unistd.h>
#include <sys/types.h>
#include <fcntl.h>
#include <string.h>
#include <stdlib.h>
#include <stdbool.h>

int check_size(int header)
{
    return header >> 1;
}
int check_status(int header)
{
    return header & 1;
}

int set_header(int size, int status)
{
    if (status)
        return (size << 1) | 1;
    else
    {
        return size << 1;
    }
}

int main()
{
    int heap[127];
    heap[0] = 127 << 1;
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
            break;
        }
        if (!strcmp(tokens[0], "malloc"))
        {
            int best_addr = -100;
            int size = atoi(tokens[1]);
            int min = 10000;
            int address = 1;
            int header = heap[0];
            int block_size = check_size(header);
            int status = check_status(header);
            while (address < 127)
            {
                if (size <= (block_size - 1) && (block_size - size - 1) < min)
                {
                    best_addr = address;
                }
                address += (block_size);
                if (address < 127)
                {
                    header = heap[address - 1];
                    block_size = check_size(header);
                    status = check_status(header);
                }
            }
            heap[best_addr + size] = set_header(check_size(heap[best_addr - 1]) - size - 1, 0);
            heap[best_addr - 1] = set_header(size + 1, 1);
            printf("%d\n", best_addr);
        }

        if (!strcmp(tokens[0], "free"))
        {
            int addr = atoi(tokens[1]);
            int header = addr - 1;
            int size = check_size(heap[header]);
            int next_header = header + size;
            if (!check_status(heap[next_header]))
            {
                size += check_size(heap[next_header]);
            }
            heap[header] = set_header(size, 0);
        }

        if (!strcmp(tokens[0], "writemem"))
        {
            int addr = atoi(tokens[1]);
            for (int i = 0; i < strlen(tokens[2]); i++)
            {
                heap[addr + i] = *(tokens[2] + i);
            }
        }

        if (!strcmp(tokens[0], "printmem"))
        {
            int addr = atoi(tokens[1]);
            int size = atoi(tokens[2]);
            for (int i = 0; i < size - 1; i++)
            {
                printf("%x ", heap[addr + i]);
            }
            printf("%x\n", heap[addr + size - 1]);
        }

        if (!strcmp(tokens[0], "blocklist"))
        {
            int address = 1;
            int header = heap[0];
            int size = check_size(header);
            int status = check_status(header);
            while (address < 127)
            {
                if (status == 1)
                    printf("%d, %d, allocated\n", address, size - 1);
                else if (status == 0)
                    printf("%d, %d, free\n", address, size - 1);
                address += size;
                if (address < 127)
                {
                    header = heap[address - 1];
                    size = check_size(header);
                    status = check_status(header);
                }
            }
        }
    }
}