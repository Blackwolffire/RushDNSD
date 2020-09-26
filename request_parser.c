#include "request_parser.h"
#include <stdlib.h>
#include <stdio.h>

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

int answer_auth_add(char *request, answer *dns_a, int accu, int count)
{
  for (int j = 0; j < count; j = j + 1)
  {
	dns_a->rname = malloc(1024);
	int len_rname = size_name(request, accu, dns_a->rname);
	accu = accu + len_rname;
	dns_a->rtype = (uint16_t)request[accu];
	dns_a->rclass = (uint16_t)request[2 + accu];
	dns_a->ttl =(uint32_t) request[4 + accu];
	dns_a->rdlen = (uint16_t)request[8 + accu];

	int len_rdata = size_name(request, (10 + accu), dns_a->rdata);
	accu = accu + len_rdata + 10;
	dns_a = dns_a + 1;
  }
  return accu;
}

dns *request_parser(void *request_void, int buffer_size)
{
	if (buffer_size < 12)
	{
		printf("Buffer received is invalid !");
		return NULL;
	}

	// allocate dns request structure
	dns *request_dns = malloc(sizeof(struct dns *));

	//cast void * in raw of Bytes
	char *request = (char *)request_void;

	// HEAD BLOCK
	request_dns->head.id = (uint16_t)request[0];
	request_dns->head.flags = (uint16_t)request[2];
	request_dns->head.qdcount = (uint16_t)request[4];
	request_dns->head.ancount = (uint16_t)request[6];
	request_dns->head.nscount = (uint16_t)request[8];
	request_dns->head.arcount = (uint16_t)request[10];

	int accu = 12;

	// QUESTION BLOCK
	if (request_dns->head.qdcount == 0)
		request_dns->quest = NULL;
	else
	{
		request_dns->quest = malloc(sizeof(struct question *));

		int len_qname = size_name(request,12,request_dns->quest->qname);
		accu = accu + len_qname;
		request_dns->quest->qtype = (uint16_t)request[accu];
		request_dns->quest->qclass = (uint16_t)request[2 + accu];
		accu = accu + 4;
	}


	// ANSWER BLOCK
	if (request_dns->head.ancount == 0)
		request_dns->answer = NULL;
	else
	{
		request_dns->answer = malloc(sizeof(struct answer *) * request_dns->head.ancount);

		answer *dns_save_answer = request_dns->answer; 
		accu = answer_auth_add(request, request_dns->answer, accu, request_dns->head.ancount);
		request_dns->answer = dns_save_answer;
	}
	// AUTHORITY BLOCK
	if (request_dns->head.nscount == 0)
		request_dns->authority = NULL;
	else
	{
		request_dns->authority = malloc(sizeof(struct answer *) * request_dns->head.nscount);
		answer *dns_save_authority = request_dns->authority;
		accu = answer_auth_add(request, request_dns->authority, accu, request_dns->head.nscount);
		request_dns->authority = dns_save_authority;
	}
	// ADDITIONAL BLOCK
	if (request_dns->head.nscount == 0)
		request_dns->additional = NULL;
	else
	{
		request_dns->additional = malloc(sizeof(struct answer *) * request_dns->head.arcount);
		answer *dns_save_additional = request_dns->additional;
		accu = answer_auth_add(request, request_dns->additional, accu, request_dns->head.arcount);
		request_dns->additional = dns_save_additional;
	}

	free(request);
	return request_dns;
}

void printer(dns *dns_paquet)
{
	printf("header block :\n");
	 printf("  id : %d\n",dns_paquet->head.id);
	 printf("  flags : %d\n",dns_paquet->head.flags);
	 printf("  qdcount : %d\n", dns_paquet->head.qdcount);
	 printf("  ancount : %d\n", dns_paquet->head.ancount);
	 printf("  nscount : %d\n", dns_paquet->head.nscount);
	 printf("  arcount : %d\n", dns_paquet->head.arcount);
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





