#include "analyser.h"

int check_request(dns *request){
	return 0;	
}

// finds zones corresponding to the request
zone *find_zone(char *name, uint16_t type){
	return NULL;
}

uint16_t get_data_size(uint16_t type, void *data){
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
	response->head.flags = flags;

	return response;
}

question *copy_question(question *quest, question *question_cpy){
	
	size_t qname_len = strlen(quest->qname);
	char *qname = malloc(sizeof(char) * (qname_len + 1));
	if (qname == NULL){
		return NULL;
	}

	strncpy(qname, quest->qname, qname_len + 1);
	question_cpy->qname = qname;

	question_cpy->qtype = quest->qtype;
	question_cpy->qclass = quest->qclass;
	return question_cpy;
}

void free_question(question *question){
	free(question->qname);
	return;
}

void free_questions(question *questions, uint16_t question_nb){
	for (uint16_t i = 0; i < question_nb; i++){
		free_question(questions + i);
	}
	return;
}

question *copy_questions(question *quest, uint16_t question_nb){
	question *questions_cpy = malloc(sizeof(question) * question_nb);
	if (questions_cpy == NULL)
		return NULL;
	for (uint16_t i = 0; i < question_nb; i++){
		question *quest_cpy = copy_question(quest + i, questions_cpy + i);
		if (quest_cpy == NULL){
			free_questions(questions_cpy, i);
			return NULL;
		}
	}
	return questions_cpy;
}

dns *make_response(dns *request, dns *response){

	uint16_t an_count = 0;
	uint16_t ns_count = 0;
	uint16_t ar_count = 0;

	uint16_t an_max = 4;
	uint16_t ns_max = 4;
	uint16_t ar_max = 4;

	answer *an_answers = calloc(an_max, sizeof(answer));
	if (an_answers == NULL){
		free(response);
		return NULL;
	}

	answer *ns_answers = calloc(ns_max, sizeof(answer));
	if (ns_answers == NULL){
		free(response);
		free(an_answers);
		return NULL;
	}
	
	answer *ar_answers = calloc(ar_max, sizeof(answer));
	if (ar_answers == NULL){
		free(response);
		free(an_answers);
		free(ns_answers);
		return NULL;
	}
	
	for (uint16_t i = 0; i < request->head.qdcount; i++){
		question *quest = (request->quest) + i;	
		zone *zone = find_zone(quest->qname, quest->qtype);	
		if (zone != NULL){
			// set answer depending on type (Nom present/type qbsent)
		}
		else {
			// set answer to type depending (Non existant/ Emtynon terminal/ zone inconnue)
		}
	}
	return response;
}


dns *make_valid_response(dns *request){	

	// creqte response
	dns *response = malloc(sizeof(dns));
	if (response == NULL)
		return NULL;

	// copy questions	
	uint16_t question_count = request->head.qdcount;
	question *questions = copy_questions(request->quest, question_count);
	if (questions == NULL){
		free(response);
		return NULL;
	}
	response->quest = questions;

	// fill responces 
	if(make_response(request, response) == NULL){
		free(response->quest);
		free(response->answer);
		free(response->authority);
		free(response->additional);
		free(response);
		return NULL;
	}
	return response;
}
