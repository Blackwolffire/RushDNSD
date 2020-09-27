#pragma once

#include "dns_engine.h"

void free_zone(zone *zone1);
void free_bin_tree(bin_tree *tree);
void free_soa(SOA_data *soa);
