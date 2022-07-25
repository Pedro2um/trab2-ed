#include "freq_table.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>


#define forn(i , n) for( int i =0; i < n; i++)

struct freq_table{
    unsigned long long int freq_vector[256];
};

Freq_Table* init_freq_table(void){
    Freq_Table* f_tbl = (Freq_Table*)malloc(sizeof(Freq_Table));
    forn(i , 256){
        f_tbl->freq_vector[i] =0;
    }
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
    forn(i , 256){
        if(f_tbl->freq_vector[i]) printf("%c: %lld\n", (char)i, f_tbl->freq_vector[i]);
    }
    return ;
}


void fread_freq_table(Freq_Table* f_table, FILE * file){
    fseek(file, 0, SEEK_SET);
    char c = 0;
    while(fread(&c, 1, 1, file)){
        int index = (int)c;
        insert_freq_table(f_table, index);
    }
}