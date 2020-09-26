#pragma once
#include "dns.h"

int size_name(uint16_t *request, int index, char *request_dns_name);

int answer_auth_add(uint16_t *request, answer *dns_a, int accu, int count);

dns *request_parser(void *request, int buffer_size);

void printer(dns *dns_paquet);

void free_repete(answer *answer_free, int count);

void free_dns_struct(dns *dns_free);

