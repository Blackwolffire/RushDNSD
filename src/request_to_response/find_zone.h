#pragma once
#include "analyser.h"

typedef struct zone_found zone_found;
struct zone_found {
	zone *found;
	size_t found_no_type;
	size_t found_no_terminal;
};

zone_found find_zone(bin_tree *tree, char *qname, uint16_t qtype);

