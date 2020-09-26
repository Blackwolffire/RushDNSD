#pragma once
#include "dns.h"
#include "dns_engine.h"

// response structure
typedef struct response response;
struct respone {
	char *name;
	uint16_t *type;
	uint16_t *clas;
	uint32_t *ttl;
	uint16_t *data_len;
	void *data;
};

// Check if request is vqlid
int check_request(dns *request);

// finds zones corresponding to the request
zone **find_zones(char *name, uint16_t type);

// make the response
dns *make_response(zone **zone_list);

dns *anayser(dns *request);
