#pragma once

struct question {
char *qname;
uint16 qtype;
uint16 qclass;
}

struct answer {
char *rname;
uint16 rtype;
uint16 rclass;
uint32 ttl;
uint16 rdlen;
char *rdata; //char [rdlen]rdata
}

struct header {
uint16 id;
uint16 flags; // qr,opcode(4),aa,tc,rd,ra,z(3),rcode(4)
uint16 qdcount;
uint16 ancount;
uint16 nscount;
uint16 arcount;
}

struct dns {
header head;
question *quest;
an_au_add *answer;
an_au_add *authority;
an_au_add *additional;
}


