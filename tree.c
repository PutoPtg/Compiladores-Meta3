/***********************************************************************************
*Ficheiro "tree.c"
*Anexo ao Analisador Sintático da linguagem Ja - Sub-conjunto de Java
*Cadeira de Compiladores - 2017 - Licenciatura em Engenharia Informática
*Manuel Madeira Amado - 2006131282
*Xavier Silva - 2013153577
*Versão 0.06
************************************************************************************/

#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include "tree.h"



/************************************************************************************
* 1 - mostra os valores a serem colocados nos nós na sua criação					*
* 2 - mostra os valores dos nós a serem juntos pai e filho							*
************************************************************************************/
int DEBUG = 0;

/************************************************************************************
* DESCRIÇÃO: Cria um nó para a Árvore												*
* RECEBE: estrutura node_type, Ponteiro para o nome e ponteiro para a 				*
* variável a armazenar																*
* DEVOLVE: ponteiro para o nó criado												*
************************************************************************************/
node* createNode(node_type nodeType, char* nodeTypeName, char* var)
{
    node* result = (node*)malloc(sizeof(node));
    result->nodeType = nodeType;
    result->nodeTypeName = (char*)malloc((strlen(nodeTypeName)+1)*sizeof(char));
    strcpy(result->nodeTypeName, nodeTypeName);
    if(var != NULL){
    	result->var = (char*)malloc((strlen(var)+1)*sizeof(char));
    	strcpy(result->var, var);
    }
    else{
    	result->var = NULL;
    }
    result->numChildren = 0;
    result->children = NULL;
    result->parent = NULL;
    result->left = NULL;
    result->right = NULL;
    result->anot = (char*)malloc(256*sizeof(char));
    if (DEBUG == 1){printf("*********%s : %s\n", result->nodeTypeName, result->var);}
    return result;
}

/************************************************************************************
* DESCRIÇÃO: Adiciona um nó filho a outro nó										*
* RECEBE: ponteiro para o nó pai e ponteiro para o nó filho							*
* DEVOLVE: nada																		*
************************************************************************************/
void addChild(node* parent, node* child)
{
    if(parent==NULL || child==NULL)
    {
        return;
    }
    if(DEBUG == 2){printf("+++++++++Parent %s : %s ++++++Child %s : %s\n ", parent->nodeTypeName, parent->var, child->nodeTypeName, child->var);}
    parent->numChildren++;
    parent->children = (node**)realloc(parent->children, parent->numChildren*sizeof(node*));
    parent->children[parent->numChildren-1] = child;
    child->parent = parent;
    node* aux = child;
    while((aux = aux->right) != NULL)
    {
        parent->numChildren++;
        parent->children = (node**)realloc(parent->children, parent->numChildren*sizeof(node*));
        parent->children[parent->numChildren-1] = aux;
        aux->parent = parent;
    }
}

/************************************************************************************
* DESCRIÇÃO: Adiciona um ponteiro à esquerda ou direita entre dois nós				*
* RECEBE: ponteiro para o nó à esquerda e ponteiro para o nó à direita				*
* DEVOLVE: nada																		*
************************************************************************************/
void addBrother(node* left, node* right)
{
    if(left==NULL || right==NULL)
    {
        return;
    }
    node* aux = left;
    while(aux->right!=NULL)
    {
        aux = aux->right;
    }
    aux->right = right;
    right->left = aux;
    if(left->parent!=NULL)
    {
        addChild(left->parent, right);
    }
}

/************************************************************************************
* DESCRIÇÃO: Imprime a árvore														*
* RECEBE: TODO																		*
* DEVOLVE: nada																		*
************************************************************************************/
void printTree(node* current, int level, int valorS)
{
    if(current==NULL)
    {
        return;
    }
    int i;


    for(i=0; i<level; i++)
        {
            printf("..");
        }


        if(current->nodeType == ID_node)
        {
            printf("Id(%s)\n",current->var);
        }
        else if(current->nodeType == BOOLLIT_node)
        {
            printf("BoolLit(%s)\n", current->var);
        }
        else if(current->nodeType == DECLIT_node)
        {
            printf("DecLit(%s)\n", current->var);
        }
        else if(current->nodeType == STRLIT_node)
        {
            printf("StrLit(%s)\n", current->var);
        }
        else if(current->nodeType == REALLIT_node)
        {
            printf("RealLit(%s)\n", current->var);
        }
        else
        {
            printf("%s\n", current->nodeTypeName);
        }
        for(i=0; i<current->numChildren; i++)
        {
            printTree(current->children[i], level+1, valorS);
        }
    	
}



/************************************************************************************
* DESCRIÇÃO: Liberta a Memória ocupada pela árvore									*
* RECEBE: ponteiro para a raíz														*
* DEVOLVE: nada																		*
************************************************************************************/
void clearTree(node* current)
{
    if(current == NULL)
    {
        return;
    }
    int i;
    for(i=0; i<current->numChildren; i++)
    {
        clearTree(current->children[i]);
    }
    free(current->children);
    free(current->nodeTypeName);
    free(current->var);
    free(current);
}
