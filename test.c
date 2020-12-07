#include <stdio.h>
#include <stdlib.h>
#include <string.h>
int main()
{
    int c = 0;
    FILE *input = fopen("input", "r");
    FILE *output = fopen("output", "w");
    char *buffer = malloc(sizeof(char) * 155);
    char instr[3][130];
    while (!feof(input))
    {
        /* code */

        int c2 = 0;

        fgets(buffer, 155, input);

                //printf("%d\n", strlen(buffer));

        char *temp = buffer;
        char *token = strtok(temp, " ");
        while (token != NULL)
        {
            strcpy(instr[c2++], token);
            token = strtok(NULL, " ");
        }
        int complement = 19 - strlen(instr[1]);
        printf("%d\n", complement);
        char space[19];
        for (int i = 0; i <= complement; ++i)
        {
            if (i == complement)
            {
                space[i] = '\n';
                space[i + 1] = '\0';
            }
            else
                space[i] = '|';
        }
        int len = strlen(instr[2]) - 1;
        if (instr[2][len] == '\n')
            instr[2][len] = '\0';
        sprintf(buffer, "%s %s %s%s", instr[0], instr[1], instr[2], space);
        fputs(buffer, output);
    }
    fclose(output);
    fclose(input);
}