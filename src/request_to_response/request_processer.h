#pragma once
#include "analyser.h"


// Gets the data type depending on type.
uint16_t get_data_size(uint16_t type, void *data);

// Set header flag tc and rcode.
dns *set_header_flags(dns *response, size_t tc, size_t rcode);

// Set header flag rcode.
dns *set_rcode_flag(dns *response, size_t rcode);

// Set header flag tc.
dns *set_tc_flag(dns *response, size_t tc);

// Copy a question values.
question *copy_question(question *quest, question *question_cpy);

// free allocated variables in a question.
void free_question(question *question);

// Copy consecutive questions.
question *copy_questions(question *quest, uint16_t question_nb);

// Free allocated variables in a number of consecutive questions.
void free_questions(question *questions, uint16_t question_nb);

// Makes dns responses in dns_response depending on dns_request 
dns *make_response(dns *request, dns *response, zone *soa, bin_tree *tree);

// Make avaidresponse
dns *make_valid_response(dns *request, zone *soa, bin_tree *tree);

// Make response fonction for theother  type of responses (errors).
