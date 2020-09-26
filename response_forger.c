#include "response_forger.h"


void *response_forge(dns *ans) {
    void *res = malloc(sizeof(header + question + (answer * 3))); // FAUT CONNAITRE LA TAILLE DES CHAR*
    void *p = forge_header(ans, res);
}

void *forge_header(dns *ans, header *h) {
    *h = ans.head;
    // little endian to big endian : htons
    
}


