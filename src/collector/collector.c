#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>

#include "collector.h"
#include "../my_libc.h"

// Global linked list keeping track of all allocated resources
struct collector *collector;


// Returns true if collector is empty
static bool collector_empty(){
    return !(collector->first);
}

// Prints all pointers contained in collector
// Used for debug purposes
static void collector_print(){
    if (collector_empty())
        return;
    for (struct collector_node *ride = collector->first; ride; ride = ride->next)
        printf("ptr:\t%p\n", ride->ptr);
}

// Init global collector
void collector_init(){
    collector = my_malloc(sizeof(struct collector));
    collector->first = NULL;
}

// my_malloc() wrapper to keep track of resources in collector
// Reference to resource is stored in a node that us inserted at the start of
// the collector list
void *collector_malloc(size_t size){
    void *ptr = my_malloc(size);

    struct collector_node *node = my_malloc(sizeof(struct collector_node));
    node->ptr = ptr;
    node->prev = NULL;
    if (collector_empty()){
	 node->next = NULL;
    }
    else{
	node->next = collector->first;
        collector->first->prev = node;
    }
    collector->first = node;

    return ptr;
}


// free() wrapper to free resource and remove its reference from the collector
void collector_free(void *ptr){
    for (struct collector_node *ride = collector->first; ride; ride = ride->next)
        if (ride->ptr == ptr){
	    if (ride->prev)  // Not first node of the collector
	        ride->prev->next = ride->next;
	    else  // First node of the collector, update new first
                collector->first = ride->next;
            if (ride->next)  // Not last node of the collector
	        ride->next->prev = ride->prev;
            free(ptr);
	    free(ride);
	    return;
	}
}


// Free every resource referenced in the collector, and the collector itself
void collector_exit(){
    for (struct collector_node *ride = collector->first; ride;){
        if (ride->next){  // More nodes
            ride = ride->next;
	    free(ride->prev->ptr);
	    free(ride->prev);
	} else{  // Last node
            free(ride->ptr);
	    free(ride);
            break;
	}
    }
    free(collector);
}
