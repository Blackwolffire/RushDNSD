#include <stdio.h>
#include <stdlib.h>

#include "linked_list.h"
#include "../my_libc.h"


static void list_print(struct linked_list *l){
    if (!(l->first))  //List is empty
        return;
    for (struct linked_list_node *ride = l->first; ride; ride = ride->next){
        printf("ptr: %p\n", ride->ptr);
    }
}


// Create new linked_list
struct linked_list *list_create(){
    struct linked_list *l =  my_malloc(sizeof(struct linked_list));
    l->first = NULL;
    l->last = NULL;
    return l;
}


// Add node to end of linked_list
struct linked_list *list_add(struct linked_list *l, void *p){
    struct linked_list_node *node = my_malloc(sizeof(struct linked_list_node));
    node->ptr = p;
    node->next = NULL;
    node->prev = l->last;
    if (!(l->first)){  // List is empty
        l->first = node;
	l->last = node;
    } else {
        l->last->next = node;
	l->last = node;
    }
    
    return l;
}


// Remove element p from linked_list (if it exists)
struct linked_list *list_remove(struct linked_list *l, void *p){
    if (!(l->first))  // List is empty
        return l;
    for (struct linked_list_node *ride = l->first; ride; ride = ride->next){
        if (ride->ptr == p){
            ride->prev->next = ride->next;
	    ride->next->prev = ride->prev;
	    free(node);
	}
    }
}

int main(int argc, char **argv){
    return 0;
}
