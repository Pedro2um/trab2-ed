//
// Created by user on 7/24/2022.
//

#ifndef TRAB2ED_TREE_H
#define TRAB2ED_TREE_H
#include "headers.h"




typedef struct tree tree;

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



#endif //TRAB2ED_TREE_H
