#include <stdio.h>
#include <stdlib.h>
#include <limits.h>
#include <time.h>
#include <math.h>
#include <unistd.h>
int main()
{
    printf("pid=%d\n", getpid());
    srand(time(NULL));
    int size = 10;
    FILE *ouput = fopen("5.input", "w");
    FILE *proofFile = fopen("2_5.output", "w");
    int file_num = 0;
    long int array[size];
    long int key = 2147483648;
    char *buffer = malloc(sizeof(char) * 155);
    char *value = malloc(sizeof(char) * 130);
    for (int i = 0; i < 50000; ++i)
    {
        for (int j = 0; j < 128; ++j)
        {
            int type = rand() % 3;
            switch (type)
            {
            case 0:
                value[j] = (rand() % size) + 48;
                break;
            case 1:
                value[j] = (rand() % size) + 65;
                break;

            default:
                value[j] = (rand() % size) + 97;
                break;
            }
        }
        value[128] = '\n';
        value[129] = '\0';
        sprintf(buffer, "PUT %d %s", i, value);
        fputs(buffer, ouput);

        fputs(value, proofFile);
    }
    fclose(ouput);
    fclose(proofFile);
}