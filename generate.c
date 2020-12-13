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
    FILE *ouput = fopen("1.input", "w");
    FILE *proofFile = fopen("2_1.output", "w");
    int file_num = 0;
    long int array[size];
    long int key = 2147483648;

    char *value[size];
    char *buffer = malloc(sizeof(char) * 255);

    for (int i = 0; i < size; ++i)
        value[i] = malloc(sizeof(char) * 128);
    while (file_num <= 100000)
    {

        for (int i = 0; i < size; ++i)
        {
            int pivot = pow(2, 20);
            int r = rand() % pivot;
            array[i] = key + r;

            for (int j = 0; j < 128; ++j)
            {
                int type = rand() % 3;
                switch (type)
                {
                case 0:
                    value[i][j] = (rand() % size) + 48;
                    break;
                case 1:
                    value[i][j] = (rand() % size) + 65;
                    break;

                default:
                    value[i][j] = (rand() % size) + 97;
                    break;
                }
            }
            sprintf(buffer, "PUT %ld %s\n", array[i], value[i]);
            fputs(buffer, ouput);
            //generate get instrution
            if (i == 9)
            {
                int idx = rand() % 10;
                sprintf(buffer, "GET %ld\n", array[idx]);

                fputs(buffer, ouput);
                sprintf(buffer, "%s\n", value[idx]);
                fputs(buffer, proofFile);
            }
        }
        //generate scan

        for (long int i = key; i < key + 50; ++i)
        {
            char *temp = malloc(sizeof(char) * 128);
            for (int j = 0; j < 128; ++j)
            {
                int type = rand() % 3;
                switch (type)
                {
                case 0:
                    temp[j] = (rand() % size) + 48;
                    break;
                case 1:
                    temp[j] = (rand() % size) + 65;
                    break;

                default:
                    temp[j] = (rand() % size) + 97;
                    break;
                }
            }
            sprintf(buffer, "PUT %ld %s\n", i, temp);
            fputs(buffer, ouput);
            sprintf(buffer, "%s\n", temp);
            fputs(buffer, proofFile);
        }
        fputs("EMPTY\n", proofFile);
        if (file_num == 100000)
            sprintf(buffer, "SCAN %ld %ld", key, key + 50);
        else
            sprintf(buffer, "SCAN %ld %ld\n", key, key + 50);
        fputs(buffer, ouput);

        key += pow(2, 20);
        ++file_num;
    }
    fclose(ouput);
    fclose(proofFile);
}