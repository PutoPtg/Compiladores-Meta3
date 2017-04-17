/***********************************************************************************
*Ficheiro "tree.h"
*Anexo ao Analisador Sintático da linguagem Ja - Sub-conjunto de Java
*Cadeira de Compiladores - 2017 - Licenciatura em Engenharia Informática
*Manuel Madeira Amado - 2006131282
*Xavier Silva - 2013153577
*Versão 0.03
************************************************************************************/

#ifndef TREE_H_INCLUDED
#define TREE_H_INCLUDED

typedef enum {ID_node, STRLIT_node, BOOLLIT_node, DECLIT_node,OTHER_node, REALLIT_node, EXP_node} node_type;


typedef struct node
{
    struct node* parent;
    struct node* left;
    struct node* right;
    struct node** children;
    node_type nodeType;
    char* nodeTypeName;
    char* var;
    char* anot;   // para guardar as anotações
    int numChildren;
}node;


node* createNode(node_type nodeType, char* nodeTypeName, char* var);
void addChild(node* parent, node* child);
void addBrother(node* left, node* right);
void printTree(node* current, int level, int valorS);
void clearTree(node* current);

#endif // TREE_H_INCLUDED
