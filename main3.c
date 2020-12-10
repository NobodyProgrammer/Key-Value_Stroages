#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <unistd.h>
#include <string.h>
#include <math.h>
#include <stdbool.h>
int size = 10;
struct myMemory
{
    /* data */
    long long int key;
    char value[255];
};
typedef struct myMemory myMemory;
void binaryInsert(char **buffer, int s, long long int compare_key, char *value)
{
    bool isMax = true;
    for (int i = 0; i < s; ++i)
    {
        long long int k = atoll(buffer[i]);
        if (compare_key < k)
        {
            isMax = false;
            for (int j = s; j > i; --j)
            {
                strcpy(buffer[j], buffer[j - 1]);
            }
            sprintf(buffer[i], "%lld %s", compare_key, value);
            break;
        }
    }
    if (isMax)
        sprintf(buffer[s], "%lld %s", compare_key, value);
}
void mapToDataBase(myMemory *memory, int m_count)
{
    char *output_file = malloc(sizeof(char) * 30);
    FILE *output;
    char *string = malloc(sizeof(char) * 255);
    char *buffer[m_count];
    for (int i = 0; i < size; ++i)
    {
        buffer[i] = malloc(sizeof(char) * 255);
    }
    for (int i = m_count - 1; i >= 0; --i)
    {
        long long int file_num = (memory[i].key + 1) / pow(2, 20); //map to the relative file base on the key number
        sprintf(output_file, "./storage/%lld", file_num);
        if (output = fopen(output_file, "r"))
        {
            int c = 0;
            bool isFindKey = false;
            while (!feof(output))
            {
                fgets(buffer[c], 255, output);
                ++c;
            }
            fclose(output);
            for (int j = 0; j < c; ++j)
            {
                int len = strlen(buffer[j]);
                char key[30]; //get key from buffer
                for (int k = 0; k < len; ++k)
                {
                    if (buffer[j][k] == ' ')
                    {
                        strncpy(key, buffer[j], k);
                        break;
                    }
                }

                if (memory[i].key == atoll(key))
                {

                    sprintf(buffer[j], "%lld %s", memory[i].key, memory[i].value);

                    isFindKey = true;
                    break;
                }
            }
            if (!isFindKey)
            {

                //insert key into database
                //printf("not found\n");
                binaryInsert(buffer, c, memory[i].key, memory[i].value);
                //sprintf(buffer[c], "%lld %s", memory[i].key, memory[i].value);
                ++c;
                printf("%s", buffer[c]);
            }
            output = fopen(output_file, "w");
            for (int j = 0; j < c; ++j)
            {
                if (j != c - 1)
                    buffer[j][strlen(buffer[j]) - 1] = '\n';
                fputs(buffer[j], output);
            }
            //printf("%s\n", buffer[j]);
            //printf("\n");
            fclose(output);
        }
        else
        {
            output = fopen(output_file, "w");
            sprintf(string, "%lld %s", memory[i].key, memory[i].value);
            fputs(string, output);
            fclose(output);
        }
    }
}
void exeInstr(char **instr, myMemory *memory, int *m_count)
{
    if (strcmp(instr[0], "PUT") == 0)
    {
        long long int k = atoll(instr[1]);
        memory[*m_count].key = k;

        int len = strlen(instr[2]) - 1;
        if (instr[2][len] == '\n')
            instr[2][len] = '\0';
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
        printf("GET\n");
    }
    else
    {
    }
}
int main()
{
    char *folder = "storage";
    char *input_file = "3.input";
    char *input_string = malloc(sizeof(char) * 255);
    char *instr[3];
    for (int i = 0; i < 3; ++i)
        instr[i] = malloc(sizeof(char) * 130);
    mkdir(folder, 0777);
    FILE *input = fopen(input_file, "r");

    myMemory *memory = malloc(sizeof(myMemory) * size);
    int m_count = 0;
    while (!feof(input))
    {
        fgets(input_string, 255, input);
        char *token = strtok(input_string, " ");
        int count = 0;
        while (token != NULL)
        {
            /* code */
            strcpy(instr[count++], token);
            token = strtok(NULL, " ");
        }
        exeInstr(instr, memory, &m_count);
    }
    //sortPut(memory, 0, m_count - 1);
    //printf("successful");

    //printf("%d",m_count);
    fclose(input);
}