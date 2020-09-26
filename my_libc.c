#define _POSIX_C_SOURCE 200809L

#include "my_libc.h"

void *my_malloc(int size)
{
    char *res = calloc(size, 1);
    if (!res)
    {
        dprintf(STDERR_FILENO, "Allocation error\n");
        _exit(1);
    }
    return (void*)res;
}
