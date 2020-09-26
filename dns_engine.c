#include "dns.h"
#include "my_libc.h"

int error_file(char *msg)
{
    dprintf(stderr, "error file : %s", msg);
    _exit(12);
}

bin_tree *load_zone(char *filename)
{
    FILE *file = fopen(filename, 'r');
    if (!file)
        error_file("Impossible d'ouvrir le fichier");
    zone *tree_zone = create_tree(f);
    fclose(f);
    return tree_zone;
}

bin_tree *create_tree(FILE *file)
{
    //escape ; attention
    int SOA = 0;
    char *current_line = NULL;
    bin_tree *tree = NULL;

    while (getline(&current_line, 0, file) != -1)
    {
        zone *current_zone = get_zone(current_line); 
        if (zone->type == SOA_type)
            SOA++;
        if (SOA > 1)
            error_file("Multiple SOA registration");
        add_to_tree(current_zone, tree);
    }

    free(current_line);

    if (!tree)
        error_file("file is empty");
    return tree;
}

void add_to_tree(zone *new_zone, bin_tree *tree)
{
    bin_tree *node = my_malloc(sizeof(bin_tree));

}

zone *get_zone(char *line)
{
    zone *new_zone = my_malloc(sizeof(zone));

    char *tmp_word = strtok(line, ";");
    if (!tmp_word)
        error_file("mal foutu");
    int tmp_size = strlen(tmp_word) + 1; //with \0
    new_zone->name = my_malloc(tmp_size);
    strncpy(new_zone->name, tmp_word, tmp_size);

    tmp_word = strtok(NULL,";");
    new_zone->type = (short)strtol(tmp_word, NULL, 10);

    tmp_word = strtok(NULL, ";");
    new_zone->ttl = (int)strtol(tmp_word, NULL, 10);

    tmp_word = strtok(NULL, ";");
    if (new_zone->type == SOA_type)
        new_zone->data = get_soa_struct(tmp_word);
    else
    {
        tmp_size = strlen(tmp_word) + 1;
        new_zone->data = my_malloc(tmp_size);
        strncpy(new_zone->data, tmp_word, tmp_size);
    }
    return new_zone;
}

SOA_data *get_soa_struct(char *word)
{
    SOA_data *soa = my_malloc(sizeof(SOA_data));

    char *tmp_word = strtok(word, " ");
    int tmp_size = strlen(tmp_word) + 1;
    soa->mname = malloc(tmp_size);
    strncpy(soa->mname, tmp_word, tmp_size);
    
    tmp_word = strtok(NULL, " ");
    tmp_size = strlen(tmp_word) + 1;
    soa->rname = malloc(tmp_size);
    strncpy(soa->rname, tmp_word, tmp_size);


    tmp_word = strtok(NULL, " ");
    soa->serial = strtol(tmp_word, NULL, 10);

    tmp_word = strtok(NULL, " ");
    soa->refresh = strtol(tmp_word, NULL, 10);

    tmp_word = strtok(NULL, " ");
    soa->retry = strtol(tmp_word, NULL, 10);

    tmp_word = strtok(NULL, " ");
    soa->expire = strtol(tmp_word, NULL, 10);

    tmp_word = strtok(NULL, " ");
    soa->minimum = strtol(tmp_word, NULL, 10);
    
    return soa;
}
