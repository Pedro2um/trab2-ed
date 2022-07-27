#ifndef TREE_C
#define TREE_C

#include "tree.h"
#include "headers.h"
#include <string.h>
//
// Created by user on 7/24/2022.
//


#define BYTE_SIZE 8


typedef struct tree tree;
/*
 * Convenção adotada: caminhar para a esquerda de um nó, equivale a o bit 0, para a direita o bit 1
 */
struct tree{
    unsigned char c;
    llu freq;
    tree* right;
    tree* left;
};

static tree* private_recover_tree_2(FILE* f_in ,bitmap* map, int * index);

/*
 * Alocação de memória
 */
tree* new(llu freq, char c){
    tree* new_tree = calloc(1, sizeof(tree));
    if(new_tree == NULL){
        fprintf(stderr,"ERROR, NEW TREE IS NOT ALLOCATED\n");
        exit(1);
    }

    new_tree->c =c;
    new_tree->freq = freq;
    new_tree->left = NULL;
    new_tree->right = NULL;

    return new_tree;
}

/*
 * Une duas arvores e cria um nó pai que não possui um caractere associado
 */
tree* merge(tree* a, tree* b){
    tree* parent = new(0,0);
    parent->left = a;
    parent->right = b;
    parent->freq = a->freq + b->freq;
    return parent;
}

llu get_weight(tree* a){
    return a->freq;
}



static tree* e(tree* h){
    if(h == NULL) return NULL;
    h->right = e(h->right);
	h->left = e(h->left);
	if(h->right == NULL && h->left == NULL){
		free(h);
		return NULL;
	}
}
/*
 *  Desalocação de memória
 */
void erase(tree* root){
    if(root == NULL) return;
    e(root);

}

void show_tree(tree* a){
    if(!a)return;
    printf("< %c: %lld", a->c, a->freq);
    show_tree(a->left);
    show_tree(a->right);
    printf(" >");
    return;
}


static int max(int a, int b){
    return ( a > b ) ? a : b;
}

int height_tree(tree* a){
    if(a == NULL) return -1;
    return (1 + max(height_tree(a->right), height_tree(a->left)));
}


char get_char(tree* a){
    return a->c;
}

int its_leaf(tree*a){
    return (a->left == NULL && a->right == NULL) ? 1 : 0;
}

tree* left_child(tree* a ){
    return a->left;
}


tree* right_child(tree* a){
    return a->right;
}

#endif

static tree* private_recover_tree(bitmap* map, unsigned int* index){

    tree* a = new(0,0);
    if(bitmapGetBit(map, *index)){
        char c = 0;
        *index = *index + 1;
        for(int i =0; i < 8; i ++ ){
            c <<= 1;
            int k = bitmapGetBit(map, *index);
            if(k) c = c | 0x01;
            *index = *index  + 1;
        }
        *index = *index  - 1;
        a->c =c;
        a->left = NULL;
        a->right = NULL;
    }else{
        *index = *index + 1;
        a->left = private_recover_tree(map, index );
        *index = *index + 1;
        a->right = private_recover_tree(map, index);
    }
    return a;
}

tree* recover_tree(bitmap* map){
    if(!map) return NULL;
    unsigned int index =0;
    tree* a = private_recover_tree(map, &index);
    return a;
}


static void appendbyte(bitmap* map, unsigned char c){
    bitMapSetLenght(map, 0 );
    unsigned char * contents = bitmapGetContents(map);
    memset(contents,0 , sizeof(char));

    for(int i =0x80; i != 0; i >>=1 ){
        if(i & c) bitmapAppendLeastSignificantBit(map, 0x01);
        else bitmapAppendLeastSignificantBit(map, 0 );
    }
}

static int read_bit(bitmap* map, FILE* f_in,int * index){
    if(*index >=8){
        unsigned char c = fgetc(f_in);
        appendbyte(map, c);
        *index =0 ;
    }
    
    return bitmapGetBit(map, (*index) ++);
}

static char read_byte(bitmap* map, FILE* f_in,int * index){
    unsigned char c = 0;
    for(int i =0; i < BYTE_SIZE; i ++){
        c <<=1;
        int n  = read_bit(map, f_in, index);
        c |= n;
    }
    return c;
}


static tree* private_recover_tree_2(FILE* f_in ,bitmap* map, int * index){
    tree* a = new(0,0);
    a->freq =0;

    if(read_bit(map, f_in, index)){
        unsigned char c = read_byte(map, f_in,index);
        a->c =c;
        a->left = NULL;
        a->right = NULL;
    }else{
        a->left = private_recover_tree_2(f_in , map, index);
        a->right = private_recover_tree_2(f_in, map, index);
        a->c = 0;
    }
    return a;
}


tree* recover_tree_2(FILE* f_in){
    bitmap* map = bitmapInit(BYTE_SIZE);
    int index = 0 ;

   unsigned char * contents = bitmapGetContents(map);
    *contents = fgetc(f_in);
    bitMapSetLenght(map, 8 );

    tree* a = private_recover_tree_2(f_in, map, &index );

    bitmapLibera(map);
    return a;
   
}