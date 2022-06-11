#include "hack_vm.h"
#include <dirent.h>
#include <errno.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/stat.h>

/* Reader module */
/* Check given name is file(0) or directory(!0) */
int check_file_ext(char *file_name, char *ext)
{
    char *p;

    p = strrchr(file_name, '.');
    if (strcmp(p, ext) == 0)
        return (0);
    return (1);
}

/* Read file contents and set to parser */
void file_to_parser(char *file_name, char *dest_file_name)
{
    char read_line[READSIZ] = {'\0'};
    FILE *fp;
    vm_command *vm_commands = (vm_command *)malloc(sizeof(vm_command));
    if (vm_commands == NULL)
    {
        fprintf(stderr, "%s\n", strerror(ENOMEM));
        exit(ENOMEM);
    }

    initialize_vm_command(vm_commands);
    vm_commands->IS_NULL = 1;
    fp = fopen(file_name, "r");
    while ((fgets(read_line, READSIZ, fp) != NULL))
        vm_commands = parser(read_line, vm_commands);
    fclose(fp);
    code_writer(file_name, dest_file_name, vm_commands);
    free_commands(vm_commands);
}

/* Open directory recursively and set to parser  */
void directory_to_parser(char *file_name, char *dest_file_name)
{
    DIR *dir = NULL;
    struct dirent *dp = NULL;
    char file_path[MAXPATHSIZ] = {'\0'};
    struct stat fi;

    dir = opendir(file_name);
    if (dir == NULL)
    {
        fprintf(stderr, "%s\n", strerror(ENOENT));
        exit(ENOENT);
    }
    for (dp = readdir(dir); dp != NULL; dp = readdir(dir))
    {
        if (dp->d_name[0] != '.')
        {
            memset(file_path, '\0', MAXPATHSIZ);
            joinword(file_path, file_name, "/");
            joinword(file_path, file_path, dp->d_name);
            stat(file_path, &fi);
            if (!S_ISDIR(fi.st_mode))
            {
                if (check_file_ext(file_path, ".vm") == 0)
                    file_to_parser(file_path, dest_file_name);
            }
            else
                directory_to_parser(file_path, dest_file_name);
        }
    }
    closedir(dir);
}

/* Join src1 to src2 */
void joinword(char *dest, char *src1, char *src2)
{
    int i = 0;
    int j = 0;

    while (src1[i] != '\0')
    {
        dest[i] = src1[i];
        i++;
    }
    while (src2[j] != '\0')
        dest[i++] = src2[j++];
}
