
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
#define BITS_READ (unsigned int)8*1024*1024*64


#define forn(i, n) for(int i =0; i < n ; i ++)

typedef struct stream Stream;

struct stream{
    unsigned char * arr;
    unsigned int index;
    unsigned int leght;
    unsigned int MAX_SIZE;
    int flag;
};

static Stream* init_stream(unsigned int MAX_SIZE_bytes){
    Stream* s = (Stream*)calloc(1 , sizeof(Stream));
    s->arr = (char*)calloc(MAX_SIZE_bytes, sizeof(char));
    s->index =0 ;
    s->leght =0 ;
    s->MAX_SIZE = MAX_SIZE_bytes;
    s->flag =0 ;
    return s ;
}

static void fill_stream(FILE* f_in, Stream* s ){
    s->leght =0 ;
    s->index =0 ;

    unsigned long long int pos = ftell(f_in);
    fseek(f_in, 0 , SEEK_END);
    unsigned long long int  TAM = ftell(f_in);

    fseek(f_in, pos, SEEK_SET);

 
    char * arr = s->arr;



    if((TAM - pos) > s->MAX_SIZE){
        //fread((void*)arr, sizeof(char), s->MAX_SIZE, f_in);
        for(int i= 0; i < s->MAX_SIZE; i ++ ){
            unsigned char c = fgetc(f_in);
            arr[i] = c;
        }
        s->leght = s->MAX_SIZE;
    }else{
        s->flag = 1;
        for( int i =0; i < (TAM - pos); i ++){
           unsigned char c = fgetc(f_in);
           arr[i] = c;
        }
        //fread((void*)arr, sizeof(char), (TAM - pos), f_in);
        s->leght = TAM - pos;
    }
}

static int stream_get_flag(Stream* s){
    return s->flag;
}

static void read_by_stream(Stream* s , unsigned char * c ){
    *c = s->arr[s->index];
    s->index ++ ; 
    return;
}

static int empty_stream(Stream* s){
    if(s->index == s->leght) return 1 ;
    else return 0;
}

static void free_stream(Stream* s ){
    free(s->arr);
    free(s);
    return;
}


/*headers *****************************************************************************************************/

static void code_and_write_bitmap(FILE* f_in, FILE* f_out,Code_Table* c_table, int * rem, unsigned int MAX_SIZE);

static void read_bitmap_for_unzip(FILE* f_in , bitmap * map, int * flag);

void private_zip(FILE* f, Code_Table* c_tbl, tree* ruffman, char ** _argv );

void private_unzip(char * dir );

void unzip(char ** argv );

void zip(char ** argv);

static void write_uncoded_for_unzip(FILE* f_out,FILE * f_in , tree* ruffman_coded);

static void search_for_char( FILE* f_in, bitmap* map, tree* ruffman,unsigned char* ret, int * index, long long  int * i);


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

void private_zip(FILE* f, Code_Table* c_tbl, tree* ruffman, char ** _argv ){

    char * dir = _argv[1]; 
    char separator[2] = ".";
    char * aux  = strtok(dir , separator);
    char * f_zip_dir = (char*)calloc(1 , sizeof(char)*(strlen(aux) + 17));
    sprintf(f_zip_dir,"./compfile/%s.comp", aux);
    //strcpy(f_zip_dir, aux);
    //strcat(f_zip_dir, ".comp");



    FILE* f_zip = fopen(f_zip_dir,"wb");
    if (f_zip == NULL ) exit(1);
    
    aux = strtok(NULL, separator);

    unsigned char c_fwrite = strlen(aux);
  


    /*escrevendo bits de controle para o tipo de extensão anterior a compressao*/
    /*na ordem, é impresso quantos bytes tem a terminação e a terminação em si*/
    fwrite((void*)&c_fwrite, sizeof(char), 1, f_zip);
    fwrite((void*)aux , sizeof(char)*((unsigned int )c_fwrite) , 1, f_zip);
       
    unsigned int coded_tree_size = get_size_coded_tree(ruffman); 
    unsigned int coded_tree_size_bytes = (coded_tree_size + 7)/8;
    //printf("\n%d\n", coded_tree_size);
    /*escrevendo em dois bytes o tamanho da arvore codificada
    **é um numero binario impresso ao contrario (AB)- > (BA)*/
   


    bitmap * map_coded_tree = bitmapInit(coded_tree_size);
    write_coded_tree(ruffman, f_zip, map_coded_tree );


    unsigned char * contents = bitmapGetContents(map_coded_tree);

    /*
    for(int i =0; i < bitmapGetLength(map_coded_tree); i ++){
       int n = bitmapGetBit(map_coded_tree, i );
       printf("%d", n );
    }
    */

    /*escrevendo arvore no arquivo*/
    fwrite((void*)contents, sizeof(char), coded_tree_size_bytes, f_zip);
    /*for(int i =0; i < coded_tree_size_bytes; i ++){
        char c = contents[i];
        fwrite((void*)&c, sizeof(char), 1, f_zip);
    }
    */

    bitmapLibera(map_coded_tree);

    code_and_write_bitmap(f, f_zip, c_tbl, 0 , BITS_READ);


    fclose(f_zip);

    free(f_zip_dir);


    
    return ;
}


