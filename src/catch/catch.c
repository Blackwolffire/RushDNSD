#include <stdio.h>
#include <signal.h>
#include <stdlib.h>

#include "catch.h"

// All the functions called when ctrl-c is caught go here
static void inthandler(){
    // Functions run on interrupt go between these 2 comments
    printf("Caught ctrl-c, exiting\n");
    // Functions run on interrupt go between these 2 comments

    exit(0);
}

// Catches ctrl-c input and calls functions to exit gracefully
void catch(){
    signal(SIGINT, inthandler);
}

int main(int argc, char **argv){
    catch();
    char *s = malloc(64);
    while (1);
    return 0;
}
