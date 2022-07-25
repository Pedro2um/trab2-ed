#ifndef FUNCTIONS_H
#define FUNCTIONS_H

#include <stdio.h>
#include <stdlib.h>
#include "heap.h"
#include "freq_table.h"

void fill_heap_with_freq_table(binary_heap* b, Freq_Table* f_tbl);

tree* ruffman_tree_constructor(binary_heap* b);


#endif 