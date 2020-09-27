#include "response_forger.h"


void *response_forge(dns *ans) {
    int total_len = sizeof(dns_header); // used to compute the size of the memory to allocate
    total_len += compute_strings_length(ans); // computes the size of the variable-length fields (string fields)
    total_len += 4 + (ans->head.ancount * 10) + (ans->head.nscount * 10) + (ans->head.arcount * 10); // computes the size of the fixed-size fields
    void *res = malloc(total_len); // allocates memory for the DNS packet

    // writes the different parts of the DNS packet
    void *p = forge_header(ans, res);
    p = forge_quest(ans, p);
    p = forge_answer(ans, p);
    p = forge_authority(ans, p);
    p = forge_additional(ans, p);


    return res;
}


// Computes the cumulated length of all of the string fields
int compute_strings_length(dns *ans) {

    // checks the need to add padding or not
    int quest_len = strlen(ans->quest->qname) + 1;
    if (quest_len % 2 != 0) {
        quest_len++;
    }

    int answ_len = 0; // for string fields of answers
    answer *answ_index = ans->answer; // used as an index to browse through the answers

    for (int c = 0 ; c < ans->head.ancount ; c++) {
    // checks the need to add padding or not
        int answ_rname_len = strlen(answ_index->rname) + 1;
        if (answ_rname_len % 2 != 0) {
            answ_rname_len++;
        }

    // checks the need to add padding or not
        int answ_rdlen = answ_index->rdlen + 1;
        if (answ_rdlen % 2 != 0) {
            answ_rdlen++;
        }

        answ_len += answ_rname_len + answ_rdlen;
        answ_index++;
    }

    int auth_len = 0; // for string fields of authority sections
    answer *auth_index = ans->authority; // used as an index to browse through the authority structs

    for (int c = 0 ; c < ans->head.nscount ; c++) {
    // checks the need to add padding or not
        int auth_rname_len = strlen(auth_index->rname) + 1;
        if (auth_rname_len % 2 != 0) {
            auth_rname_len++;
        }

    // checks the need to add padding or not
        int auth_rdlen = auth_index->rdlen + 1;
        if (auth_rdlen % 2 != 0) {
            auth_rdlen++;
        }
        auth_len += auth_rname_len + auth_rdlen;
        auth_index++;
    }

    int addi_len = 0; // for string fields of additional records
    answer *addi_index = ans->additional; // used as an index to browse through the additional records

    for (int c = 0 ; c < ans->head.arcount ; c++) {
    // checks the need to add padding or not
        int addi_rname_len = strlen(addi_index->rname) + 1;
        if (addi_rname_len % 2 != 0) {
            addi_rname_len++;
        }

    // checks the need to add padding or not
        int addi_rdlen = addi_index->rdlen + 1;
        if (addi_rdlen % 2 != 0) {
            addi_rdlen++;
        }
        addi_len += addi_rname_len + addi_rdlen;
        addi_index++;
    }

    return quest_len + answ_len + auth_len + addi_len;
}

// writes the header section in the DNS packet
void *forge_header(dns *ans, void *header) { // header = pointer on where the section will be written
    uint16_t *h_16b = header; // Writes 2 bytes at a time
    *h_16b = htons(ans->head.id);
    h_16b++;
    *h_16b = htons(ans->head.flags);
    h_16b++;
    *h_16b = htons(ans->head.qdcount);
    h_16b++;
    *h_16b = htons(ans->head.ancount);
    h_16b++;
    *h_16b = htons(ans->head.nscount);
    h_16b++;
    *h_16b = htons(ans->head.arcount);
    h_16b++;

    return h_16b;
}


// writes the question section in the DNS packet
void *forge_quest(dns *ans, void *quest) { // quest = pointer on where the section will be written
    char *q = quest; // Writes byte per byte
    int qname_len = strlen(ans->quest->qname) + 1;
    q = strncpy(q, (const char*)ans->quest->qname, qname_len);
    q += qname_len;

    // checks the need to add padding or not
    if (qname_len % 2 != 0) {
        *q = '\0';
        q++;
    }

    // Writes 2 bytes at a time
    uint16_t *q_16b = (uint16_t*)q;
    *q_16b = htons(ans->quest->qtype);
    q_16b++;
    *q_16b = htons(ans->quest->qclass);
    q_16b++;

    return q_16b;
}


