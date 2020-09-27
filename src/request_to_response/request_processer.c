#include "analyser.h"


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

dns *set_rcode_flag(dns *response, size_t rcode){
	//set rcode to 0
	uint16_t flags = response->head.flags;
	uint16_t rcode_mask = 15;
	flags = flags & (~rcode_mask);
	//add new rdcode
	response->head.flags = flags + rcode;
	return response;		
}

dns *set_tc_flag(dns *response, size_t tc){
	uint16_t flags = response->head.flags;
	uint16_t tc_mask = (tc > 0) << 9;
	response->head.flags = flags | tc_mask;
	return response;
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
int check_unknown_zone(char *soa, char *qname){
	size_t soa_size = strlen(soa);
	size_t qname_size = strlen(qname);
	if (soa_size < qname_size)
		return 1;
	return strcmp(soa, qname + (qname_size - soa_size));
}

int is_type_supported(uint16_t qtype){
	int res = 1;
	switch(qtype){
		case SOA_type :
		       break;	
		case A_type :
		       break;	
		case AAAA_type :
		       break;	
		case CNAME_type :
		       break;	
		case TXT_type :
		       break;	
		default :
		       res = 0;
		       break;
	}
	return res;
}

int is_class_supported(uint16_t class){
	if (class == 1) // class IN
		return 1;
	return 0;
}


uint16_t add_an(dns *response, zone *zone, uint16_t an_max){

	response->head.ancount += 1;
	uint16_t an_count = response->head.ancount;
	if (an_count >= an_max){
		an_max *= 2;
	 	answer * realloc_answer = realloc(response->answer, sizeof(answer) * an_max);
		if (realloc_answer == NULL){
			response->head.ancount -= 1;
			return an_max / 2;
		}
		response->answer = realloc_answer; 
	}

	answer *new_answer = (response->answer) + an_count;
	new_answer->rname = zone->name;
	new_answer->rtype = zone->type;
	new_answer->rclass = 1;
	new_answer->ttl = zone->ttl;
	new_answer->rdlen = get_data_size(zone->type, zone->data);
	new_answer->rdata = zone->data;

	return an_max;
}

uint16_t add_ns(dns *response, zone *zone, uint16_t ns_max){

	response->head.nscount += 1;
	uint16_t ns_count = response->head.nscount;
	if (ns_count >= ns_max){
		ns_max *= 2;
	 	answer * realloc_answer = realloc(response->authority, sizeof(answer) * ns_max);
		if (realloc_answer == NULL){
			response->head.nscount -= 1;
			return ns_max / 2;
		}
		response->authority = realloc_answer; 
	}

	answer *new_answer = (response->authority) + ns_count;
	new_answer->rname = zone->name;
	new_answer->rtype = zone->type;
	new_answer->rclass = 1;
	new_answer->ttl = zone->ttl;
	new_answer->rdlen = get_data_size(zone->type, zone->data);
	new_answer->rdata = zone->data;

	return ns_max;
}

uint16_t add_ar(dns *response, zone *zone, uint16_t ar_max){

	response->head.arcount += 1;
	uint16_t ar_count = response->head.arcount;
	if (ar_count >= ar_max){
		ar_max *= 2;
	 	answer * realloc_answer = realloc(response->additional, sizeof(answer) * ar_max);
		if (realloc_answer == NULL){
			response->head.arcount -= 1;
			return ar_max / 2;
		}
		response->additional = realloc_answer; 
	}

	answer *new_answer = (response->additional) + ar_count;
	new_answer->rname = zone->name;
	new_answer->rtype = zone->type;
	new_answer->rclass = 1;
	new_answer->ttl = zone->ttl;
	new_answer->rdlen = get_data_size(zone->type, zone->data);
	new_answer->rdata = zone->data;

	return ar_max;
}

dns *make_response(dns *request, dns *response, zone *soa, bin_tree *tree){

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
		uint16_t qtype = quest->qtype;
		uint16_t qclass = quest->qclass;
		if (is_type_supported(qtype) == 0){
			set_rcode_flag(response, 4); // Set NOTIMP not implemented
			break;
		}
		if (is_class_supported(qclass) == 0){
			set_rcode_flag(response, 4); // Set NOTIMP not implemented
			break;
		}


		if (check_unknown_zone(soa->name, request->quest->qname) != 0)
		{
			// Zone is unknown
			set_rcode_flag(response, 5); // Set to REFFUSED
			break;
		}

		zone_found zone = find_zone(tree, quest->qname, quest->qtype);
		if (zone.found != NULL){
			// Known zone and type
			set_rcode_flag(response, 0); // Set to NOERROR
			// ANSWER SECTION 
			an_max = add_an(response, zone.found, an_max);

		}
		else if (zone.found_no_type == 1){
			// zone found but not type
			// AUTHORITY SECTION
			set_rcode_flag(response, 0); // Set to NOERROR
			ns_max = add_ns(response, zone.found, ns_max);
		}
		else if (zone.found_no_terminal == 1) {
			// Emty non terminal
			// AUTHORITY SECTION
			set_rcode_flag(response, 0); // Set to NOERROR
			ns_max = add_ns(response, zone.found, ns_max);
		}
		else{
			// Not found
			// AUTHORITY SECTION
			set_rcode_flag(response, 3); // Set to NXDOMAIN	
			ns_max = add_ns(response, zone.found, ns_max);

		}
	}


	// set header falgs
	//
	// rcode :
	// 	0: NOERROR : if no error
	// 	1: format error in querry
	// 	2: server fail 
	// 	3: NXDOMAIN :Name does not exist
	// 	4: Not implented, server does not support query type
	// 	5: reffused ->zone inconu
	//
	//	Only accept type:
	//		SOA 6,
	//		A 1,
	//		AAAA 28,
	//		CNAME 5,
	//		TXT 16,
	//
	//		class:
	//		1 : IN 
	//
	return response;
}


dns *make_valid_response(dns *request, zone *soa, bin_tree *tree){	

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

	// fill responce and headers 
	if(make_response(request, response, soa, tree) == NULL){
		free(response->quest);
		free(response->answer);
		free(response->authority);
		free(response->additional);
		free(response);
		return NULL;
	}

	return response;
}
