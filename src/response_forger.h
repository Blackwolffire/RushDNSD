#pragma once

#include <stdlib.h>
#include <string.h>
#include "dns.h"
#include "request_parser.h"


void *response_forge(dns *ans, ssize_t *size); // struct -> packet (void*)
int compute_strings_length(dns *ans);
void *forge_header(dns *ans, void *header);
void *forge_quest(dns *ans, void *quest);
void *forge_answer(dns *ans, void *answ);
void *forge_authority(dns *ans, void *auth);
void *forge_additional(dns *ans, void *addit);