// writes the answer section in the DNS packet
void *forge_answer(dns *ans, void *answ) { // answ = pointer on where the section will be written
    char *a = answ; // Writes byte per byte
    answer *answer_index = ans->answer;
    for (int i = 0 ; i < ans->head.ancount ; i++) {
        int rname_len = strlen(answer_index->rname) + 1;
        a = strncpy(a, (const char*)answer_index->rname, rname_len);
        a += rname_len;

    // checks the need to add padding or not
        if (rname_len % 2 != 0) {
            *a = '\0';
            a++;
        }

    // Writes 2 bytes at a time
        uint16_t *a_16b = (uint16_t*)a;
        *a_16b = htons(answer_index->rtype);
        a_16b++;
        *a_16b = htons(answer_index->rclass);
        a_16b++;

    // Writes 4 bytes at a time
        uint32_t *a_32b = (uint32_t*)a_16b;
        *a_32b = htonl(answer_index->ttl);
        a_32b++;

    // Writes 2 bytes at a time
        a_16b = (uint16_t*)a_32b;
        *a_16b = htons(answer_index->rdlen);
        a_16b++;

    // Writes byte per byte
        a = (char*)a_16b;
        a = strncpy(a, (const char*)answer_index->rdata, answer_index->rdlen + 1);
        a += answer_index->rdlen + 1;

    // checks the need to add padding or not
        if ((answer_index->rdlen + 1) % 2 != 0) {
            *a = '\0';
            a++;
        }

        answer_index++;
    }

    return a;
}


// writes the authority section in the DNS packet
void *forge_authority(dns *ans, void *auth) { // auth = pointer on where the section will be written
    char *au = auth; // Writes byte per byte
    answer *authority_index = ans->answer;
    for (int i = 0 ; i < ans->head.nscount ; i++) {
        int rname_len = strlen(authority_index->rname) + 1;
        au = strncpy(au, (const char*)authority_index->rname, rname_len);
        au += rname_len;

    // checks the need to add padding or not
        if (rname_len % 2 != 0) {
            *au = '\0';
            au++;
        }

    // Writes 2 bytes at a time
        uint16_t *au_16b = (uint16_t*)au;
        *au_16b = htons(authority_index->rtype);
        au_16b++;
        *au_16b = htons(authority_index->rclass);
        au_16b++;

    // Writes 4 bytes at a time
        uint32_t *au_32b = (uint32_t*)au_16b;
        *au_32b = htonl(authority_index->ttl);
        au_32b++;

    // Writes 2 bytes at a time
        au_16b = (uint16_t*)au_32b;
        *au_16b = htons(authority_index->rdlen);
        au_16b++;

    // Writes byte per byte
        au = (char*)au_16b;
        au = strncpy(au, (const char*)authority_index->rdata, authority_index->rdlen + 1);
        au += authority_index->rdlen + 1;

    // checks the need to add padding or not
        if ((authority_index->rdlen + 1) % 2 != 0) {
            *au = '\0';
            au++;
        }

        authority_index++;
    }

    return au;
}


// writes the additional records section in the DNS packet
void *forge_additional(dns *ans, void *addit) { // addit = pointer on where the section will be written
    char *ad = addit; // Writes byte per byte
    answer *additional_index = ans->answer;
    for (int i = 0 ; i < ans->head.arcount ; i++) {
        int rname_len = strlen(additional_index->rname) + 1;
        ad = strncpy(ad, (const char*)additional_index->rname, rname_len);
        ad += rname_len;

    // checks the need to add padding or not
        if (rname_len % 2 != 0) {
            *ad = '\0';
            ad++;
        }

    // Writes 2 bytes at a time
        uint16_t *ad_16b = (uint16_t*)ad;
        *ad_16b = htons(additional_index->rtype);
        ad_16b++;
        *ad_16b = htons(additional_index->rclass);
        ad_16b++;

    // Writes 4 bytes at a time
        uint32_t *ad_32b = (uint32_t*)ad_16b;
        *ad_32b = htonl(additional_index->ttl);
        ad_32b++;

    // Writes 2 bytes at a time
        ad_16b = (uint16_t*)ad_32b;
        *ad_16b = htons(additional_index->rdlen);
        ad_16b++;

    // Writes byte per byte
        ad = (char*)ad_16b;
        ad = strncpy(ad, (const char*)additional_index->rdata, additional_index->rdlen + 1);
        ad += additional_index->rdlen + 1;

    // checks the need to add padding or not
        if ((additional_index->rdlen + 1) % 2 != 0) {
            *ad = '\0';
            ad++;
        }

        additional_index++;
    }

    return ad;
}
