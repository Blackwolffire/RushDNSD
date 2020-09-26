#include "response_forger.h"


void *response_forge(dns *ans) {
    int total_len = sizeof(dns_header);
    total_len += compute_strings_length(ans);
    total_len += 4 + (ans->head.ancount * 10) + (ans->head.nscount * 10) + (ans->head.arcount * 10);
    void *res = malloc(total_len);
    void *p = forge_header(ans, res);
    p = forge_quest(ans, p);

    return res;
}

int compute_strings_length(dns *ans) {
    int quest_len = strlen(ans->quest->qname);

    int answ_len = 0;
    answer *answ_index = ans->answer;

    for (int c = 0 ; c < ans->head.ancount ; c++) {
        answ_len += strlen(answ_index->rname) + answ_index->rdlen;
        answ_index++;
    }

    int auth_len = 0;
    answer *auth_index = ans->authority;

    for (int c = 0 ; c < ans->head.nscount ; c++) {
        auth_len += strlen(auth_index->rname) + auth_index->rdlen;
        auth_index++;
    }

    int addi_len = 0;
    answer *addi_index = ans->additional;

    for (int c = 0 ; c < ans->head.arcount ; c++) {
        addi_len += strlen(addi_index->rname) + addi_index->rdlen;
        addi_index++;
    }

    return quest_len + answ_len + auth_len + addi_len;
}

void *forge_header(dns *ans, dns_header *h) {
    *h = ans->head;
    // little endian to big endian : htons
    h++;
    return h;
}

void *forge_quest(dns *ans, question *q) {
    strncpy((char*)q, (const char*)ans->quest->qname, strlen(ans->quest->qname));
    q->qtype = ans->quest->qtype;
    q->qclass = ans->quest->qclass;
    q++;

    return q;
}

/*
void *forge_answer(dns *ans, answer *a) {
    for (answ_index = 0 ; answ_index < answ_index->rdlen ; answ_index++) {
        
    }
}*/
