#include <stdio.h>
#include "main.h"

/********************************************************************************************************************************/

    /*                                                                                                                      
     *
     *
     *  DESCRIÇÃO PARA A EXECUÇÃO DO PROGRAMA: 
     *  
     *  FORMATO :  digite no terminal --> "./(nome do executável) (alvo) (operação)"
     * 
     *   
     *  
     *  1 - Para zipar um arquivo:
     *      
     *      (*)   Crie uma pasta chamada "compfile" no mesmo diretório do executável, nessa será colocado o arquivo zipado
     * 
     *      (**) O Alvo deve estar no mesmo diretório do executável
     * 
     *      Ex.: ./ruffman texto.txt zip
     * 
     *      
     *      
     *  2 - Para unzipar um arquivo:
     *      
     *      (*)  Crie uma pasta chamada "newfile" no mesmo diretório do executável, nessa será colocado o arquivo unzipado
     * 
     *      (**) O Alvo deve estar em uma pasta chamada "compfile", está pasta deve estar no mesmo diretório do executável
     * 
     *      Ex.: ./ruffman texto.comp unzip
     * 
     * 
     *  
    */

/********************************************************************************************************************************/



#include "headers.h"
int main(int argc, char* argv[]) {

    execute(argc,  argv);
    
    return 0;
}



// ANOTAÇÕES PASSADAS


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

/*int x = atoi(argv[1]);
    if(x == 1) zip(argv[2]);
    else if( x == 2) unzip(argv[2]);
    else assert(0);
     */