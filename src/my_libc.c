#define _GNU_SOURCE

#include <stdlib.h>
#include <stdio.h>
#include <err.h>

void *my_malloc(size_t size)
{
    char *res = malloc(size);
    if (!res)
    {
        dprintf(2, "Allocation error\n");  // File descriptor 2 is stderr
        exit(1);
    }
    return (void*)res;
}
