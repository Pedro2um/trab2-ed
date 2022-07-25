#include <stdio.h>
#include "main.h"

/* 0 - vetor de frequencia e criar nos folhas
 *      llu freq_arr[ASCII]={0};
 *      le char
 *      freq_arr[char]++;
 * 1 - encher a heap de nós folhas{
 *      for(int i = 0; i < ASCII; i++)
 *              if(freq_arr[i]) insert(heap, new_tree(freq_arr[i], i);
 * }
 * 2 - while(heap->n  >= 2){
 *      insert(heap, merge(remove_min(heap), remove_min(heap)));
 *      }
 * 3 - return remove_min(heap);
 * 4 - fazer na main: montar a tabela a partir da arvore
 *
 */

void solve(){
    puts("Hello");
}

#include "headers.h"
int main(int argc, char* argv[]) {
    /*int x = atoi(argv[1]);
    if(x == 1) zip(argv[2]);
    else if( x == 2) unzip(argv[2]);
    else assert(0);
     */
    FILE* f = fopen("filme.mkv", "rb");
    if(f == NULL ) exit (1);
    Freq_Table * f_tbl = init_freq_table();


    fread_freq_table(f_tbl, f);

   binary_heap * b = new_binary_heap();
   fill_heap_with_freq_table(b, f_tbl);
   tree* ruffman = ruffman_tree_constructor(b);
   show_tree(ruffman);



    free_freq_table(f_tbl);
    fclose(f);
    delete_binary_heap(b);
    erase(ruffman);

    return 0;
}
