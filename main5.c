#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <unistd.h>
#include <string.h>
#include <math.h>
#include <stdbool.h>
int size = 10000;
int out_buffer_size = 10000;
int folder_num = 0;
char *output_file;
struct stat st = {0};
struct myMemory
{
    /* data */
    long long int key;
    char value[130];
};
typedef struct myMemory myMemory;
void mapToDataBase(myMemory *memory, int m_count)
{
    char *db_name = malloc(sizeof(char) * 35);
    strcpy(db_name, "./storage");
    FILE *db;
    char *buffer = malloc(sizeof(char) * 153);
    char path[30];
    for (int i = 0; i < m_count; ++i)
    {
        long long int key = memory[i].key;
        long long int folder = key / size; //map to the relative folder/file base on the key number
        long long int file = key % size;
        sprintf(path, "/%lld", folder);
        strcat(db_name, path);
        if (stat(db_name, &st) == -1)
        {
            //printf("%d\n", ++folder_num);
            mkdir(db_name, 0777);
        }
        sprintf(path, "/%lld", file);
        strcat(db_name, path);
        if (db = fopen(db_name, "r"))
        {

            fgets(buffer, 255, db);
            int c = 0;
            char *temp = malloc(sizeof(char) * 153);
            char key[30];
            char value[130];
            strcpy(temp, buffer);
            char *token = strtok(temp, " ");
            while (token != NULL)
            {
                if (c == 0)
                {
                    strcpy(key, token);
                    ++c;
                }
                else
                    strcpy(value, token);

                token = strtok(NULL, " ");
            }

            if (memory[i].key == atoll(key))
                sprintf(buffer, "%lld %s", memory[i].key, memory[i].value);
            free(temp);

            fclose(db);
            db = fopen(db_name, "w");
            fputs(buffer, db);
            fclose(db);
        }
        else
        {
            db = fopen(db_name, "w");
            char *string = malloc(sizeof(char) * 153);
            sprintf(string, "%lld %s", memory[i].key, memory[i].value);
            fputs(string, db);
            fclose(db);
            free(string);
        }
        strcpy(db_name, "./storage");
    }
    free(db_name);
}
void writeToOutput(char **out_buffer, int o_count)
{
    FILE *output;
    if (output = fopen(output_file, "r"))
    {
        fclose(output);
        output = fopen(output_file, "a");
    }
    else
        output = fopen(output_file, "w");
    for (int i = 0; i < o_count; ++i)
        fputs(out_buffer[i], output);
    fclose(output);
}
void getFromDataBase(myMemory *memory, int m_count, long long int compare_key, char *value)
{

    FILE *input;
    char *db_name = malloc(sizeof(char) * 35);
    char *buffer = malloc(sizeof(char) * 153);
    char path[30];
    strcpy(db_name, "./storage");
    //first see memory
    for (int i = m_count - 1; i >= 0; --i)
    {
        //i just read from the end to begin,so it is the newest
        if (memory[i].key == compare_key)
        {
            strcpy(value, memory[i].value);
            int len = strlen(value);
            value[len] = '\n';
            value[len + 1] = '\0';
            free(db_name);
            free(buffer);
            return;
        }
    }

    //then see the database
    long long int key = compare_key;
    long long int folder = key / size; //map to the relative folder/file base on the key number
    long long int file = key % size;
    sprintf(path, "/%lld", folder);
    strcat(db_name, path);
    if (stat(db_name, &st) != -1)
    {
        //not exist folder
        sprintf(path, "/%lld", file);
        strcat(db_name, path);
        if (input = fopen(db_name, "r"))
        {
            fgets(buffer, 255, input);
            int c = 0;
            char key[30];
            char v[130];
            char *token = strtok(buffer, " ");
            while (token != NULL)
            {
                if (c == 0)
                {
                    strcpy(key, token);
                    ++c;
                }
                else
                    strcpy(v, token);
                token = strtok(NULL, " ");
            }

            if (compare_key == atoll(key))
            {
                free(db_name);
                free(buffer);
                fclose(input);
                strcpy(value, v);
                int len = strlen(value);

                value[len] = '\n';
                value[len + 1] = '\0';
                printf("%s", value);
                return;
            }
            free(db_name);
            free(buffer);
            fclose(input);
            strcpy(value, "EMPTY\n");
            return;
        }
    }
    else
    {
        free(db_name);
        free(buffer);
        strcpy(value, "EMPTY\n");
        return;
    }
}
void scanFromeDatabase(myMemory *memory, int m_count, long long int begin, long long int end, char **out_buffer, int *o_count)
{
    FILE *db;
    int range = end - begin + 1;
    char *buffer[range];
    char *input_string = malloc(sizeof(char) * 153);
    char *db_name = malloc(sizeof(char) * 35);
    char *key_value[2];
    char path[30];
    strcpy(db_name, "./storage");
    for (int i = 0; i < range; ++i)
    {
        buffer[i] = malloc(sizeof(char) * 153);
        strcpy(buffer[i], "EMPTY\n");
    }

    //scan may cross file!!!!!!
    for (long long int i = begin; i <= end; ++i)
    {
        long long int key = i;
        long long int folder = key / size; //map to the relative folder/file base on the key number
        long long int file = key % size;
        sprintf(path, "/%lld", folder);
        strcat(db_name, path);
        if (stat(db_name, &st) != -1)
        {

            //folder exist

            sprintf(path, "/%lld", file);
            strcat(db_name, path);
            if (db = fopen(db_name, "r"))
            {
                fgets(input_string, 153, db);
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
                fclose(db);
            }
        }
        strcpy(db_name, "./storage");
    }

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

        if (*o_count >= out_buffer_size)
        {
            writeToOutput(out_buffer, *o_count);
            *o_count = 0;
        }
        strcpy(out_buffer[*o_count], buffer[i]);
        *o_count += 1;
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
        if (instr[2][end] == '\n')
            instr[2][end] = '\0';

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

        long long int key = atoll(instr[1]);
        char *value = malloc(sizeof(char) * 130);
        getFromDataBase(memory, *m_count, key, value);
        if (*out_count >= out_buffer_size)
        {

            writeToOutput(out_buffer, *out_count);
            *out_count = 0;
        }
        strcpy(out_buffer[*out_count], value);
        ++*out_count;
        free(value);
    }
    else
    {
        //scan range may be large,so i print the answer as soon as i find key,not sotre in memory

        //printf("%lld %lld", atoll(instr[1]), atoll(instr[2]));
        long long int begin = atoll(instr[1]);
        long long int end = atoll(instr[2]);
        scanFromeDatabase(memory, *m_count, begin, end, out_buffer, out_count);
    }
}
void substr(char *dest, const char *src, unsigned int start, unsigned int cnt)
{
    strncpy(dest, src + start, cnt);
    dest[cnt] = 0;
}
int main(int argc, char *argv[])
{
    printf("pid=%d\n", getpid());
    char *folder = "storage";
    char *input_file = malloc(sizeof(char) * 20);
    output_file = malloc(sizeof(char) * 10);
    char *input_string = malloc(sizeof(char) * 255);
    char *instr[3];
    FILE *input;
    myMemory *memory = malloc(sizeof(myMemory) * size);
    char *out_buffer[out_buffer_size];
    int m_count = 0;
    int out_count = 0;
    //strcpy(input_file, "hw3example.input");
    //strcpy(output_file, "ex.output");
    substr(input_file, argv[1], 2, 7);
    sprintf(output_file, "%c.output", input_file[0]);
    input = fopen(input_file, "r");
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
    //rest in output_buffer
    writeToOutput(out_buffer, out_count);
    free(memory);
    for (int i = 0; i < out_buffer_size; ++i)
        free(out_buffer[i]);
    return 0;
}