static void code_and_write_bitmap(FILE* f_in, FILE* f_out,Code_Table* c_table, int * rem, unsigned int MAX_SIZE){

    fseek(f_in, 0, SEEK_SET);
    bitmap* b_writer = bitmapInit(MAX_SIZE);
    Stream* s = init_stream(MAX_SIZE/8);
    


    unsigned char c =0;
    
    while(!stream_get_flag(s)){

        fill_stream(f_in, s );
        
        while(!empty_stream(s)){

            read_by_stream(s, &c);
            char * string = get_code_table(c_table, (unsigned int )c);
            int index =0;
            int tam_string = strlen(string);

            while(index < tam_string){

                bitmapAppendLeastSignificantBit(b_writer, string[index]);

                if(bitmapGetLength(b_writer) == bitmapGetMaxSize(b_writer)){
                    char * contents = bitmapGetContents(b_writer);
                    fwrite((void*)contents, sizeof(char)*(MAX_SIZE/8) , 1, f_out);
                    bitmapLibera(b_writer);
                    b_writer = bitmapInit(MAX_SIZE);
                }
                index ++;
            }

        }

    }
    unsigned int bit_map_leght = bitmapGetLength(b_writer);
    unsigned char  n_aprox = bit_map_leght%8;
    /*se for multiplo de 8, entao inseriu tudo */
    if(bit_map_leght != 0 && n_aprox == 0){
        n_aprox = 8;
    }

    //tirei o if
    char * contents = bitmapGetContents(b_writer);
    unsigned int lenght_byte= (bit_map_leght + 7)/8;
    fwrite((void*)contents, sizeof(char)*lenght_byte , 1, f_out);    


    fwrite((void*)&n_aprox, sizeof(char), 1, f_out);



    free_stream(s);
    bitmapLibera(b_writer);
}



void private_unzip(char * dir ){

    int tam_dir = strlen(dir);
    char *modify_dir = (char*)calloc(1,sizeof(char)*(tam_dir + 12));
    sprintf(modify_dir, "./compfile/%s", dir);

    FILE* f_in = fopen(modify_dir, "rd");
    if(f_in == NULL) exit(2);

    fseek(f_in, 0 , SEEK_SET);
    
    char * aux ;
    sscanf(dir, "%m[^.]", &aux);
    unsigned char c_read = fgetc(f_in);

    int tam = c_read;
    char terminator[tam + 1];

    for(int i =0; i < tam; i ++){
        c_read = fgetc(f_in);
        terminator[i] = c_read;
    }
    terminator[tam] = '\0';

    char * new_dir = (char*)calloc(1, sizeof(char)*(strlen(aux) + c_read + 2 + 10));
    sprintf(new_dir, "./newfile/%s.%s", aux, terminator );

    //printf("\n%s", new_dir);
    free(aux);

    FILE* f_out  = fopen(new_dir, "wb");
    if(f_out == NULL) exit(3);

    
    /*
    bitmap* recovered_tree = bitmapInit(size_recovered_tree*BYTE_SIZE);
    bitMapSetLenght(recovered_tree,size_recovered_tree*BYTE_SIZE);

    char * contents = bitmapGetContents(recovered_tree);
    
    for(int i =0; i < size_recovered_tree; i ++){
        c_read = fgetc(f_in);
        contents[i] = c_read;
    }

    */
    tree* ruffman_decoded = recover_tree_2(f_in);
    //show_tree(ruffman_decoded);
    //bitmapLibera(recovered_tree);
   
    
    
    
    write_uncoded_for_unzip(f_out,f_in, ruffman_decoded);
        
    
    


    fclose(f_in);
    fclose(f_out);
    free(new_dir);
    free(modify_dir);
    erase(ruffman_decoded);
    return ;


}


static void read_bitmap(FILE* f_in , bitmap * map, int * flag){

    unsigned long long int pos = ftell(f_in);
    fseek(f_in, 0 , SEEK_END);
    unsigned long long int  TAM = ftell(f_in);

    fseek(f_in, pos, SEEK_SET);

 
    char * contents = bitmapGetContents(map);



    if((TAM - pos) > BITS_READ/BYTE_SIZE){

        for(int i= 0; i < BITS_READ/BYTE_SIZE; i ++ ){
            unsigned char c = fgetc(f_in);
            contents[i] = c;
        }
        bitMapSetLenght(map,BITS_READ);
    }else{
        *flag = 1;
        for( int i =0; i < (TAM - pos); i ++){
            unsigned char c = fgetc(f_in);
            contents[i] = c;
        }
        bitMapSetLenght(map ,(TAM - pos )*BYTE_SIZE);
    }

    return ;
}


