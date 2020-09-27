#include "my_free.h"

#include <stdlib.h>

void free_zone(zone *zone1)
{
    free(zone1->name);
    if (zone1->type == SOA_type && zone1->data)
        free_soa(zone1->data);
    else
        free(zone1->data);
    free(zone1);
}

void free_bin_tree(bin_tree *tree)
{
    bin_tree *tmp = tree;
    if (!tree)
        return;
    if (tmp->son)
        free_bin_tree(tmp->son);
    if (tmp->bro)
        free_bin_tree(tmp->bro);

    free(tmp->name);
    for (unsigned i = 0; i < tmp->nb_zone; i++)
        free_zone(tmp->zone_list[i]);
    free(tmp->zone_list);
    free(tmp);
}


void free_soa(SOA_data *soa)
{
    free(soa->mname);
    free(soa->rname);
    free(soa);
}

