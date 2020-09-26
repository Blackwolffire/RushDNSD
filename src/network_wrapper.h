#ifndef NETWORK_WRAPPER
#define NETWORK_WRAPPER

#include "dns_engine.h"

void* init_serv(dns_engine *engine, uint16_t port, char *ip);
void* dns_get(dns_engine *engine);
void dns_send(dns_engine *engine, void *pck);
void* close_serv(dns_engine *engine);

#endif // NETWORK_WRAPPER
