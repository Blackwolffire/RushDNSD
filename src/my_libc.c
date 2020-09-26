void *my_malloc(size_t size)
{
    char *res = calloc(size, 1);
    if (!res)
    {
        dprintf(stderr, "Allocation error\n");
        _exit(1);
    }
    return (void*)res;
}
