#include "request_parser.h"
#include <stdlib.h>
#include <stdio.h>

int size_name(void *request, int index)
{
 char *name = (char *)request;
 save_index = index;
 while (name[index] != '\0')
 {
 	index = index + 1;
 }
 return index - save_index;
}

dns *request_parser(void *request, size_t buf_size)
{
// allocate dns request structure
dns *request_dns = malloc(1500);

// request_dns = (*request_dns)request;

// HEAD BLOCK
request_dns->head->id = request[0];
request_dns->head->flags = request[2];
request_dns->head->qdcount = request[4];
request_dns->head->ancount = request[6];
request_dns->head->nscount = request[8];
request_dns->head->arcount = request[10];

// QUESTION BLOCK
int len_qname = size_name(request,12);
int accu = len_qname;
request_dns->quest->qname = request[];// ?
request_dns->quest->qtype = request[12 + accu];
request_dns->quest->qclass = request[14 + accu];

// ANSWER BLOCK
int len_rname = size_name(request, (16 + len_qname));
accu = accu + len_rname;
request_dns->answer->rname = request[]; // ?
request_dns->answer->rtype = request[16 + accu];
request_dns->answer->rclass = request[18 + accu];
request_dns->answer->ttl = request[20 + accu];
request_dns->answer->rdlen = request[24 + accu];

int len_rdata = size_name(request, (26 + accu));
accu = accu + len_rdata;
request_dns->answer->rdata = request[]; // ?


// AUTHORITY BLOCK
int len_rname_auth = size_name(request, (26 + accu));
accu = accu + len_rname_auth;
request_dns->authority->rname = request[]; // ?
request_dns->authority->rtype = request[26 + accu];
request_dns->authority->rclass = request[28 + accu];
request_dns->authority->ttl = request[30 + accu];
request_dns->authority->rdlen = request[34 + accu];

int len_rdata_auth = size_name(request, (36 + accu));
accu = accu + len_rdata_auth;
request_dns->authority->rdata = request[]; // ?

// ADDITIONAL BLOCK
int len_rname_add = size_name(request, (36 + accu));
accu = accu + len_rname_add;
request_dns->additional->rname = request[]; // ?
request_dns->additional->rtype = request[36 + accu];
request_dns->additional->rclass = request[38 + accu];
request_dns->additional->ttl = request[40 + accu];
request_dns->additional->rdlen = request[44 + accu];

int len_rdata_add = size_name(request, (46 + accu));
accu = accu + len_rdata_add;
request_dns->additional->rdata = request[]; // ?

free(request);
return request_dns;
}

void printer(dns *dns_paquet)
{
 print("id : %d\n",dns_paquet->head->id);
  print("flags : %d\n",dns_paquet->head->flags);
  print("qdcount : %d\n", dns_paquet->head->qdcount);
  print("ancount : %d\n", dns_paquet->head->ancount);
  print("nscount : %d\n", dns_paquet->head->nscount);
  print("arcount : %d\n", dns_paquet->head->arcount);
 print("question block :\n");
  print("  qname : %s", dns_paquet->quest->qname);
  print("  qtype : %d", dns_paquet->quest->qtype);
  print("  qclass : %d", dns_paquet->quest->qclass);
 print("answer block :\n");
  print("  rname : %s", dns_paquet->answer->rname);
  print("  rtype : %d", dns_paquet->answer->rtype);
  print("  rclass : %d", dns_paquet->answer->rclass);
  print("  ttl : %d", dns_paquet->answer->ttl);
  print("  rdlen : %d", dns_paquet->answer->rdlen);
  print("  rdata : %s", dns_paquet->answer->rdata);
 print("authority block :\n");
  print("  rname : %s", dns_paquet->authority->rname);
  print("  rtype : %d", dns_paquet->authority->rtype);
  print("  rclass : %d", dns_paquet->authority->rclass);
  print("  ttl : %d", dns_paquet->authority->ttl);
  print("  rdlen : %d", dns_paquet->authority->rdlen);
  print("  rdata : %s", dns_paquet->authority->rdata);
 print("additional block :\n");
  print("  rname : %s", dns_paquet->additional->rname);
  print("  rtype : %d", dns_paquet->additional->rtype);
  print("  rclass : %d", dns_paquet->additional->rclass);
  print("  ttl : %d", dns_paquet->additional->ttl);
  print("  rdlen : %d", dns_paquet->additional->rdlen);
  print("  rdata : %s", dns_paquet->additional->rdata);
}





