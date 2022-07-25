//
// Created by user on 7/24/2022.
//

#ifndef TRAB2ED_TREE_H
#define TRAB2ED_TREE_H





typedef struct tree tree;

#include <stdio.h>
#include <stdlib.h>
typedef long long unsigned llu;
typedef long long ll;
typedef long double ld;

/*
 * Alocação de memória
 */
tree* new(llu freq, char c);

/*
 * Une duas arvores e cria um nó pai
 */
tree* merge(tree* a, tree* b);

/*
 * Retorna o peso do nó
 */
llu get_weight(tree* a);

/*
 *  Desalocação de memória
 */
void erase(tree* a);

void show_tree(tree* a);



#endif //TRAB2ED_TREE_H
