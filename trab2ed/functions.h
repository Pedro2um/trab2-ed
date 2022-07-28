#ifndef FUNCTIONS_H
#define FUNCTIONS_H

#include <stdio.h>
#include <stdlib.h>
#include "heap.h"
#include "freq_table.h"
#include "code_table.h"
#include "bitmap.h"

void fill_heap_with_freq_table(binary_heap* b, Freq_Table* f_tbl);

tree* ruffman_tree_constructor(binary_heap* b);

void fill_code_table(Code_Table* c_tbl, tree* a );

void unzip(char ** argv );

void zip(char ** argv);

int execute(int argc, char* argv[]);

#endif 