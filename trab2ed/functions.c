
#include <stdio.h>
#include <stdlib.h>
#include "heap.h"
#include "freq_table.h"
#include "code_table.h"
#include "tree.h"
#include <string.h>
#include "bitmap.h"
#define ASCII 256

#define BYTE_SIZE 8

#define forn(i, n) for(int i =0; i < n ; i ++)

/*headers *****************************************************************************************************/

static void code_and_write_bitmap(FILE* f_in, FILE* f_out,Code_Table* c_table, int * rem, unsigned int MAX_SIZE);
void zip(FILE* f, Code_Table* c_tbl, tree* ruffman, char ** _argv );

/***************************************************************************************************************/
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
        string[index] = '\0';
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

static int get_size_coded_tree(tree* a ){
    if(!a) return 0;
    if(its_leaf(a)){
        return BYTE_SIZE + 1 ;
    }else{
        return 1 + get_size_coded_tree(left_child(a)) + get_size_coded_tree(right_child(a));
    }
}

static void imprime_arvore_cd(tree* a){
    if(!a) return ;
    if(its_leaf(a)){
        printf("1%c", get_char(a));
    }else{
        printf("0");
        imprime_arvore_cd(left_child(a));
        imprime_arvore_cd(right_child(a));
    }
    return ;
}

static void write_coded_tree(tree* a, FILE* f, bitmap* map){
    if(!a) return ;

    if(its_leaf(a)){
        bitmapAppendLeastSignificantBit(map, 0x01);
        unsigned int aux = 0x80;
        unsigned char c = get_char(a);
        for(; aux !=0; aux >>= 1){
            unsigned int and = aux & c;
            if(and) bitmapAppendLeastSignificantBit(map, 0x01);
            else bitmapAppendLeastSignificantBit(map, 0);
        }
    }else{
        bitmapAppendLeastSignificantBit(map, 0);
        write_coded_tree(left_child(a),f , map);
        write_coded_tree(right_child(a), f ,map);
    }
}

void zip(FILE* f, Code_Table* c_tbl, tree* ruffman, char ** _argv ){

    char * dir = _argv[1]; 
    char separator[2] = ".";
    char * aux  = strtok(dir , separator);
    char * f_zip_dir = (char*)calloc(1 , sizeof(char)*(strlen(aux) + 6 ));
    
    strcpy(f_zip_dir, aux);
    strcat(f_zip_dir, ".comp\0");



    FILE* f_zip = fopen(f_zip_dir,"wb");
    
    aux = strtok(NULL, separator);

    unsigned char c_fwrite = strlen(aux);
  


    /*escrevendo bits de controle para o tipo de extensão anterior a compressao*/
    /*na ordem, é impresso quantos bytes tem a terminação e a terminação em si*/
    fwrite((void*)&c_fwrite, sizeof(char), 1, f_zip);
    fwrite((void*)aux , sizeof(char)*((unsigned int )c_fwrite) , 1, f_zip);
       
    unsigned int coded_tree_size = get_size_coded_tree(ruffman); 
    printf("\n%d\n", coded_tree_size);
    /*escrevendo em dois bytes o tamanho da arvore codificada
    **é um numero binario impresso ao contrario (AB)- > (BA)*/
    c_fwrite = coded_tree_size%256;
    fwrite((void*)&c_fwrite, sizeof(char) , 1, f_zip);
    c_fwrite = coded_tree_size/256;
    fwrite((void*)&c_fwrite, sizeof(char) , 1, f_zip);


    bitmap * map_coded_tree = bitmapInit(coded_tree_size);
    write_coded_tree(ruffman, f_zip, map_coded_tree );

    unsigned int qtd_bytes =0;
    unsigned int l = bitmapGetLength(map_coded_tree)/8;

    if(bitmapGetLength(map_coded_tree)%8 == 0) qtd_bytes = l;
    else qtd_bytes = l + 1;

    unsigned char * contents = bitmapGetContents(map_coded_tree);

    for(int i =0; i < bitmapGetLength(map_coded_tree); i ++){
       int n = bitmapGetBit(map_coded_tree, i );
       printf("%d", n );
    }

    /*escrevendo arvore no arquivo*/
    for(int i =0; i < qtd_bytes; i ++){
        char c = contents[i];
        fwrite((void*)&c, sizeof(char), 1, f_zip);
    }
    

    bitmapLibera(map_coded_tree);

    code_and_write_bitmap(f, f_zip, c_tbl, 0 , (unsigned int)1024*1024*8*256);


    fclose(f_zip);
    free(f_zip_dir);
    
    return ;
}


static void code_and_write_bitmap(FILE* f_in, FILE* f_out,Code_Table* c_table, int * rem, unsigned int MAX_SIZE){
    fseek(f_in, 0, SEEK_SET);
    bitmap* b = bitmapInit(MAX_SIZE);

    unsigned char c =0;
    while(1){
        c = fgetc(f_in);
        if(feof(f_in)) break;
        char * string = get_code_table(c_table, (unsigned int )c);
        int index =0;

        while(1){
            if(string[index] != '\0'){

                if(string[index] != '0') bitmapAppendLeastSignificantBit(b , 0x01);
                else bitmapAppendLeastSignificantBit(b, 0);

                if(bitmapGetLength(b) == bitmapGetMaxSize(b)){
                    char * contents = bitmapGetContents(b);

                    fwrite((void*)contents, sizeof(char)*(MAX_SIZE/8) , 1, f_out);
                    memset((void*)contents, 0 , sizeof(char)*(MAX_SIZE/8));
                    bitMapSetLenght(b, 0);

                }
            }else {
                break;
            }
            index ++;
        }

    }

    unsigned char  n_aprox = 8 - (bitmapGetLength(b)%8);

    if(bitmapGetLength != 0 ){
        char * contents = bitmapGetContents(b);

        unsigned int lenght_byte=0;
        unsigned int b_map_lenght = bitmapGetLength(b);
        if(b_map_lenght%8 ==0 ) lenght_byte = b_map_lenght/8;
        else lenght_byte = b_map_lenght/8 + 1;

        fwrite((void*)contents, sizeof(char)*lenght_byte , 1, f_out);    
    }

   fwrite((void*)&n_aprox, sizeof(char), 1, f_out);




    bitmapLibera(b);

}



void unzip(FILE * f){
    
}