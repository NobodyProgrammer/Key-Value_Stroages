#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <unistd.h>
#include <string.h>
#include <math.h>
#include <stdbool.h>
int size = 1000000;
int out_buffer_size = 100000;
int per = 2;
char *output_file = "1.output";
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

    char *buffer[size];
    for (int i = 0; i < size; ++i)
    {
        buffer[i] = malloc(sizeof(char) * 255);
    }
    for (int i = m_count - 1; i >= 0; --i)
    {
        long long int file_num = (memory[i].key + 1) / size; //map to the relative file base on the key number
        sprintf(output_file, "./storage/%lld", file_num);
        if (output = fopen(output_file, "r"))
        {
            int c = 0;
            bool isFindKey = false;

            while (!feof(output))
            {
                fgets(buffer[c], 255, output);
                int len = strlen(buffer[c]);
                int end = len - 1;
                if (buffer[c][end] != '\n')
                {
                    buffer[c][end + 1] = '\n';
                    buffer[c][end + 2] = '\0';
                }
                char key[30]; //get key from buffer

                for (int k = 0; k < len; ++k)
                {
                    if (buffer[c][k] == ' ')
                    {
                        strncpy(key, buffer[c], k);
                        break;
                    }
                }
                if (memory[i].key == atoll(key))
                {
                    sprintf(buffer[c], "%lld %s", memory[i].key, memory[i].value);
                    isFindKey = true;
                }
                ++c;
            }
            fclose(output);
            if (!isFindKey)
            {

                //insert key into database
                binaryInsert(buffer, c, memory[i].key, memory[i].value);
                //sprintf(buffer[c], "%lld %s", memory[i].key, memory[i].value);
                ++c;
            }
            output = fopen(output_file, "w");
            for (int j = 0; j < c; ++j)
            {
                if (j == c - 1)
                {
                    int end = strlen(buffer[j]) - 1;
                    buffer[j][end] = '\0';
                }
                fputs(buffer[j], output);
            }
            fclose(output);
        }
        else
        {
            output = fopen(output_file, "w");

            char *string = malloc(sizeof(char) * 155);
            sprintf(string, "%lld %s", memory[i].key, memory[i].value);
            int end = strlen(string) - 1;
            string[end] = '\0';
            fputs(string, output);
            fclose(output);
            free(string);
        }
    }

    for (int i = 0; i < size; ++i)
    {
        free(buffer[i]);
    }
}
void writeToOutput()
{
}
char *getFromDataBase(myMemory *memory, int m_count, long long int compare_key)
{

    //first see memory
    long long int file_num;
    FILE *input;
    char *file = malloc(sizeof(char) * 30);
    char *buffer = malloc(sizeof(char) * 153);
    for (int i = m_count - 1; i >= 0; --i)
    {
        //i just read from the end to begin,so it is the newest
        if (memory[i].key == compare_key)
        {
            free(file);
            free(buffer);
            return memory[i].value;
        }
    }
    //then see the database
    file_num = (compare_key + 1) / size; //map to the relative file base on the key number
    sprintf(file, "./storage/%lld", file_num);
    if (input = fopen(file, "r"))
    {
        while (!feof(input))
        {
            fgets(buffer, 255, input);
            int len = strlen(buffer);
            int c = 0;
            char *key_value[2];
            char *token = strtok(buffer, " ");
            key_value[0] = malloc(sizeof(char) * 30);
            key_value[1] = malloc(sizeof(char) * 130);
            while (token != NULL)
            {
                strcpy(key_value[c++], token);
                token = strtok(NULL, " ");
            }
            if (compare_key == atoll(key_value[0]))
            {
                int end = strlen(key_value[1]) - 1;
                if (key_value[1][end] != '\n')
                {
                    key_value[1][end + 1] = '\n';
                    key_value[1][end + 2] = '\0';
                }
                free(file);
                free(buffer);
                free(key_value[0]);
                fclose(input);
                return key_value[1];
            }
            free(key_value[0]);
            free(key_value[1]);
        }
        free(file);
        free(buffer);
        fclose(input);
        return "EMPTY!\n";
    }
    else
    {
        free(file);
        free(buffer);
        fclose(input);
        return "EMPTY!\n";
    }
}
void scanFromeDatabase(myMemory *memory, int m_count, long long int begin, long long int end, char **out_buffer, int *o_count)
{
    FILE *input;
    long long int file_num;
    int range = end - begin + 1;
    char *buffer[range];
    char *input_string = malloc(sizeof(char) * 130);
    char *file = malloc(sizeof(char) * 30);
    char *key_value[2];
    for (int i = 0; i < range; ++i)
    {
        buffer[i] = malloc(sizeof(char) * 153);
        strcpy(buffer[i], "EMPTY!\n");
    }

    /*find the range of file we need to search*/
    file_num = (begin + 1) / size;
    sprintf(file, "./storage/%lld", file_num);
    if (input = fopen(file, "r"))
    {
        while (!feof(input))
        {
            fgets(input_string, 255, input);
            int c = 0;
            char *token = strtok(input_string, " ");
            key_value[0] = malloc(sizeof(char) * 30);
            key_value[1] = malloc(sizeof(char) * 130);
            while (token != NULL)
            {
                strcpy(key_value[c++], token);
                token = strtok(NULL, " ");
            }
            long long int key = atoll(key_value[0]);
            if (key >= begin && key <= end)
            {

                //find the key in file base on my scan range,store in my buffer
                int idx = key - begin;
                strcpy(buffer[idx], key_value[1]);
                int last = strlen(buffer[idx]) - 1;
                if (buffer[idx][last] != '\n')
                {
                    buffer[idx][last + 1] = '\n';
                    buffer[idx][last + 2] = '\0';
                }
            }
            free(key_value[0]);
            free(key_value[1]);
        }
    }
    fclose(input);
    //see memory because memory is newest;
    for (int i = 0; i < m_count; ++i)
    {
        long long int key = memory[i].key;
        if (key >= begin && key <= end)
        {
            int idx = key - begin;
            strcpy(buffer[idx], memory[i].value);
            int end = strlen(buffer[idx]) - 1;
            if (buffer[idx][end] != '\n')
            {
                buffer[idx][end + 1] = '\n';
                buffer[idx][end + 2] = '\0';
            }
        }
    }

    for (int i = 0; i < range; ++i)
    {
        printf("%s", buffer[i]);
        if (*o_count >= out_buffer_size)
        {
            writeToOutput(out_buffer, *o_count);
            *o_count = 0;
        }
    }
}
void exeInstr(char **instr, myMemory *memory, int *m_count, char **out_buffer, int *out_count)
{
    if (strcmp(instr[0], "PUT") == 0)
    {
        long long int k = atoll(instr[1]);
        memory[*m_count].key = k;

        int end = strlen(instr[2]) - 1;
        //this is the last in the input file,just add '\n' in every instrution
        if (instr[2][end] != '\n')
        {
            instr[2][end + 1] = '\n';
            instr[2][end + 2] = '\0';
        }
        strcpy(memory[*m_count].value, instr[2]);

        *m_count += 1;
        if (*m_count >= per)
        {
            mapToDataBase(memory, *m_count);
            *m_count = 0;
        }
    }
    else if (strcmp(instr[0], "GET") == 0)
    {

        long long int key = atoll(instr[1]);
        if (*out_count >= out_buffer_size)
        {
            writeToOutput(out_buffer, *out_count);
            *out_count = 0;
        }
        char *value = getFromDataBase(memory, *m_count, key);
        strcpy(out_buffer[*out_count], value);
        printf("%s", out_buffer[*out_count]);
        ++out_count;
    }
    else
    {
        //scan range may be large,so i print the answer as soon as i find key,not sotre in memory

        //printf("%lld %lld", atoll(instr[1]), atoll(instr[2]));
        long long int begin = atoll(instr[1]);
        long long int end = atoll(instr[2]);
        scanFromeDatabase(memory, *m_count, begin, end, out_buffer, &out_count);
    }
}
int main()
{
    char *folder = "storage";
    char *input_file = "3.input";
    char *input_string = malloc(sizeof(char) * 255);
    char *instr[3];
    char *out_buffer[out_buffer_size];
    FILE *input = fopen(input_file, "r");
    myMemory *memory = malloc(sizeof(myMemory) * size);
    int m_count = 0;
    int out_count = 0;
    mkdir(folder, 0777);
    for (int i = 0; i < out_buffer_size; ++i)
        out_buffer[i] = malloc(sizeof(char) * 130);
    for (int i = 0; i < 3; ++i)
        instr[i] = malloc(sizeof(char) * 130);
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
        exeInstr(instr, memory, &m_count, out_buffer, &out_count);
    }
    fclose(input);
}