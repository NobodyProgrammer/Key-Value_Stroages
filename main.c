#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <unistd.h>
#include <string.h>
#include <math.h>
#include <stdbool.h>
int size = 1000000;
struct myMemory
{
    /* data */
    long long int key;
    char value[255];
};
typedef struct myMemory myMemory;
void mapToDataBase(myMemory *memory, int m_count)
{
    char *output_file = malloc(sizeof(char) * 30);
    FILE *output;
    char *string = malloc(sizeof(char) * 255);
    char *buffer[size];
    for (int i = 0; i < size; ++i)
    {
        buffer[i] = malloc(sizeof(char) * 255);
    }
    for (int i = m_count - 1; i >= 0; --i)
    {
        //printf("%lld %s", memory[i].key, memory[i].value);
        sprintf(string, "%lld %s", memory[i].key, memory[i].value);

        long long int file_num = (memory[i].key + 1) / pow(2, 20); //map to the relative file base on the key number
        sprintf(output_file, "./storage/%lld", file_num);

        if (output = fopen(output_file, "r"))
        { //open file in specific key
            int c = 0;
            bool isFindKey = false;
            while (!feof(output))
            {
                fgets(buffer[c], 255, output);
                ++c;
                //printf("%s\n", buffer[c - 1]);
            }
            fclose(output);
            for (int j = 0; j < c; ++j)
            {
                char *temp = malloc(sizeof(char) * 255);
                strcpy(temp, buffer[j]);
                char *token = strtok(temp, " ");
                char *key = malloc(sizeof(char) * 30);
                while (token != NULL)
                {
                    /* code */
                    strcpy(key, token);
                    break;
                    //token = strtok(NULL, " ");
                }
                //find the same key and change
                if (memory[i].key == atoll(key))
                {
                    //printf("match\n");
                    sprintf(buffer[j], "%lld %s", memory[i].key, memory[i].value);
                    isFindKey = true;
                }
            }
            if (!isFindKey)
            {
                sprintf(buffer[c], "%lld %s", memory[i].key, memory[i].value);
                ++c;
                //printf("not match!\n");
            }
            //printf("c=%d\n", c);
            output = fopen(output_file, "w");
            for (int j = 0; j < c; ++j)
            {
                //printf("j=%d string=%s\n", c, buffer[j]);
                fputs(buffer[j], output);
            }
            fclose(output);
        }
        //file not exist,create file
        else
        {
            //printf("not exist!\n");
            output = fopen(output_file, "w");
            fputs(string, output);
            fclose(output);
        }
        //printf("-----------------\n");
    }
}
void binarySearch()
{
}
void Swap(myMemory *a, myMemory *b)
{
    myMemory temp = *a;
    *a = *b;
    *b = temp;
}
int Partiton(myMemory *memory, int begin, int end)
{
    int pivot_idx = begin - 1;
    long long int pivot = memory[end].key;
    for (int i = begin; i < end; ++i)
    {
        if (memory[i].key < pivot)
        {
            pivot_idx++;
            Swap(&memory[i], &memory[pivot_idx]);
        }
    }
    pivot_idx++;
    Swap(&memory[pivot_idx], &memory[end]);
    return pivot_idx;
}
void sortPut(myMemory *memory, int begin, int end)
{
    if (begin < end)
    {
        int middle = Partiton(memory, begin, end);
        sortPut(memory, begin, middle - 1);
        sortPut(memory, middle + 1, end);
    }
}
void exeInstruction(char instr[][255], myMemory *memory, int *m_count)
{
    if (strcmp(instr[0], "PUT") == 0)
    {
        //printf("PUT\n");
        long long int k = atoll(instr[1]);
        memory[*m_count].key = k;
        strcpy(memory[*m_count].value, instr[2]);

        *m_count += 1;
        if (*m_count >= size)
        {
            mapToDataBase(memory, *m_count);
            *m_count = 0;
        }
    }
    else if (strcmp(instr[0], "GET") == 0)
    {
    }
    else
    {
    }
}

int main()
{
    char *folder = "storage";
    char *input_file = "1.input";
    mkdir(folder, 0777);
    FILE *input = fopen(input_file, "r");

    myMemory *memory = malloc(sizeof(myMemory) * size);
    int m_count = 0;
    while (!feof(input))
    {
        /* code */
        char *input_string = malloc(sizeof(char) * 255);
        fgets(input_string, 255, input);
        char *token = strtok(input_string, " ");
        char instr[3][255];
        int count = 0;
        while (token != NULL)
        {
            /* code */
            strcpy(instr[count++], token);
            token = strtok(NULL, " ");
        }
        exeInstruction(instr, memory, &m_count);
    }
    //sortPut(memory, 0, m_count - 1);
    //printf("successful");

    //printf("%d",m_count);
    fclose(input);
}