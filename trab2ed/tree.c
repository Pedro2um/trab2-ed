#ifndef TREE_C
#define TREE_C

#include "tree.h"
#include "headers.h"
//
// Created by user on 7/24/2022.
//





typedef struct tree tree;
/*
 * Convenção adotada: caminhar para a esquerda de um nó, equivale a o bit 0, para a direita o bit 1
 */
struct tree{
    char c;
    llu freq;
    tree* right;
    tree* left;
};

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

#endif
