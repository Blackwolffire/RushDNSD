#include "find_zone.h"

size_t domain_nb(char *qname){
	size_t count = 0;
	for (size_t i = 0; i <= strlen(qname); i++){
		if (qname[i] == '.')
			count++;
	}
	return count + 1;
}



// find a zone in the file zone
zone_found find_zone(bin_tree *tree, char *qname, uint16_t qtype){
	size_t array_size = domain_nb(qname);
	char *url_array[array_size];
	url_array[0] = strtok(qname, ".");
	for (size_t i = 1; i < array_size; i++){
		url_array[i] = strtok(NULL, ".");
	}

	// found name and type
	zone_found result;
	result.found = NULL;
	result.found_no_type = 0;
	result.found_no_terminal = 0;

	int j = array_size - 1;

	bin_tree *current = tree;

	while (j >= 0){
		// while no match, go next
		while (current && (strcmp(qname, url_array[j]) != 0)){
			if (current->bro)
				current = current->bro;
		}
		if (j == 0 ){
			// Empty non terminal
			if (current->son && current->nb_zone == 0){
				result.found_no_terminal = 1;
				return result;
			}
			else{
				for (size_t i = 0; i < current->nb_zone; i++){
					uint16_t type = current->zone_list[i]->type;
					if (type == qtype){
						result.found = current->zone_list[i];
						return result;
					}
				}
				result.found_no_terminal = 1;
				return result;
			}

		}
		else if (current && current->son){
			j--;
			current = current->son;
		}
		else {
			// not found
			return result;
		}
	}
	return result;
}
