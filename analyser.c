#include "analyser.h"

int check_request(dns *request){
	return 0;	
}

// finds zones corresponding to the request
zone *find_zone(char *name, uint16_t type){
	return NULL;
}

// make the response
dns *make_response(zone **zone_list){
	return NULL;
}

uint16_t get_size(uint16_t type, void *data){
	if (type == SOA_type){
		SOA_data *soa = data;	
		uint16_t res = 0;
		res += strlen(soa->mname);
		res += strlen(soa->rname);
		res += sizeof(uint32_t) * 5;
		return res;
	}
	else // type is string
	{
		char *str = data;
		return strlen(str);
	}
}

dns *set_header_flags(dns *response, size_t tc, size_t rcode){
	uint16_t flags = 34048; // 1000 0101 0000 0000
	
	if (tc == 1)
		flags += 512; // 0000 00100 0000 0000

	flags += rcode;
	response->flags = flags;

	return response;
}

question *copy_question(question *question){
	question *question_cpy = malloc(sizeof(question));
	if (question_cpy == NULL)
		return NULL;
	size_t qname_len = strlen(question->qname);
	char *qname = malloc(sizeof(char) * (qname_len + 1))
	if (qname == NULL){
		free(question_cpy);
		return NULL;
	}

	strncpy(qname, question->qname, qname_len + 1);
	question_cpy->qname = qname;

	question_cpy->qtype = question->qtype;
	question_cpy->qclass = question->qclass;
	return question;
}

void free_question(question *question){
	free(question->qname);
	free(question);
	return;
}


dns *make_valid_response(dns *request){	

	// creqte response
	dns *response = malloc(sizeof(dns));
	if (response == NULL)
		return NULL;

	// copy questions
	



	uint16_t question_count = request->qdcount;
	
	answer *answers = calloc(question_count + 1, sizeof(anszer));
	if (answers == NULL)
		return NULL;

	for (uint16_t i = 0; i < question_count; i++){
		question *quest = request->quest[i];
	
	
		zone *zone = find_zone(quest->qname, quest->qtype);
		
		if (zone != NULL){
			answer *new_answer = answer[i];
			new_answer->rname = zone->name;
			new_answer->rtype = zone->type;
			new_answer->rclass = quest->qclass;
			new_answer->tll = zone->ttl;
			new_answer->rdlen = get_size(zone->type, zone->data);
			new_answer->rdata = zone->rdata;
		}
	}
}


dns *anayser(dns *request){
	if (check_request(request) == 0)
	{
		// Invalid request
		// forge response for invqlid request

	}
	else
	{
		// valid request
		return make_valid_response(request)
	}
	return NULL;
}

