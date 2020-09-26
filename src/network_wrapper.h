#ifndef NETWORK_WRAPPER
#define NETWORK_WRAPPER

#include "dns_engine.h"

void* init_serv(dns_engine *engine, char *ip, uint16_t port);
void* dns_get(dns_engine *engine, int socket);
void dns_send(dns_engine *engine, int socket, void *pck);
void* close_serv(dns_engine *engine);

#endif // NETWORK_WRAPPER
