#pragma once

// Basic linked list for pointers
struct linked_list_node{
    void *ptr;
    struct linked_list_node *next;
    struct linked_list_node *prev;
};


// Head of linked list
struct linked_list{
    struct linked_list_node *first;
    struct linked_list_node *last;
};

