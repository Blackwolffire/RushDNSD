#include <arpa/inet.h>
#include <stdlib.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <unistd.h>

#include "network_wrapper.h"

int check_ip(char *ip)
{
  uint16_t tmp[8] = {0};
  if (!ip)
    return -1;
  for (size_t i = 0; ip[i]; ++i)
    if (ip[i] == '.')
      return inet_pton(AF_INET, ip, tmp);
    else if (ip[i] == ':' && inet_pton(AF_INET6, ip, tmp) == 1)
      return 2;

  return -1;
}

void* init_serv(dns_engine *engine, char *ip, uint16_t port)
{
  size_t len, nbip;
  int *sockets;

  if (!ip)
    return NULL;

  for (len = 0; ip[len]; ++len)
    if (ip[len] == ',')
      ++nbip;
  engine->ip = malloc(sizeof(char*) * nbip);
  if (!engine->ip)
    return NULL;

  engine->ip[0] = ip;
  size_t j = 1, i = 1;
  for (; j < nbip - 2; ++i)
    if (ip[i] == ','){
      ip[i] = 0;
      engine->ip[j++] = ip + i + 1;
    }
  engine->ip[j] = ip[i];

  engine->nbip = nbip;
  for (i = 0; i < nbip; ++i)
    if (check_ip(engine->ip[i]) < 1){
      free(engine->ip);
      engine->ip = NULL;
      engine->nbip = 0;
      return NULL;
    }

  sockets = malloc(sizeof(int) * nbip);
  if (!sockets){
    free(engine->ip);
    engine->ip = NULL;
    engine->nbip = 0;
    return NULL;
  }

  int ipcheck, domain, error = 0;
  struct sockaddr_in addr;
  addr.sin_port = htons(port);
  for (i = 0; i < nbip; ++i)
  {
    ipcheck = check_ip(engine->ip[i]);
    domain = ipcheck == 1? AF_INET : AF_INET6;
    sockets[i] = socket(domain, SOCK_STREAM, 0);
    bzero(&addr, sizeof(addr));
    addr.sin_family = domain;
    addr.sin_addr.s_addr = inet_addr(engine->ip[i]);
    if (bind(sockets[i], (struct sockaddr*)&addr, sizeof(addr))){
      error = 1;
      break;
    }
  }
  if (error){
    for (j=0; j <= i; ++j)
      close(sockets[i]);
    free(engine->sockets);
    engine->sockets = NULL;
    free(engine->ip);
    engine->ip = NULL;
    engine->nbip = 0;
  }
  engine->sockets = sockets;
}

void* dns_get(dns_engine *engine)
{
  
}
