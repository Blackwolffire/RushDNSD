void *my_malloc(size_t size)
{
    char *res = malloc(size);
    if (!res)
    {
        dprintf(stderr, "Allocation error\n");
        _exit(1);
    }
    return (void*)res;
}
