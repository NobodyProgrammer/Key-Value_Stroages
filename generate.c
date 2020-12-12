#include <stdio.h>
#include <stdlib.h>
#include <limits.h>
#include <time.h>
#include <math.h>
int main()
{
    srand(time(NULL));
    FILE *ouput = fopen("3.input", "w");
    int file_num = 0;
    long int array[5];
    long int key = 2147483648;
    char *value[5];
    char *buffer = malloc(sizeof(char) * 255);
    for (int i = 0; i < 5; ++i)
        value[i] = malloc(sizeof(char) * 128);
    while (file_num <= 10)
    {
        for (int i = 0; i < 5; ++i)
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
                    value[i][j] = (rand() % 10) + 48;
                    break;
                case 1:
                    value[i][j] = (rand() % 10) + 65;
                    break;

                default:
                    value[i][j] = (rand() % 10) + 97;
                    break;
                }
            }
            sprintf(buffer, "PUT %ld %s\n", array[i], value[i]);
            fputs(buffer, ouput);
        }

        key += pow(2, 20);
        ++file_num;
    }
}