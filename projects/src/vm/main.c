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
    vm_command *vm_commands = (vm_command *)malloc(sizeof(vm_command));
    if (vm_commands == NULL)
    {
        fprintf(stderr, "%s\n", strerror(ENOMEM));
        exit(ENOMEM);
    }

    initialize_vm_command(vm_commands);
    vm_commands->IS_NULL = 1;
    dest_name = argv[0];
    file_name = argv[1];
    stat(file_name, &fi);
    dest_file_name = create_dest_file_name(dest_name, file_name, fi.st_mode);
    if (!S_ISDIR(fi.st_mode))
    {
        if (check_file_ext(file_name, ".vm") == 0)
            vm_commands = file_to_parser(file_name, vm_commands);
    }
    else
        vm_commands = directory_to_parser(file_name, vm_commands);
    code_writer(file_name, dest_file_name, vm_commands);
    free_commands(vm_commands);
    free(dest_file_name);
    return (0);
}
