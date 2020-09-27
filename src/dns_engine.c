#define _POSIX_C_SOURCE 200809L

#include <arpa/inet.h>

#include "dns.h"
#include "my_libc.h"
#include "my_free.h"
#include "dns_engine.h"
#include "collector.h"

int error_file(char *msg)
{
    dprintf(STDERR_FILENO, "error file : %s", msg);
    _exit(12);
}

bin_tree *load_zone(dns_engine *engine, const char *filename)
{
    FILE *file = fopen(filename, "r");
    if (!file)
        error_file("Impossible d'ouvrir le fichier");
    bin_tree *tree_zone = create_tree(file, engine);
    fclose(file);
    return tree_zone;
}

bin_tree *create_tree(FILE *file, dns_engine *engine)
{
    //escape ; attention
    int SOA = 0;
    char *current_line = NULL;
    bin_tree *tree = NULL;
    uint64_t len = 0;

    while (getline(&current_line, &len, file) != -1)
    {
        zone *current_zone = get_zone(current_line); 
        if (!current_zone)
            continue;
        if (current_zone->type == SOA_type)
        {
            SOA++;
            engine->soa_zone = current_zone;
        }
        if (SOA > 1)
            error_file("Multiple SOA registration");
        tree = add_to_tree(current_zone, tree);
        printf("%s\n", tree->name);
    }

    free(current_line);

    if (!tree)
        error_file("file is empty");
    return tree;
}

bin_tree *create_node(char *name)
{
    int name_size = strlen(name) + 1;
    bin_tree *res = my_malloc(sizeof(bin_tree));
    res->name = my_malloc(name_size);
    strncpy(res->name, name, name_size);
    res->nb_zone = 0;
    printf("%s\n", name);
    return res;
}

bin_tree *add_to_tree(zone *new_zone, bin_tree *tree)
{
    char *array[20];
    int i = 0;
    array[i] = strtok(new_zone->name, ".");
    while (array[i])
    {
        i++;
        array[i] = strtok(NULL, ".");
    }
    int j = i - 1;
    if (!tree)
        tree = create_node(array[j]);    
  
    bin_tree *current = tree;

    while (j >= 0)
    {
        while (current && strcmp(current->name, array[j]) != 0) //while current
        {
            if (current->bro)
                current = current->bro;
            else
            {
                current->bro = create_node(array[j]);
                current = current->bro;
            }
        }
        if (j == 0)
        {
            current->zone_list = realloc(current->zone_list, (current->nb_zone + 1) * sizeof(zone*));
            current->zone_list[current->nb_zone] = new_zone;
            current->nb_zone += 1;
            return tree;
        }
        else if (current && current->son)
        {
            j--;
            current = current->son;
        }
        else
        {
            j--;
            current->son = create_node(array[j]);
            current = current->son;
        }
    }
    return tree;
}

zone *get_zone(char *line)
{
    zone *new_zone = my_malloc(sizeof(zone));

    char *tmp_word = strtok(line, ";");
    int tmp_size = strlen(tmp_word) + 1; //with \0
    new_zone->name = my_malloc(tmp_size);
    strncpy(new_zone->name, tmp_word, tmp_size);
    
    int i = 0;
    while ((tmp_word = strtok(NULL, ";")) != NULL)
    {
        while (tmp_word[strlen(tmp_word)-1] == '\\')
        {
            tmp_word[strlen(tmp_word)] = ';';
            strtok(NULL, ";");
        }
        if (i == 0)
            new_zone->type = (short)strtol(tmp_word, NULL, 10);
        else if (i == 1)
            new_zone->ttl = (int)strtol(tmp_word, NULL, 10);
        else if (i == 2)
        {
            if (new_zone->type == SOA_type)
                new_zone->data = get_soa_struct(tmp_word);
            else
            {
                tmp_size = strlen(tmp_word) + 1;
                new_zone->data = my_malloc(tmp_size);
                strncpy(new_zone->data, tmp_word, tmp_size);
            }
        }
        else
            break;
        i++;
    }
    if (i != 3 || (new_zone->type == SOA_type && new_zone->data == NULL))
    {
        free_zone(new_zone);
        dprintf(STDERR_FILENO, "File : a lign is incorrect and not taken into account\n");
        return NULL;
    }
    if (new_zone->type == A_type || new_zone->type == AAAA_type)
        new_zone = check_new_zone(new_zone);

    return new_zone;
}

zone *check_new_zone(zone *new_zone)
{
    void *tmp = {0};
    if (new_zone->type == A_type)
    {
        if (inet_pton(AF_INET, new_zone->data, tmp) != 1)
            return NULL;
    }
    else
    {
        if (inet_pton(AF_INET6, new_zone->data, tmp) != 1)
            return NULL;
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

    int i = 0;
    while ((tmp_word = strtok(NULL, " ")) != NULL)
    {
        if (i == 0)
            soa->serial = strtol(tmp_word, NULL, 10);
        else if (i == 1)
            soa->refresh = strtol(tmp_word, NULL, 10);
        else if (i == 2)
            soa->retry = strtol(tmp_word, NULL, 10);
        else if (i == 3)
            soa->expire = strtol(tmp_word, NULL, 10);
        else if (i == 4)
            soa->minimum = strtol(tmp_word, NULL, 10);
        else
            break;
        i++;
    }

    if (i != 5)
    {
        free(soa);
        dprintf(STDERR_FILENO, "File : a lign is incorrect and not taken into account\n");
        return NULL;
    }
    return soa;
}
