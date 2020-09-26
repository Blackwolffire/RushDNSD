#include "request_parser.h"
#include <stdlib.h>
#include <stdio.h>

//fill qname,rname and rdata fields with padding
//return size of field
int size_name(char *name, int index, char *request_dns_name)
{
 int save_index = index;
 while (name[index] != '\0')
 {
	request_dns_name = request_dns_name + name[index];
 	index = index + 1;
 }
 request_dns_name = request_dns_name + '\0';
 if (index % 2 != 0)
 {
 	index = index + 1;
	request_dns_name = request_dns_name + '\0';
 } 
 return index - save_index;
}


dns *request_parser(void *request_void)
{
// allocate dns request structure
dns *request_dns = malloc(1500);

//cast void * in raw of Bytes
char *request = (char *)request_void;
// request_dns = (*request_dns)request;

// HEAD BLOCK
request_dns->head.id = (uint16_t)request[0];
request_dns->head.flags = (uint16_t)request[2];
request_dns->head.qdcount = (uint16_t)request[4];
request_dns->head.ancount = (uint16_t)request[6];
request_dns->head.nscount = (uint16_t)request[8];
request_dns->head.arcount = (uint16_t)request[10];

// QUESTION BLOCK
int len_qname = size_name(request,12,request_dns->quest->qname);
int accu = len_qname;
request_dns->quest->qtype = (uint16_t)request[12 + accu];
request_dns->quest->qclass = (uint16_t)request[14 + accu];

// ANSWER BLOCK
int len_rname = size_name(request, (16 + accu), request_dns->answer->rname);
accu = accu + len_rname;
request_dns->answer->rtype = (uint16_t)request[16 + accu];
request_dns->answer->rclass = (uint16_t)request[18 + accu];
request_dns->answer->ttl =(uint32_t) request[20 + accu];
request_dns->answer->rdlen = (uint16_t)request[24 + accu];

int len_rdata = size_name(request, (26 + accu), request_dns->answer->rdata);
accu = accu + len_rdata;

// AUTHORITY BLOCK
int len_rname_auth = size_name(request, (26 + accu), request_dns->authority->rname);
accu = accu + len_rname_auth;
request_dns->authority->rtype = (uint16_t)request[26 + accu];
request_dns->authority->rclass = (uint16_t)request[28 + accu];
request_dns->authority->ttl = (uint32_t)request[30 + accu];
request_dns->authority->rdlen = (uint16_t)request[34 + accu];

int len_rdata_auth = size_name(request, (36 + accu), request_dns->authority->rdata);
accu = accu + len_rdata_auth;

// ADDITIONAL BLOCK
int len_rname_add = size_name(request, (36 + accu), request_dns->additional->rname);
accu = accu + len_rname_add;
request_dns->additional->rtype = (uint16_t)request[36 + accu];
request_dns->additional->rclass = (uint16_t)request[38 + accu];
request_dns->additional->ttl = (uint32_t)request[40 + accu];
request_dns->additional->rdlen = (uint16_t)request[44 + accu];

int len_rdata_add = size_name(request, (46 + accu), request_dns->additional->rdata);
accu = accu + len_rdata_add;

free(request);
return request_dns;
}

//display dns_paquet
void printer(dns *dns_paquet)
{
 printf("id : %d\n",dns_paquet->head.id);
  printf("flags : %d\n",dns_paquet->head.flags);
  printf("qdcount : %d\n", dns_paquet->head.qdcount);
  printf("ancount : %d\n", dns_paquet->head.ancount);
  printf("nscount : %d\n", dns_paquet->head.nscount);
  printf("arcount : %d\n", dns_paquet->head.arcount);
 printf("question block :\n");
  printf("  qname : %s", dns_paquet->quest->qname);
  printf("  qtype : %d", dns_paquet->quest->qtype);
  printf("  qclass : %d", dns_paquet->quest->qclass);
 printf("answer block :\n");
  printf("  rname : %s", dns_paquet->answer->rname);
  printf("  rtype : %d", dns_paquet->answer->rtype);
  printf("  rclass : %d", dns_paquet->answer->rclass);
  printf("  ttl : %d", dns_paquet->answer->ttl);
  printf("  rdlen : %d", dns_paquet->answer->rdlen);
  printf("  rdata : %s", dns_paquet->answer->rdata);
 printf("authority block :\n");
  printf("  rname : %s", dns_paquet->authority->rname);
  printf("  rtype : %d", dns_paquet->authority->rtype);
  printf("  rclass : %d", dns_paquet->authority->rclass);
  printf("  ttl : %d", dns_paquet->authority->ttl);
  printf("  rdlen : %d", dns_paquet->authority->rdlen);
  printf("  rdata : %s", dns_paquet->authority->rdata);
 printf("additional block :\n");
  printf("  rname : %s", dns_paquet->additional->rname);
  printf("  rtype : %d", dns_paquet->additional->rtype);
  printf("  rclass : %d", dns_paquet->additional->rclass);
  printf("  ttl : %d", dns_paquet->additional->ttl);
  printf("  rdlen : %d", dns_paquet->additional->rdlen);
  printf("  rdata : %s", dns_paquet->additional->rdata);
}





