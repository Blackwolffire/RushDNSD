#pragma once

#include <stdlib.h>
#include <string.h>
#include "dns.h"


void *response_forge(dns *ans); // struct -> packet (void*)
int compute_strings_length(dns *ans);
void *forge_header(dns *ans, dns_header *h);
void *forge_quest(dns *ans, void *q);
void *forge_answer(dns *ans, void *a);
void *forge_authority(dns *ans, void *au);
void *forge_additional(dns *ans, void *ad);
