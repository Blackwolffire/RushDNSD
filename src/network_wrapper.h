#ifndef NETWORK_WRAPPER
#define NETWORK_WRAPPER

#include <stddef.h>

#include "dns_engine.h"

#define BACKLOG 512
#define MAXEPOLLSIZE 1024

dns_engine* init_serv(dns_engine *engine, char *ip, uint16_t port);
int fetch_clients(dns_engine *eng);
ssize_t dns_get(char **ptr, int socket);
ssize_t dns_send(int socket, char *pck, size_t size);
void close_client(int epfd, int socket, struct epoll_event *ev);
void close_serv(dns_engine *engine);

#endif // NETWORK_WRAPPER
