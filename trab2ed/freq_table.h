#ifndef FREQ_TABLE
#define FRAQ_TABLE

#include <stdio.h>
#include <stdlib.h>

typedef struct freq_table Freq_Table;

Freq_Table* init_freq_table(void);

void insert_freq_table(Freq_Table* f_tbl, int i );

long long unsigned int get_freq_table(Freq_Table* f_tbl, int i);

void free_freq_table(Freq_Table* f_tble);

void show_freq_table(Freq_Table* f_tbl);

void fread_freq_table(Freq_Table* f_table, FILE * file);

#endif