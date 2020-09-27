#include "request_parser.h"
#include <stdlib.h>
#include <stdio.h>
#include <arpa/inet.h>

//fill qname and rname fields in question, answer type struct
int size_name(uint16_t *name, int index, char *request_dns_name)
{
	char *name_char = (char *)name;
	int index_char = index * 2 ;
	int save_index = index_char;
	char *tmp_request_dns_name = request_dns_name;
	while (name_char[index_char] != '\0')
	{
		*request_dns_name = name_char[index_char];
		request_dns_name = request_dns_name + 1;
		index = index + 1;
		index_char = index_char + 1;
	}
	request_dns_name = request_dns_name + '\0';
	index = index + 1;
	index_char = index_char + 1;
	if ((index_char - save_index)  % 2 != 0)
	{
		index = index + 1;
		index_char = index_char + 1;
		request_dns_name = request_dns_name + '\0';
	} 
	request_dns_name = tmp_request_dns_name;
	return (index_char - save_index) / 2;
}

//parse categorie answer, authority, additional in dns struct
int answer_auth_add(uint16_t *request, answer *dns_a, int accu, int count)
{
  for (int j = 0; j < count; j = j + 1)
  {
	dns_a->rname = malloc(1024);
	int len_rname = size_name(request, accu, dns_a->rname);
	accu = accu + len_rname;
	dns_a->rtype = ntohs(request[accu]);
	dns_a->rclass = ntohs(request[1 + accu]);
	dns_a->ttl = ntohl(request[2 + accu]);
	dns_a->rdlen = ntohs(request[4 + accu]);
	dns_a->rdata = malloc(1024);
	int len_rdata = size_name(request, (5 + accu), dns_a->rdata);
	accu = accu + len_rdata + 5;
	dns_a = dns_a + 1;
  }
  return accu;
}

//parse all buffer in a dns struct
dns *request_parser(void *request_void, int buffer_size)
{
	//verify if buffer contain an head block
	if (buffer_size < 12)
	{
		printf("Buffer received is invalid !");
		return NULL;
	}

	// allocate dns request structure
	dns *request_dns = malloc(sizeof(struct dns));

	//cast void * in raw of Bytes
        uint16_t *request = (uint16_t *)request_void;
	// HEAD BLOCK
	request_dns->head.id = ntohs(request[0]);
	request_dns->head.flags = ntohs(request[1]);
	request_dns->head.qdcount = ntohs(request[2]);
	request_dns->head.ancount = ntohs(request[3]);
	request_dns->head.nscount = ntohs(request[4]);
	request_dns->head.arcount = ntohs(request[5]);
	// index parse begin at 6 after parse head
	int accu = 6;

	// QUESTION BLOCK
	if (request_dns->head.qdcount == 0)
		request_dns->quest = NULL;
	else
	{
		request_dns->quest = malloc(sizeof(struct question));
		request_dns->quest->qname = malloc(1024);
		int len_qname = size_name(request,6,request_dns->quest->qname);
		accu = accu + len_qname;
		request_dns->quest->qtype = ntohs(request[accu]);
		request_dns->quest->qclass = ntohs(request[1 + accu]);
		accu = accu + 2;
	}


	// ANSWER BLOCK
	if (request_dns->head.ancount == 0)
		request_dns->answer = NULL;
	else
	{
		request_dns->answer = malloc(sizeof(struct answer) * request_dns->head.ancount);
		answer *dns_save_answer = request_dns->answer; 
		accu = answer_auth_add(request, request_dns->answer, accu, request_dns->head.ancount);
		request_dns->answer = dns_save_answer;
	}

	// AUTHORITY BLOCK
	if (request_dns->head.nscount == 0)
		request_dns->authority = NULL;
	else
	{
		request_dns->authority = malloc(sizeof(struct answer) * request_dns->head.nscount);
		answer *dns_save_authority = request_dns->authority;
		accu = answer_auth_add(request, request_dns->authority, accu, request_dns->head.nscount);
		request_dns->authority = dns_save_authority;
	}

	// ADDITIONAL BLOCK
	if (request_dns->head.nscount == 0)
		request_dns->additional = NULL;
	else
	{
		request_dns->additional = malloc(sizeof(struct answer) * request_dns->head.arcount);
		answer *dns_save_additional = request_dns->additional;
		accu = answer_auth_add(request, request_dns->additional, accu, request_dns->head.arcount);
		request_dns->additional = dns_save_additional;
	}
	free(request);
	return request_dns;
}

//display a dns struct
/*
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
	 printf("  qname : %s\n", dns_paquet->quest->qname);
	 printf("  qtype : %d\n", dns_paquet->quest->qtype);
	 printf("  qclass : %d\n", dns_paquet->quest->qclass);
	printf("answer block :\n");
	 printf("  rname : %s\n", dns_paquet->answer->rname);
	 printf("  rtype : %d\n", dns_paquet->answer->rtype);
	 printf("  rclass : %d\n", dns_paquet->answer->rclass);
	 printf("  ttl : %d\n", dns_paquet->answer->ttl);
	 printf("  rdlen : %d\n", dns_paquet->answer->rdlen);
	 printf("  rdata : %s\n", dns_paquet->answer->rdata);
	printf("authority block :\n");
	 printf("  rname : %s\n", dns_paquet->authority->rname);
	 printf("  rtype : %d\n", dns_paquet->authority->rtype);
	 printf("  rclass : %d\n", dns_paquet->authority->rclass);
	 printf("  ttl : %d\n", dns_paquet->authority->ttl);
	 printf("  rdlen : %d\n", dns_paquet->authority->rdlen);
	 printf("  rdata : %s\n", dns_paquet->authority->rdata);
	printf("additional block :\n");
	 printf("  rname : %s\n", dns_paquet->additional->rname);
	 printf("  rtype : %d\n", dns_paquet->additional->rtype);
	 printf("  rclass : %d\n", dns_paquet->additional->rclass);
	 printf("  ttl : %d\n", dns_paquet->additional->ttl);
	 printf("  rdlen : %d\n", dns_paquet->additional->rdlen);
	 printf("  rdata : %s\n", dns_paquet->additional->rdata);
	free_dns_struct(dns_paquet);
}
*/

//free answer, authority and additional struct
void free_repete(answer *answer_free, int count)
{
	for(int i = 0; i < count; i = i + 1)
	{
		answer *tmp = answer_free;
		if (answer_free->rname != NULL)
		{
			free(answer_free->rname);
			free(answer_free->rdata);
		}
		answer_free = answer_free + 1;
		free(tmp);
	}
}

//free global dns struct
void free_dns_struct(dns *dns_free)
{
if(dns_free != NULL)
{
 if(dns_free->quest->qname != NULL)
 	free(dns_free->quest->qname);
 if(dns_free->quest != NULL)
	free(dns_free->quest);
 if(dns_free->answer != NULL)
 	free_repete(dns_free->answer, dns_free->head.ancount);
 if(dns_free->authority != NULL)
 	free_repete(dns_free->authority, dns_free->head.nscount);
 if(dns_free->additional != NULL)
 	free_repete(dns_free->additional, dns_free->head.arcount);
 free(dns_free);
}
}



