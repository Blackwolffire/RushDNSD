#include <stdlib.h>

#include "dns_runner.h"
#include "network_wrapper.h"

dns_engine* dns_init(char *filename, uint16_t port, char *ip)
{
  dns_engine *engine = malloc(sizeof(dns_engine));

  if (!engine)
    return NULL;

  engine->dns_zone = NULL;
  engine->port = 0;
  engine->sockets = NULL;
  engine->ip = NULL;
  engine->nbip = 0;
  engine->epfds = NULL;
  engine->nbfd = NULL;
  engine->ep_events = NULL;
  engine->events = NULL;

  if (!init_serv(engine, ip, port)){
    free(engine);
    return NULL;
  }

  (void)filename;//load_zone(engine, filename);

  return engine;
}
