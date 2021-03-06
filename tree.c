/***********************************************************************************
*Ficheiro "tree.c"
*Anexo ao Analisador Sintático da linguagem Ja - Sub-conjunto de Java
*Cadeira de Compiladores - 2017 - Licenciatura em Engenharia Informática
*Manuel Madeira Amado - 2006131282
*Xavier Silva - 2013153577
*Versão 0.11
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

/********************************************************************************
* DESCRIÇÃO: Imprime a árvore           										*
* RECEBE: Nó raíz, nível do nó raiz, árvore simpes ou comentada					*
* DEVOLVE: nada																	*
*********************************************************************************/
void printTree(node* current, int level, int valorS){
    int i;

/*------------------------------------------------------------------------------
-  Condição de paragem da recursividade, folha                                 -
------------------------------------------------------------------------------*/
    if(current==NULL){
        return;
    }
    else{
/*------------------------------------------------------------------------------
-  Início da impressão da árvore                                               -
------------------------------------------------------------------------------*/
        for(i=0; i<level; i++){
            printf(".."); //incremento do nível
        }
/*------------------------------------------------------------------------------
- Impressão da árvore anotada                                                  -
------------------------------------------------------------------------------*/
        if(valorS == 1){
/*--EXP_node------------------------------------------------------------------*/
            if(current->nodeType == EXP_node){
                if( strcmp(current->nodeTypeName, "Eq") == 0 ||
                    strcmp(current->nodeTypeName, "Geq") == 0 ||
                    strcmp(current->nodeTypeName, "Gt") == 0 ||
                    strcmp(current->nodeTypeName, "Leq") == 0 ||
                    strcmp(current->nodeTypeName, "Lt") == 0 ||
                    strcmp(current->nodeTypeName, "Neq") == 0 ){
                        printf("%s - %s\n",current->nodeTypeName, current->anot);
                }else{
                    if(strcmp(current->nodeTypeName,"Id")==0){
                        if(strcmp(current->anot,"")==0){
                            strcpy(current->anot,"undef");
                        }
                        printf("Id(%s) - %s\n",current->var,current->anot);
                    }else{
                        printf("%s - %s\n", current->nodeTypeName,current->anot);
                    }
                }
            }
/*--STRLIT_node---------------------------------------------------------------*/
            else{
                if(current->nodeType == STRLIT_node){
                    if(strcmp(current->nodeTypeName,"Strlit")==0){
                        printf("StrLit(%s) - %s\n", current->var,current->anot);
                    }
                }
/*--DECLIT_node---------------------------------------------------------------*/
                else{
                    if(current->nodeType == DECLIT_node){
                        if(strcmp(current->nodeTypeName,"DecLit")==0){
                            printf("DecLit(%s) - %s\n", current->var,current->anot);
                        }
                    }
/*--BOOLLIT_node---------------------------------------------------------------*/
                    else{
                        if(current->nodeType == BOOLLIT_node){
            	            if(strcmp(current->nodeTypeName,"BoolLit")==0){
                                printf("BoolLit(%s) - %s\n", current->var,current->anot);
                            }
                        }
/*--REALLIT_node--------------------------------------------------------------*/
                        else{
                            if(current->nodeType == REALLIT_node){
            	                if(strcmp(current->nodeTypeName,"RealLit")==0){
                                   printf("RealLit(%s) - %s\n", current->var,current->anot);
                                }
                            }
/*--ID_node-------------------------------------------------------------------*/
                            else{
                                if(current->nodeType == ID_node){
                                	if(strcmp(current->parent->nodeTypeName, "VarDecl") == 0 || strcmp(current->parent->nodeTypeName, "ParamDecl") == 0 ||
        								strcmp(current->parent->nodeTypeName, "FieldDecl") == 0 || strcmp(current->parent->nodeTypeName, "MethodHeader") == 0 ||
        								strcmp(current->parent->nodeTypeName, "Program") == 0){
                                		printf("Id(%s)\n",current->var);
                                	}
                                	else{
                                		 printf("Id(%s) - %s\n", current->var, current->anot);
                                	}

            	                    /*if(strcmp(current->anot, "") == 0){
                                        //printf("VAZIO");
                                        printf("Id(%s)\n",current->var);
                                    }else{
                                        printf("Id(%s) - %s\n", current->var, current->anot);
                                        //printf("ANOTADO");
                                    }*/
                                }
/*--OTHER_node----------------------------------------------------------------*/
                                else{
                                    if(strcmp(current->anot, "") == 0){
                                        printf("%s\n", current->nodeTypeName);
                                    }else{
                                        printf("%s - %s\n", current->nodeTypeName, current->anot);
                                    }
                                }
                            }
                        }
                    }
                }
            }
        }
/*------------------------------------------------------------------------------
-Impressão da árvore simples                                                   -
------------------------------------------------------------------------------*/
        else{
            if(current->nodeType == ID_node){
                printf("Id(%s)\n",current->var);
            }else{
                if(current->nodeType == BOOLLIT_node){
                    printf("BoolLit(%s)\n", current->var);
                }else{
                    if(current->nodeType == DECLIT_node){
                        printf("DecLit(%s)\n", current->var);
                    }else{
                        if(current->nodeType == STRLIT_node){
                            printf("StrLit(%s)\n", current->var);
                        }else{
                            if(current->nodeType == REALLIT_node){
                                printf("RealLit(%s)\n", current->var);
                            }else{
                                printf("%s\n", current->nodeTypeName);
                            }
                        }
                    }
                }
            }
        }
/*------------------------------------------------------------------------------
-Chamada Resursiva da sub árvore                                               -
------------------------------------------------------------------------------*/
        for(i=0; i<current->numChildren; i++){
            printTree(current->children[i], level+1, valorS);
        }
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
