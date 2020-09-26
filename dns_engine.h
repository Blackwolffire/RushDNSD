#pragma once

#include "dns.h"

#define SOA_type (6)
#define A_type (1)
#define AAAA_type (28)
#define CNAME_type (5)
#define TXT_type (16)

typedef struct dns_engine dns_engine;
struct dns_engine {
    zone *dns_zone;
    uint16_t port;
    int *sockets;
    char **ip;
};

typedef struct bin_tree bin_tree;
struct bin_tree {
    bin_tree *son; //gauche
    bin_tree *bro; //droit
    char *name; //du noeud uniquement, sans les points sauf noeud pere
    zone *zone_list; //@ du premier
    size_t nb_zone;
}

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

dns_engine *dns_init(char *filename, uint16_t port, char *ip); // ip séparées
                                                              // par virgule
void *load_zone(char *filename); // TODO struct zone à mettre dans dnsengine
int dns_run(dns_engine *engine); // fork?NON! thread
void dns_quit(dns_engine *engine); //TODO struct dns_engine + dns_zone
