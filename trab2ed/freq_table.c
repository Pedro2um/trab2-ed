#include "freq_table.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#define ASCII 256

#define forn(i , n) for( int i =0; i < n; i++)

struct freq_table{
    unsigned long long int freq_vector[ASCII];
};

Freq_Table* init_freq_table(void){
    Freq_Table* f_tbl = (Freq_Table*)calloc(1,sizeof(Freq_Table));

    return f_tbl;
}

void insert_freq_table(Freq_Table* f_tbl, int i){
    f_tbl->freq_vector[i]++;
    return ;
}

long long unsigned int get_freq_table(Freq_Table* f_tbl, int i){
    return f_tbl->freq_vector[i];
}

void free_freq_table(Freq_Table* f_tble){
    free(f_tble);
    return;
}

void show_freq_table(Freq_Table* f_tbl){
    forn(i , ASCII){
        if(f_tbl->freq_vector[i]) printf("%c: %lld\n", (char)i, f_tbl->freq_vector[i]);
    }
    return ;
}


void fread_freq_table(Freq_Table* f_table, FILE * file){
    int c = 0;
    while(1){
        c = fgetc(file);
        if( feof(file)){
            break;
        }
        f_table->freq_vector[c] ++;
    }
}