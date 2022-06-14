#include <stdio.h>
#include <dirent.h>
#include <sys/stat.h>
#include <string.h>
#include <stdlib.h>
#include <unistd.h>

static unsigned long
get_file_mod(const char *filePath)
{
    struct stat fileStats;

    int errno;
    if ((errno = stat(filePath, &fileStats)) == -1)
    {
        fprintf(stderr, "Stat failed on filepath '%s' with '%s'\n", filePath, strerror(errno));
        exit(1);
    }

    return fileStats.st_mtime;
}

static unsigned long
get_file_mod_from_dir(const char *dirPath)
{
    unsigned long lastMod = 0;
    struct dirent *dir;

    DIR *d = opendir(dirPath);
    if (!d)
    {
        fprintf(stderr, "Cannot open dir: %s", dirPath);
    }

    while ((dir = readdir(d)) != NULL)
    {
        unsigned long fileMod;
        char fullDir[256];
        strcpy(fullDir, dirPath);
        strcat(fullDir, dir->d_name);
        fileMod = get_file_mod(fullDir);

        if (lastMod < fileMod)
        {
            lastMod = fileMod;
        }
    }

    closedir(d);
    return lastMod;
}

void
watch_file(const char *filePath, void (*callbackfunction)())
{
    unsigned long lastMod = get_file_mod(filePath);

    for (;;)
    {
        unsigned long fileMod = get_file_mod(filePath);
        if (lastMod < fileMod)
        {
            callbackfunction();
            lastMod = fileMod;
        }
        sleep(1);
    }
}

void
watch_dir(const char *dirPath, void (*callbackFunction)())
{
    unsigned long lastMod = get_file_mod_from_dir(dirPath);

    for (;;)
    {
        unsigned long fileMod = get_file_mod_from_dir(dirPath);
        if (lastMod < fileMod)
        {
            callbackFunction();
            lastMod = fileMod;
        }
    }
}
