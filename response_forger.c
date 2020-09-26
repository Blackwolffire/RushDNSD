#include "response_forger.h"


void *response_forge(dns *ans) {
    int total_len = sizeof(dns_header);
    total_len += compute_strings_length(ans);
    total_len += 4 + (ans->head.ancount * 10) + (ans->head.nscount * 10) + (ans->head.arcount * 10);
    void *res = malloc(total_len);
    void *p = forge_header(ans, res);

    p = forge_quest(ans, p);
    p = forge_answer(ans, p);
    p = forge_authority(ans, p);
    p = forge_additional(ans, p);

    return res;
}


int compute_strings_length(dns *ans) {
    int quest_len = strlen(ans->quest->qname) + 1;
    if (quest_len % 2 != 0) {
        quest_len++;
    }

    int answ_len = 0;
    answer *answ_index = ans->answer;

    for (int c = 0 ; c < ans->head.ancount ; c++) {
        int answ_rname_len = strlen(answ_index->rname) + 1;
        if (answ_rname_len % 2 != 0) {
            answ_rname_len++;
        }

        int answ_rdlen = answ_index->rdlen + 1;
        if (answ_rdlen % 2 != 0) {
            answ_rdlen++;
        }

        answ_len += answ_rname_len + answ_rdlen;
        answ_index++;
    }

    int auth_len = 0;
    answer *auth_index = ans->authority;

    for (int c = 0 ; c < ans->head.nscount ; c++) {
        int auth_rname_len = strlen(auth_index->rname) + 1;
        if (auth_rname_len % 2 != 0) {
            auth_rname_len++;
        }

        int auth_rdlen = auth_index->rdlen + 1;
        if (auth_rdlen % 2 != 0) {
            auth_rdlen++;
        }
        auth_len += auth_rname_len + auth_rdlen;
        auth_index++;
    }

    int addi_len = 0;
    answer *addi_index = ans->additional;

    for (int c = 0 ; c < ans->head.arcount ; c++) {
        int addi_rname_len = strlen(addi_index->rname) + 1;
        if (addi_rname_len % 2 != 0) {
            addi_rname_len++;
        }

        int addi_rdlen = addi_index->rdlen + 1;
        if (addi_rdlen % 2 != 0) {
            addi_rdlen++;
        }
        addi_len += addi_rname_len + addi_rdlen;
        addi_index++;
    }

    return quest_len + answ_len + auth_len + addi_len;
}


void *forge_header(dns *ans, void *header) {
    uint16_t *h_16b = header;
    *h_16b = ans->head.id;
    h_16b++;
    *h_16b = ans->head.flags;
    h_16b++;
    *h_16b = ans->head.qdcount;
    h_16b++;
    *h_16b = ans->head.ancount;
    h_16b++;
    *h_16b = ans->head.nscount;
    h_16b++;
    *h_16b = ans->head.arcount;
    h_16b++;

    return h_16b;
}


void *forge_quest(dns *ans, void *quest) {
    char *q = quest;
    int qname_len = strlen(ans->quest->qname) + 1;
    q = strncpy(q, (const char*)ans->quest->qname, qname_len);
    q += qname_len;
    if (qname_len % 2 != 0) {
        *q = '\0';
        q++;
    }

    uint16_t *q_16b = (uint16_t*)q;
    *q_16b = ans->quest->qtype;
    q_16b++;
    *q_16b = ans->quest->qclass;
    q_16b++;

    return q_16b;
}


void *forge_answer(dns *ans, void *answ) {
    char *a = answ;
    answer *answer_index = ans->answer;
    for (int i = 0 ; i < ans->head.ancount ; i++) {
        int rname_len = strlen(answer_index->rname) + 1;
        a = strncpy(a, (const char*)answer_index->rname, rname_len);
        a += rname_len;
        if (rname_len % 2 != 0) {
            *a = '\0';
            a++;
        }

        uint16_t *a_16b = (uint16_t*)a;
        *a_16b = answer_index->rtype;
        a_16b++;
        *a_16b = answer_index->rclass;
        a_16b++;

        uint32_t *a_32b = (uint32_t*)a_16b;
        *a_32b = answer_index->ttl;
        a_32b++;

        a_16b = (uint16_t*)a_32b;
        *a_16b = answer_index->rdlen;
        a_16b++;

        a = (char*)a_16b;
        a = strncpy(a, (const char*)answer_index->rdata, answer_index->rdlen + 1);
        a += answer_index->rdlen + 1;
        if ((answer_index->rdlen + 1) % 2 != 0) {
            *a = '\0';
            a++;
        }

        answer_index++;
    }

    return a;
}


void *forge_authority(dns *ans, void *auth) {
    char *au = auth;
    answer *authority_index = ans->answer;
    for (int i = 0 ; i < ans->head.nscount ; i++) {
        int rname_len = strlen(authority_index->rname) + 1;
        au = strncpy(au, (const char*)authority_index->rname, rname_len);
        au += rname_len;
        if (rname_len % 2 != 0) {
            *au = '\0';
            au++;
        }

        uint16_t *au_16b = (uint16_t*)au;
        *au_16b = authority_index->rtype;
        au_16b++;
        *au_16b = authority_index->rclass;
        au_16b++;

        uint32_t *au_32b = (uint32_t*)au_16b;
        *au_32b = authority_index->ttl;
        au_32b++;

        au_16b = (uint16_t*)au_32b;
        *au_16b = authority_index->rdlen;
        au_16b++;

        au = (char*)au_16b;
        au = strncpy(au, (const char*)authority_index->rdata, authority_index->rdlen + 1);
        au += authority_index->rdlen + 1;
        if ((authority_index->rdlen + 1) % 2 != 0) {
            *au = '\0';
            au++;
        }

        authority_index++;
    }

    return au;
}


void *forge_additional(dns *ans, void *addit) {
    char *ad = addit;
    answer *additional_index = ans->answer;
    for (int i = 0 ; i < ans->head.arcount ; i++) {
        int rname_len = strlen(additional_index->rname) + 1;
        ad = strncpy(ad, (const char*)additional_index->rname, rname_len);
        ad += rname_len;
        if (rname_len % 2 != 0) {
            *ad = '\0';
            ad++;
        }

        uint16_t *ad_16b = (uint16_t*)ad;
        *ad_16b = additional_index->rtype;
        ad_16b++;
        *ad_16b = additional_index->rclass;
        ad_16b++;

        uint32_t *ad_32b = (uint32_t*)ad_16b;
        *ad_32b = additional_index->ttl;
        ad_32b++;

        ad_16b = (uint16_t*)ad_32b;
        *ad_16b = additional_index->rdlen;
        ad_16b++;

        ad = (char*)ad_16b;
        ad = strncpy(ad, (const char*)additional_index->rdata, additional_index->rdlen + 1);
        ad += additional_index->rdlen + 1;
        if ((additional_index->rdlen + 1) % 2 != 0) {
            *ad = '\0';
            ad++;
        }

        additional_index++;
    }

    return ad;
}
