#include "request_parser.h"
#include <stdlib.h>

int main ()
{
 dns *dns_test = malloc(1500);
 //head
 dns_test->head.id = 1582;
 dns_test->head.flags = 34816;
 dns_test->head.qdcount = 1;
 dns_test->head.ancount = 1;
 dns_test->head.nscount = 1;
 dns_test->head.arcount = 1;
 //quest
 dns_test->quest->qname = "www.google.com";
 dns_test->quest->qtype = 1;
 dns_test->quest->qclass = 1;
 //answer
 dns_test->answer->rname = "www.google.com";
 dns_test->answer->rtype = 1;
 dns_test->answer->rclass = 1;
 dns_test->answer->ttl = 3;
 dns_test->answer->rdlen = 12;
 dns_test->answer->rdata = "205.35.65.25" ;
 //authority
 dns_test->answer->rname = "www.google.com";
 dns_test->answer->rtype = 1;
 dns_test->answer->rclass = 1;
 dns_test->answer->ttl = 3;
 dns_test->answer->rdlen = 12;
 dns_test->answer->rdata = "205.35.65.25" ;
 //additional
 dns_test->answer->rname = "www.google.com";
 dns_test->answer->rtype = 1;
 dns_test->answer->rclass = 1;
 dns_test->answer->ttl = 3;
 dns_test->answer->rdlen = 12;
 dns_test->answer->rdata = "205.35.65.25" ;
 void *paquet_test = response_forge(dns_test);
 dns *dns_response = request_parser(paquet_test);
 printer(dns_response);
}
