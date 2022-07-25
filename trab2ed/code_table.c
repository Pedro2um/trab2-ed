#include "code_table.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>



#define ASCII 256

struct code_table{
    char* string_arr[256];
};

Code_Table* init_code_table(void){
    Code_Table* c_tbl = (Code_Table*)calloc(1,sizeof(Code_Table));
    for(int i =0; i < ASCII; i ++){
        c_tbl->string_arr[i] = NULL;
    }
    return c_tbl;
}

void insert_code_table(Code_Table* c_table, char* string , int index){
    if(c_table == NULL) return ;
    if(c_table->string_arr[index]) free(c_table->string_arr[index]);
    c_table->string_arr[index] = strdup(string);
    return ;
}


void free_code_table(Code_Table* c_table){
    if(c_table == NULL) return;
    for(int i =0; i < ASCII; i ++){
        if(c_table->string_arr[i]){
            free(c_table->string_arr[i]);
        }
    }
    free(c_table);
    return ;
}


char * get_code_table(Code_Table* c_table, int index){
    if(c_table == NULL) return NULL;
    return c_table->string_arr[index];
}

void show_code_table(Code_Table* c_table){
    if(c_table == NULL) return;
    printf("\n");
    for(int i =0; i < ASCII; i ++){
        if(c_table->string_arr[i]) printf("%s\n", c_table->string_arr[i]);
    }
    return ;
}