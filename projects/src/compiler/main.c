#include "jack_compiler.h"
#include <dirent.h>
#include <errno.h>
#include <libgen.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/stat.h>

int main(int argc, char **argv)
{
    char *file_path = NULL;
    struct stat fi;

    if (argc != 2)
    {
        fprintf(stderr, "%s\n", strerror(EINVAL));
        exit(EINVAL);
    }
    file_path = argv[1];
    if (stat(file_path, &fi))
    {
        fprintf(stderr, "%s\n", strerror(ENOENT));
        exit(ENONET);
    }
    switch (fi.st_mode & S_IFMT)
    {
    case S_IFREG:
        open_file(file_path);
        break;
    case S_IFDIR:
        open_dir(file_path);
        break;
    default:
        fprintf(stderr, "Invalid file or directory\n");
        exit(1);
    }
    return (0);
}

void open_file(char *file_path)
{
    jack_analyzer(file_path);
}

void open_dir(char *dir_path)
{
    DIR *dp = NULL;
    struct dirent *dir;
    char path_buf[PATH_MAX];

    dp = opendir(dir_path);
    if (dp == NULL)
    {
        fprintf(stderr, "%s\n", strerror(ENOENT));
        exit(ENOENT);
    }
    for (dir = readdir(dp); dir != NULL; dir = readdir(dp))
    {
        if (dir->d_name[0] == '.')
            continue;
        switch (dir->d_type)
        {
        case DT_REG:
            if (match_extension(dir->d_name, ".jack"))
            {
                memset(path_buf, '\0', PATH_MAX);
                if (dir_path[strlen(dir_path) - 1] == '/')
                    strcat(path_buf, dir_path);
                else
                {
                    strcat(path_buf, dir_path);
                    strcat(path_buf, "/");
                }
                strcat(path_buf, dir->d_name);
                open_file(path_buf);
            }
            break;
        case DT_DIR:
            memset(path_buf, '\0', PATH_MAX);
            if (dir_path[strlen(dir_path) - 1] == '/')
                strcat(path_buf, dir_path);
            else
            {
                strncat(path_buf, dir_path, strlen(dir_path) - 1);
                strcat(path_buf, "/");
            }
            strcat(path_buf, dir->d_name);
            open_dir(path_buf);
            break;
        }
    }
    closedir(dp);
}
