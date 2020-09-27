#include <stdio.h>
#include <signal.h>
#include <stdlib.h>

#include "catch.h"
#include "collector.h"

// All the functions called when ctrl-c is caught go here
static void inthandler(){
    // Functions run on interrupt go between these 2 comments
    printf("Caught SIGINT, exiting ...\n");
    collector_exit();
    // Functions run on interrupt go between these 2 comments

    exit(0);
}

// Catches ctrl-c input and calls functions to exit gracefully
void catch(){
    signal(SIGINT, inthandler);
}
