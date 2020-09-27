#include <stdlib.h>

#include "dns_runner.h"
#include "network_wrapper.h"
#include "request_parser.h"
#include "analyser.h"
#include "response_forger.h"

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

void dns_run(dns_engine *engine)
{
  ssize_t size, sizeres;
  char *pck, *res;
  dns *dnspck;

  for(;;)
  {

    if (fetch_clients(engine)){
      dns_quit(engine);
      break;
    }
    for (int i=0; i < engine->nbip; ++i)
    {
      for (int j=0; j < engine->nbfd[i]; ++j)
      {
        size = dns_get(&pck, engine->events[i][j].data.fd);
        dnspck = request_parser(pck, size);
        dnspck = analyser(dnspck);
        res = response_forge(dnspck, &sizeres);
        if (sizeres < 0){
          free(dnspck);
          dns_quit(engine);
          break;
        }
        dns_send(engine->events[i][j].data.fd, res, (size_t)sizeres);
        close_client(engine->epfds[i], engine->events[i][j].data.fd,
                     engine->ep_events + i);
        engine->nbfd[i] -= 1;
      }
    }
  }
}

void dns_quit(dns_engine *engine)
{
  if (!engine)
    return;
  close_serv(engine);
  free(engine);
}
