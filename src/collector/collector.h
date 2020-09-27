#pragma once

#include <stdlib.h> 

// Linked list node containing reference to allocated resource
struct collector_node{
    void *ptr;
    struct collector_node *next;
    struct collector_node *prev;
};

// Head of linked list
struct collector{
    struct collector_node *first;
};

// Global linked list keeping track of all allocated resources
extern struct collector *collector;


// Init global collector
void collector_init();

// my_malloc() wrapper to keep track of resources in collector
// Reference to resource is stored in a node that is inserted at the start of
// the collector list
void *collector_malloc(size_t size);

// free() wrapper to free resource and remove its reference from the collector
void collector_free(void *ptr);

// Allocate a new size for an existing resource, and free its previous reference
void *collector_realloc(void *ptr, size_t size);

// Free every resource referenced in the collector, and the collector itself
void collector_exit();
