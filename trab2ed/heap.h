//
// Created by user on 7/24/2022.
//




#ifndef TRAB2ED_HEAP_H
#define TRAB2ED_HEAP_H
#include "headers.h"
#include "tree.h"

typedef struct binary_heap binary_heap;

binary_heap* new_binary_heap();

void insert(binary_heap* a, tree* x);
tree* remove_min(binary_heap* a);
void delete_binary_heap(binary_heap* a);


#endif //TRAB2ED_HEAP_H
