#include <stdio.h>
#include <stdlib.h>
#include <direct.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <unistd.h>
int main()
{
    char *folder = "storage";
    if (access(folder, F_OK) != 0)
    {
        mkdir(folder, 0777);
    }
}