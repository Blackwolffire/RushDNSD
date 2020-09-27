#include <arpa/inet.h>
#include <fcntl.h>
#include <stdlib.h>
#include <strings.h>
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

int unblock_sock(int fd)
{
  int flags, tmp;
  flags = fcntl(fd, F_GETFL, 0);
  if (flags == -1)
    return -1;
  flags |= O_NONBLOCK;
  tmp = fcntl(fd, F_SETFL, flags);
  if (tmp == -1)
    return -1;
  return 0;
}

dns_engine* init_serv(dns_engine *engine, char *ip, uint16_t port)
{
  size_t len, nbip = 1;
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
  for (; j < nbip; ++i)
    if (ip[i] == ','){
      ip[i] = 0;
      engine->ip[j++] = ip + i + 1;
    }

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

  int *epfds = malloc(sizeof(int) * nbip);
  if (!epfds){
    free(sockets);
    engine->sockets= NULL;
    free(engine->ip);
    engine->ip = NULL;
    engine->nbip = 0;
    return NULL;
  }

  int *nbfd = calloc(nbip, sizeof(int));
  if (!nbfd){
    free(epfds);
    free(sockets);
    engine->sockets= NULL;
    free(engine->ip);
    engine->ip = NULL;
    engine->nbip = 0;
    return NULL;
  }

  struct epoll_event *ep_events = calloc(nbip, sizeof(struct epoll_event));
  if (!ep_events){
    free(nbfd);
    free(epfds);
    free(sockets);
    engine->sockets= NULL;
    free(engine->ip);
    engine->ip = NULL;
    engine->nbip = 0;
    return NULL;
  }

  struct epoll_event **events = calloc(nbip, sizeof(struct epoll_event*));

  int ipcheck, domain, error = 0; //, flags;
  struct sockaddr_in addr;
  struct sockaddr_in6 addr6;
  addr.sin_port = htons(port);
  for (i = 0; i < nbip; ++i)
  {
    ipcheck = check_ip(engine->ip[i]);
    domain = ipcheck == 1? AF_INET : AF_INET6;
    sockets[i] = socket(domain, SOCK_STREAM, 0);
    /*if (setsockopt(sockets[i], SOL_SOCKET, SO_REUSEADDR, &flags,
                   sizeof(flags)) == -1){
      error = 1;
      break;
    }*/
    //unblock_sock(sockets[i]);
    if (domain == AF_INET){
      bzero(&addr, sizeof(addr));
      addr.sin_family = domain;
      addr.sin_addr.s_addr = inet_addr(engine->ip[i]);
      addr.sin_port = htons(port);
    }else{
      bzero(&addr6, sizeof(addr6));
      addr6.sin6_family = domain;
      inet_pton(AF_INET6, engine->ip[i], &(addr6.sin6_addr.s6_addr));
      addr6.sin6_port = htons(port);
    }
    if (domain == AF_INET){
      if (bind(sockets[i], (struct sockaddr*)&addr, sizeof(addr))){
        error = 1;
        break;
      }
    }else{
      if (bind(sockets[i], (struct sockaddr*)&addr6, sizeof(addr6))){
        error = 1;
        break;
      }
    }
    if (listen(sockets[i], BACKLOG)){
      error = 1;
      break;
    }
    if ((epfds[i] = epoll_create(MAXEPOLLSIZE)) == -1){
      error = 1;
      break;
    }
    ep_events[i].events = EPOLLIN | EPOLLET;
    ep_events[i].data.fd = sockets[i];
    if (epoll_ctl(epfds[i], EPOLL_CTL_ADD, sockets[i], ep_events + i) < 0){
      error = 1;
      break;
    }
     events[i] = calloc(MAXEPOLLSIZE, sizeof(ep_events[i]));
     if (!events[i]){
       error = 1;
       break;
     }
     nbfd[i] = 1;
  }

  if (error){
    for (j=0; j <= i; ++j){
      close(sockets[j]);
      if (j < i)
        free(events[j]);
    }
    free(events);
    free(ep_events);
    free(epfds);
    free(nbfd);
    free(engine->sockets);
    engine->sockets = NULL;
    free(engine->ip);
    engine->ip = NULL;
    engine->nbip = 0;
    return NULL;
  }
  engine->nbfd = nbfd;
  engine->epfds = epfds;
  engine->ep_events = ep_events;
  engine->events = events;
  engine->sockets = sockets;
  engine->port = port;
  return engine;
}


int fetch_clients(dns_engine *eng)
{
  int nbfd;
  int clfd;
  struct sockaddr_storage cl_addr;
  socklen_t addr_size;

  for (int i=0; i < eng->nbip; ++i){
    nbfd = epoll_wait(eng->epfds[i], eng->events[i], eng->nbfd[i], -1);
    if (nbfd == -1)
      return -1;
    for (int j=0; j < nbfd; ++j){
      if (eng->events[i][j].data.fd == eng->sockets[i]){
        addr_size = sizeof(cl_addr);
        clfd = accept(eng->events[i][j].data.fd, (struct sockaddr*)&cl_addr,
                      &addr_size);
        if (clfd == -1)
          return -1;
        if (unblock_sock(clfd))
          return -1;
        eng->ep_events[i].events = EPOLLIN|EPOLLET;
        eng->ep_events[i].data.fd = clfd;
        if (epoll_ctl(eng->epfds[i], EPOLL_CTL_ADD, clfd, eng->ep_events + i)
            < 0)
          return -1;
        eng->nbfd[i] += 1;
      }
    }
  }
  return 0;
}

ssize_t dns_get(char **ptr, int socket)
{
  ssize_t cap = 4096;
  char *buf = malloc(sizeof(char) * cap);
  char *tbuf;
  ssize_t size = 0;
  ssize_t tmp;


  while ((tmp = read(socket, buf + size, cap / 2))){
    if (tmp <= 0)
    {
      free(buf);
      return -1;
    }
    size += tmp;
    if (size >= cap){
      tbuf = realloc(buf, cap * 2);
      if (!tbuf){
        free(buf);
        return -1;
      }
      buf = tbuf;
      cap *= 2;
    }
  }

  *ptr = buf;
  return size;
}

ssize_t dns_send(int socket, char *pck, size_t size)
{
  ssize_t written = write(socket, pck, size);
  return written;
}

void close_client(int epfd, int socket, struct epoll_event *ev)
{
  epoll_ctl(epfd, EPOLL_CTL_DEL, socket, ev);
  close(socket);
}

void close_serv(dns_engine *engine)
{
  for (int i=0; i < engine->nbip; ++i){
    for (int j=0; j < engine->nbfd[i]; ++j){
      epoll_ctl(engine->epfds[i], EPOLL_CTL_DEL, engine->events[i][j].data.fd,
                engine->ep_events + i);
      close(engine->events[i][j].data.fd);
    }
    free(engine->events[i]);
    close(engine->sockets[i]);
  }
  free(engine->events);
  free(engine->ep_events);
  free(engine->epfds);
  free(engine->nbfd);
  free(engine->sockets);
  engine->sockets = NULL;
  free(engine->ip);
  engine->ip = NULL;
  engine->nbip = 0;
}
