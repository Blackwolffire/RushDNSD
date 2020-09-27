#pragma once
#include <string.h>
#include <stdlib.h>
#include <stdint.h>
#include <arpa/inet.h>


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
        uint32_t ttl;
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
        dns_header head;
        question *quest;
        answer *answer;
        answer *authority;
        answer *additional;
};

