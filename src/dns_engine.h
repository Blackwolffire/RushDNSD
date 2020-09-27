#pragma once

#include <stddef.h>
#include <stdint.h>
#include <stdio.h>
#include <string.h>
#include <sys/epoll.h>
#include <unistd.h>

#include "dns.h"

#define SOA_type (6)
#define A_type (1)
#define AAAA_type (28)
#define CNAME_type (5)
#define TXT_type (16)

typedef struct zone zone;
struct zone {
    char *name;
    uint16_t type;
    int ttl;
    void *data; //soit char*, soit struct SOA_data*
};

typedef struct dns_engine dns_engine;
struct dns_engine {
    zone *dns_zone;
    uint16_t port;
    int *sockets;
    char **ip;
    int nbip;
    int *epfds; //epoll file descriptors
    int *nbfd; // number of socket by epoll
    struct epoll_event *ep_events; // epoll events
    struct epoll_event **events;
};


typedef struct bin_tree bin_tree;
struct bin_tree {
    bin_tree *son; //gauche
    bin_tree *bro; //droit
    char *name; //du noeud uniquement, sans les points sauf noeud pere
    zone **zone_list; //@ du premier
    size_t nb_zone;
};

typedef struct SOA_data SOA_data;
struct SOA_data {
    char *mname;
    char *rname;
    uint32_t serial;
    uint32_t refresh;
    uint32_t retry;
    uint32_t expire;
    uint32_t minimum;
};


int error_file(char *msg);
bin_tree *load_zone(const char *filename);
bin_tree *create_tree(FILE *file);
bin_tree *add_to_tree(zone *new_zone, bin_tree *tree);
zone *get_zone(char *line);
SOA_data *get_soa_struct(char *word);
zone *check_new_zone(zone *new_zone);

//dns_engine *dns_init(char *filename, uint16_t port, char *ip); // ip séparées
                                                              // par virgule
//void *load_zone(char *filename); // TODO struct zone à mettre dans dnsengine
//int dns_run(dns_engine *engine); // fork?NON! thread
//void dns_quit(dns_engine *engine); //TODO struct dns_engine + dns_zone
