#pragma once

#include "dns_engine.h"

typedef struct question question;
struct question {
        char *qname;
        uint16_t qtype;
        uint16_t qclass;
};

typedef struct answer answer;
struct answer {
        char *rname;
        uint16_t rtype;
        uint16_t rclass;
        uint32 ttl;
        uint16_t rdlen;
        char *rdata; //char [rdlen]rdata
};

typedef struct dns_header dns_header;
struct dns_header {
        uint16_t id;
        uint16_t flags; // qr,opcode(4),aa,tc,rd,ra,z(3),rcode(4)
        uint16_t qdcount;
        uint16_t ancount;
        uint16_t nscount;
        uint16_t arcount;
};

typedef struct dns dns;
struct dns {
        header head;
        question *quest;
        an_au_add *answer;
        an_au_add *authority;
        an_au_add *additional;
};


// dns_engine.h
dns_engine *dns_init(char *filename, uint16_t port, char *ip); // ip séparées
                                                              // par virgule
void *load_zone(char *filename); // TODO struct zone à mettre dans dnsengine
int dns_run(dns_engine *engine); // fork?NON! thread
void dns_quit(dns_engine *engine); //TODO struct dns_engine

// request_parser.h
dns *request_parser(void *request, size_t buf_size);

// client_handler.h
dns *get_request(dns_engine *engine); // queue add chez le networkwrapper?
dns *request_process(dns *req);

// response_forger.h
void *response_forge(dns *ans); // struct -> packet (void*)

// network_wrapper.h
void *init_serv(dns_engine *engine, uint16_t port, char *ip);
void *dns_get(dns_engine *engine);
void dns_send(dns_engine *engine, void *pck);
void *close_serv(dns_engine *engine);
