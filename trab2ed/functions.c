
#include <stdio.h>
#include <stdlib.h>
#include "heap.h"
#include "freq_table.h"
#include "code_table.h"
#include "tree.h"
#include <string.h>
#define ASCII 256


#define forn(i, n) for(int i =0; i < n ; i ++)

void fill_heap_with_freq_table(binary_heap* b, Freq_Table* f_tbl){
    forn(i , ASCII){
        if(get_freq_table(f_tbl, i)){
            tree* aux = new(get_freq_table(f_tbl, i), (char)i);
            insert(b, aux);
        }
    }
}

tree* ruffman_tree_constructor(binary_heap* b){
    while(get_tam_binary_heap(b) >=2){
        insert(b, merge(remove_min(b), remove_min(b)));
    }
    return remove_min(b);
}


static void private_fill_code_table(Code_Table* c_tbl , tree* a, char * string, int index){
    if(!a) return;
    if(its_leaf(a)){
        string[index + 1 ] = '\0';
        
        insert_code_table(c_tbl, string , get_char(a));
    }else{
        string[index] = '0';
        private_fill_code_table(c_tbl,left_child(a), string ,index + 1 );
        string[index] = '1';
        private_fill_code_table(c_tbl, right_child(a), string ,index + 1 );
    }
    return ;

}


void fill_code_table(Code_Table* c_tbl, tree* a ){
    int a_height = height_tree(a);
    char string [a_height + 1 ];
    for(int i =0; i < a_height + 1 ; i ++){
        string[i] =0;
    } 
    private_fill_code_table(c_tbl, a, string , 0);
    return ;


}