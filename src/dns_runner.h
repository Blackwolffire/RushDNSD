#ifndef DNS_RUNNER_H
#define DNS_RUNNER_H

#include "dns_engine.h"

dns_engine* dns_init(char *filename, uint16_t port, char *ip);
void dns_run();

#endif // DNS_RUNNER_H
