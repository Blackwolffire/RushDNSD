#include "request_parser.h"
#include "response_forger.h"
#include <stdlib.h>

int main ()
{
 dns *dns_test = malloc(sizeof(struct dns));
 dns_test->quest = malloc(sizeof(struct question));
 dns_test->answer = malloc(sizeof(struct answer));
 dns_test->authority = malloc(sizeof(struct answer));
 dns_test->additional = malloc(sizeof(struct answer));
 //head
 dns_test->head.id = 1582;
 dns_test->head.flags = 34816;
 dns_test->head.qdcount = 1;
 dns_test->head.ancount = 1;
 dns_test->head.nscount = 1;
 dns_test->head.arcount = 1;
 //quest
 dns_test->quest->qname = malloc(15);
 dns_test->quest->qname = "www.google.com";
 dns_test->quest->qtype = 1;
 dns_test->quest->qclass = 1;
 //answer
 dns_test->answer->rname = malloc(15);
 dns_test->answer->rname = "www.google.com";
 dns_test->answer->rtype = 1;
 dns_test->answer->rclass = 1;
 dns_test->answer->ttl = 3;
 dns_test->answer->rdlen = 12;
 dns_test->answer->rdata = malloc(13);
 dns_test->answer->rdata = "205.35.65.25";
 //authority
 dns_test->authority->rname = malloc(15);
 dns_test->authority->rname = "www.google.com";
 dns_test->authority->rtype = 1;
 dns_test->authority->rclass = 1;
 dns_test->authority->ttl = 3;
 dns_test->authority->rdlen = 12;
 dns_test->authority->rdata = malloc(13);
 dns_test->authority->rdata = "205.35.65.25";
 //additional
 dns_test->additional->rname = malloc(15);
 dns_test->additional->rname = "www.google.com";
 dns_test->additional->rtype = 1;
 dns_test->additional->rclass = 1;
 dns_test->additional->ttl = 3;
 dns_test->additional->rdlen = 12;
 dns_test->additional->rdata = malloc(13);
 dns_test->additional->rdata = "205.35.65.25";
 void *paquet_test = response_forge(dns_test);
 dns *dns_response = request_parser(paquet_test,50);
 printer(dns_response);
}
