#include "hack_vm.h"
#include <errno.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/stat.h>

int main(int argc, char **argv)
{
    if (argc != 2)
    {
        fprintf(stderr, "%s\nUsage:\thack-vm file_name\n", strerror(EINVAL));
        exit(EINVAL);
    }

    char *file_name = NULL;
    char *dest_name = NULL;
    char *dest_file_name = NULL;
    struct stat fi;

    dest_name = argv[0];
    file_name = argv[1];
    stat(file_name, &fi);
    dest_file_name = create_dest_file_name(dest_name, file_name, fi.st_mode);
    if (!S_ISDIR(fi.st_mode))
    {
        if (check_file_ext(file_name, ".vm") == 0)
            file_to_parser(file_name, dest_file_name);
    }
    else
        directory_to_parser(file_name, dest_file_name);
    free(dest_file_name);
    return (0);
}
