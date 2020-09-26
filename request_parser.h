#pragma once
#include "dns.h"

int size_name(char *request, int index, char *request_dns_name);

dns *request_parser(void *request, int buffer_size);

void printer(dns *dns_paquet);