static void search_for_char( FILE* f_in, bitmap* map, tree* ruffman,unsigned char* ret, int * index, long long  int * i){
    if(right_child(ruffman) == NULL){

        *ret = get_char(ruffman);
        return ;
    } 
    if(*index >= BYTE_SIZE){
        char * contents = bitmapGetContents(map);
        *contents= fgetc(f_in);
        *i = *i  + 1  ;
        *index = 0;
    }
    int n = bitmapGetBit(map, *index);
    if(n){
        *index = *index +  1;
        search_for_char(f_in, map, right_child(ruffman), ret, index , i );
    }else{
        *index = *index + 1;
        search_for_char(f_in, map, left_child(ruffman), ret, index , i );
    }
    return;
}



static void write_uncoded_for_unzip(FILE* f_out,FILE * f_in , tree* ruffman_coded){

    long long unsigned int pos = ftell(f_in);
    fseek(f_in, 0, SEEK_END);
    long long unsigned int TAM = ftell(f_in);
    fseek(f_in, pos, SEEK_SET);
    
    bitmap* map = bitmapInit(BYTE_SIZE);
    bitMapSetLenght(map, BYTE_SIZE);
    int index =8 ;
    long long  int i = 0;

    bitmap* map_write = bitmapInit(BITS_READ);
    unsigned char * char_arr = bitmapGetContents(map_write);
    unsigned int index_arr = -1 ;
    unsigned int tam_arr = (BITS_READ + 7)/8;
    unsigned char ret =0;
    

   while( i < (TAM - pos - 1)){
        index_arr ++;
        bitMapSetLenght(map_write , 8*(index_arr + 1) );
        search_for_char(f_in, map, ruffman_coded,&ret, &index , &i);
        
       //printf("\n:%c:%lld:%d", ret, i, index);
        char_arr[index_arr] = ret;


        if(index_arr == tam_arr - 1 ){
            index_arr = -1;
            fwrite((void*)char_arr, sizeof(char)*tam_arr, 1, f_out);
            bitMapSetLenght(map_write, 0 );
        }
    }
    


    long unsigned int tam_map = bitmapGetLength(map_write);
    if(tam_map){
        fwrite((void*)char_arr, sizeof(char)*(tam_map/8), 1, f_out);
    }

    if(index >= 8){
        index =0;
    }

   unsigned char aux = fgetc(f_in);
    printf("%d", aux);
    
    
    printf("\n%d %d\n", aux, index);
    if(aux != 8){
         while(aux >= index){
         search_for_char(f_in, map, ruffman_coded, &ret, &index, &i);

         printf("\n:%d:%d:", ret ,index);
         break;
         fwrite((void*)&ret, 1, sizeof(char), f_out);
    }


    }
    
    
    bitmapLibera(map);
    bitmapLibera(map_write);

    return;

}

void zip(char ** argv){

    int tam  =strlen(argv[1]);
    
    char * dir = (char*)calloc(1, sizeof(char)*(tam + 10 ));
    sprintf(dir, "./target/%s", argv[1]);
    FILE* f = fopen(dir, "rb");
    if(f == NULL ) exit (1);
    Freq_Table * f_tbl = init_freq_table();


    fread_freq_table(f_tbl, f);

    binary_heap * b = new_binary_heap();
    fill_heap_with_freq_table(b, f_tbl);
    tree* ruffman = ruffman_tree_constructor(b);
    //show_tree(ruffman);

    //printf("\n%d\n", height_tree(ruffman));

    Code_Table* c_tbl = init_code_table();
    
    fill_code_table(c_tbl, ruffman);
    //show_code_table(c_tbl);


    private_zip(f, c_tbl, ruffman, argv);

   

    free_code_table(c_tbl);
    free_freq_table(f_tbl);
    free(dir);
    fclose(f);
    delete_binary_heap(b);
    erase(ruffman);
}

void unzip(char ** argv){
    private_unzip(argv[1]);
}

int execute(int argc, char* argv[]){

    if(argc < 2){
        printf("\nFormato de execução digitado incorretamente!\n");
        return -1;
    }
    
    if(strcmp("zip", argv[2]) == 0 ){
        zip(argv);
    }else if(strcmp("unzip", argv[2]) == 0){
        unzip(argv);
    }else{
        printf("\nOperação inválida digitada!\n");
        return -1;
    }
    printf("\nOpeação realizada com sucesso\n");
    
    return 1;
}