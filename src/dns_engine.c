#include "dns.h"
#include "my_libc.h"

int error_file(char *msg)
{
    dprintf(stderr, "error file : %s", msg);
    _exit(12);
}

bin_tree *load_zone(const char *filename)
{
    FILE *file = fopen(filename, "r");
    if (!file)
        error_file("Impossible d'ouvrir le fichier");
    bin_tree *tree_zone = create_tree(file);
    fclose(file);
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
        if (current_zone->type == SOA_type)
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


int add_to_tree(zone *new_zone, bin_tree *tree)
{
    char **array;
    *(array[0]) = strtok(new_zone->name, ".");
    int i = 1;
    while (1)
    {
        array[i] = strtok(NULL, ".");
        if (!array[i])
            break;
        i++;
    }

    if (!tree)
    {
        bin_tree *current = tree;

        for (int j = i -1; j >= 0; j--)
        {
            current = my_malloc(sizeof(bin_tree));
            current->name = my_malloc(strlen(array[j]) + 1);
            strncpy(current->name, array[j], strlen(array[j]) + 1);
            if (j == 0)
            {
                current->nb_zone = 1;
                current->zone_list = my_malloc(sizeof(zone*));
                *(current->zone_list) = new_zone;
            }
            else
                current->nb_zone = 0;
        }
    }

    //for (int j = i -1; j >= 0; j--)
    //{
    
    //}
    //create array of pointer + counter
    //parcours liste à l'envers
    //si noeuc null on créé
    //garder noeud supperieur
    //gauche ou droite
    //add avec malloc
    //bin_tree *node = my_malloc(sizeof(bin_tree));
    return 0;

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

int main(){
    return 0;
}
