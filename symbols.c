/************************************************************************************
* Ficheiro "symbols.c"                                                              *
* Analisador Sintático da linguagem Ja - Sub-conjunto de Java                       *
* Cadeira de Compiladores - 2017 - Licenciatura em Engenharia Informática           *
* Manuel Madeira Amado - 2006131282                                                 *
* Xavier Silva - 2013153577                                                         *
* Versão 0.11                                                                     *
************************************************************************************/

#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <ctype.h>
#include <limits.h>
#include "tree.h"
#include "symbols.h"

#define VAR3 (char*)calloc(1,sizeof(char))

int numFunc = 0;
char actualMethod[256];

/*******************************************************************************
* Cria a tabela de símbolos                                                    *
*******************************************************************************/
table* createTable(table_type type, char* name)
{
    table* tab = (table*)malloc(sizeof(table));
    tab->name = (char*)calloc(1+strlen(name),sizeof(char));
    strcpy(tab->name, name);
    tab->type = type;
    tab->next = NULL;
    tab->prev = NULL;
    tab->numSymbols = 0;
    tab->symbols = NULL;
    tab->check = False;
    tab->checkSem = False;
    return tab;
}

/*******************************************************************************
* Cria Símbolos                                                                *
*******************************************************************************/
symbol* createSymbol(char* name, char* paramTypes ,char* type, param_type param, long long int line, long long int column, int isFunction, int numParam)
{
    symbol* sym = (symbol*)malloc(sizeof(symbol));
    sym->name = (char*)calloc(1+strlen(name),sizeof(char));
    strcpy(sym->name, name);
    sym->paramTypes = (char*)calloc(1+strlen(paramTypes),sizeof(char));
    strcpy(sym->paramTypes, paramTypes);
    sym->type = (char*)calloc(1+strlen(type),sizeof(char));
    strcpy(sym->type,type);
    sym->param = param;
    sym->linha = line;
    sym->coluna = column;
    sym->checkSem = False;
    sym->isFunction = isFunction;
    sym->numParam = numParam;
    return sym;
}

/*******************************************************************************
* Adiciona Símbolos à tabela                                                   *
*******************************************************************************/
void AddSymbol(table* tab, symbol* sym)
{
    tab->numSymbols++;
    tab->symbols = (symbol**)realloc(tab->symbols, tab->numSymbols*sizeof(symbol*));
    tab->symbols[tab->numSymbols-1] = sym;
}

/*******************************************************************************
* Imprime as tabela de símbolos                                                *
*******************************************************************************/
void printTables(table* tab)
{
    int i;
    while(tab!=NULL)
    {
        switch(tab->type)
        {
        case classTable:
            printf("===== Class %s Symbol Table =====\n", tab->name);
            break;
        case methodTable:
            printf("===== Method %s Symbol Table =====\n",tab->name); //nome da função
            break;
        }
        for(i=0; i<tab->numSymbols; i++)
        {
            printSymbol(tab->symbols[i]);
        }
        tab = tab->next;
        if(tab!=NULL)
        {
            printf("\n");
        }
    }
}

/*******************************************************************************
* Imprime os símbolos                                                    *
*******************************************************************************/
void printSymbol(symbol* sym)
{
    if(sym==NULL)
    {
        return;
    }
    printf("%s\t", sym->name);

    if(strcmp(sym->paramTypes,"") != 0){
      printf("%s", sym->paramTypes);
    }

    printf("\t%s", sym->type);
    switch(sym->param)
    {
    case paramParam:
        printf("\tparam");
        break;
    default:
        break;
    }
    printf("\n");
}





/*******************************************************************************
* Elimina as tabelas                                                    *
*******************************************************************************/
void clearTables(table* tab)
{
    if(tab == NULL)
    {
        return;
    }
    clearTables(tab->next);
    int i;
    for(i=0; i<tab->numSymbols; i++)
    {
        free(tab->symbols[i]->name);
        free(tab->symbols[i]->type);
        free(tab->symbols[i]);
    }
    free(tab->symbols);
    free(tab->name);
    free(tab);
    return;
}

/*******************************************************************************
* Coloca uma string em minúsculas                                              *
*******************************************************************************/
char* toLower(char* string)
{
    char* result = (char*)calloc(1+strlen(string), sizeof(char));
    int i;
    for(i=0; i<strlen(string); i++)
    {
        result[i] = tolower(string[i]);
    }
    return result;
}

/*Cria as tabelas*/
table* initTables(node* root){
	if(root == NULL){
		return NULL;
	}

  int i, j, k;
	/*int i,j,k,l;
  int contaArg = 1;*/
  /* Aqui vai buscar o nome à posição 0 dos filhos */
	table* global = createTable(classTable,root->children[0]->var);
	table* current;
	current = global;

  // Nós auxiliares para apontarem para os elementos da árvore
  node *aux;
  node *aux2, *aux3, *aux4, *aux5, *aux6;

  for(i=0 ; i<root->numChildren ; i++){

    aux = root->children[i];
    if(strcmp(aux->nodeTypeName, "FieldDecl") == 0){
      aux2 = aux->children[1];  //guarda o ID do nome
      // verifica se o Type é boolean
      if(strcmp(aux->children[0]->nodeTypeName, "Bool") == 0){
        AddSymbol(global, createSymbol(aux2->var, "", "boolean",nullParam,-1,-1,1,0));
      }
      else{
        AddSymbol(global, createSymbol(aux2->var, "", toLower(aux->children[0]->nodeTypeName),nullParam,-1,-1,1,0));
      }
    }

    if(strcmp(aux->nodeTypeName, "MethodDecl") == 0){
    	table* method;
      	char value[256];
      	int contaChildren = 0;
      	aux2 = aux->children[0]->children[1]; //guarda o ID do nome

      	aux3 = aux->children[0]->children[2]; //MethodParams para ver os paraâmetros
      	aux5 = aux->children[1];	//Guarda valor para utilizar nos varDecl
      	sprintf(value, "%s", "(");

      	for(j=0 ; j<aux3->numChildren ; j++){
        	aux4 = aux3->children[j];

        	if(aux3->numChildren <= 1){
	        	if(strcmp(aux4->children[0]->nodeTypeName,"StringArray") == 0){
	          		strcat(value, "String[]");
	        	}
            else if(strcmp(aux4->children[0]->nodeTypeName,"Bool") == 0){
                strcat(value, "boolean");
            }
	        	else{
	          		strcat(value, toLower(aux4->children[0]->nodeTypeName));
	        	}
        	}
        	else{
        		contaChildren++;
        		if(strcmp(aux4->children[0]->nodeTypeName,"StringArray") == 0){
	          		strcat(value, "String[]");
	        	}
            else if(strcmp(aux4->children[0]->nodeTypeName,"Bool") == 0){
                strcat(value, "boolean");
            }
	        	else{
	          		strcat(value, toLower(aux4->children[0]->nodeTypeName));
	        	}

	        	if(contaChildren != aux3->numChildren){
	        		strcat(value, ",");
	        	}

        	}
      	}

      	strcat(value, ")");
        if(strcmp(aux->children[0]->children[0]->nodeTypeName, "Bool") == 0){
            AddSymbol(global, createSymbol(aux2->var, value, "boolean",nullParam,-1,-1,1,0));
        }
        else{
            AddSymbol(global, createSymbol(aux2->var, value, toLower(aux->children[0]->children[0]->nodeTypeName),nullParam,-1,-1,1,0));
        }


      	// Iniciar as tabelas de métodos
      	char methodName[256];
      	sprintf(methodName, "%s", aux2->var);
      	strcat(methodName, value);

      	method = createTable(methodTable,methodName);
        if(strcmp(aux->children[0]->children[0]->nodeTypeName, "Bool") == 0){
            AddSymbol(method,createSymbol("return", "", "boolean", nullParam,-1,-1,0,0));
        }
        else{
            AddSymbol(method,createSymbol("return", "", toLower(aux->children[0]->children[0]->nodeTypeName), nullParam,-1,-1,0,0));
        }

      	current->next = method;
  	  	method->prev = current;
  	  	current = current->next;

  	  	// Se o método for o MAIN colocar os args
  	  	if(strcmp(methodName, "main(String[])") == 0){
  	  		AddSymbol(method,createSymbol("args", "", "String[]", paramParam,-1,-1,0,0));
  	  	}
  	  	else{
  	  		for(j=0 ; j<aux3->numChildren ; j++){
  	  			aux4 = aux3->children[j];
  	  			if(strcmp(aux4->children[0]->nodeTypeName, "Bool") == 0){
                AddSymbol(method,createSymbol(aux4->children[1]->var, "", "boolean", paramParam,-1,-1,0,0));
            }
            else if(strcmp(aux4->children[0]->nodeTypeName, "StringArray") == 0){
                AddSymbol(method,createSymbol(aux4->children[1]->var, "", "String[]", paramParam,-1,-1,0,0));
            }
            else{
                AddSymbol(method,createSymbol(aux4->children[1]->var, "", toLower(aux4->children[0]->nodeTypeName), paramParam,-1,-1,0,0));
            }
  	  		}
  	  	}

  	  // Ciclo que percorre os filhos de methodBody e vê as VarDecl
    	for(k=0 ; k<aux5->numChildren ; k++){
    		aux6 = aux5->children[k];

    		if(strcmp(aux6->nodeTypeName, "VarDecl") == 0){
    			if(strcmp(aux6->children[0]->nodeTypeName, "Bool") == 0){
            AddSymbol(method,createSymbol(aux6->children[1]->var, "", "boolean", nullParam,-1,-1,0,0));
          }
          else{
            AddSymbol(method,createSymbol(aux6->children[1]->var, "", toLower(aux6->children[0]->nodeTypeName), nullParam,-1,-1,0,0));
          }
    		}
    	}
	}
  }

	return global;
}

/*Retira o tipo de um Id*/
char* cutType(char* type){
	char *newType = malloc(256*sizeof(char));
	int i;
	for(i=0;i<strlen(type);i++){
		if(type[i]=='('){
			break;
		}else{
			newType[i]=type[i];
		}
	}
	newType[i] = '\0';
	return newType;
}

char* cutDerefPointer(char* type,int pos){
	char *newType = malloc(256*sizeof(char));
	int i;
	for(i=0;i<strlen(type)-pos;i++){
			newType[i]=type[i];
	}
	newType[i] = '\0';
	return newType;
}

int countPointer(char* type){
	int i;
	int contador = 0;
	for(i=0;i<strlen(type);i++){
		if(type[i]==','){
			contador++;
		}
	}
	return contador;
}

char* TransfPointer(char* type){
	char *newType = malloc(256*sizeof(char));
	int i;
	int isTransf = 0;
	for(i=0;i<strlen(type);i++){
		if(type[i]=='['){
			newType[i]='*';
			isTransf = 1;
			break;
		}else{
			newType[i]=type[i];
		}
	}
	if(isTransf){
		newType[i+1] = '\0';
	}else{
		newType[i] = '\0';
	}
	return newType;
}

int lengthEscape(char* string){
	int i,j;
	int contadorAux = 0;
	int contador = 0;
	int isEscape = 0;
	for(i=0;i<strlen(string);i++){
		if(string[i]=='\\'){
			if(string[i+1] == '\\'){
				i++;
			}
			isEscape = 1;
			contadorAux = 0;
			for(j=1;j<=3;j++){
				if(string[i+j]>='0' && string[i+j]<='7'){
					contadorAux++;
				}else{
					break;
				}
			}
			if(contadorAux==0){
				contador+=1;
			}else
			{
				contador+=contadorAux;
			}
		}
	}
	if(isEscape)
		return strlen(string)-contador-1;
	else
		return strlen(string)-1;
}

/*******************************************************************************
* Cria Árvore Anotada                                                          *
*******************************************************************************/
void TreeAnt(node* current, int level, table* tabela, table* atual){
	int i, j, k;
    int isGlobal=0;

    table* auxTable2 = tabela;
    table* auxTable3 = tabela;
    
	if(current == NULL){
		return;
	}

	for(i=0; i<current->numChildren; i++){
        TreeAnt(current->children[i], level+1,tabela,atual);
    }

   
    if(current->nodeType == EXP_node){
        if(strcmp(current->nodeTypeName, "Eq") == 0 ||
        strcmp(current->nodeTypeName, "Geq") == 0 ||
        strcmp(current->nodeTypeName, "Gt") == 0 ||
        strcmp(current->nodeTypeName, "Leq") == 0 ||
        strcmp(current->nodeTypeName, "Lt") == 0 ||
        strcmp(current->nodeTypeName, "Neq") == 0 ||
        strcmp(current->nodeTypeName, "And") == 0 ||
        strcmp(current->nodeTypeName, "Or") == 0
        )
        {
            strcpy(current->anot,"boolean");
        }
        if(strcmp(current->nodeTypeName, "Length") == 0){
            strcpy(current->anot,"int");
        }

        if(strcmp(current->nodeTypeName, "Not") == 0){
            if(strcmp(current->children[0]->anot, "int") == 0 || strcmp(current->children[0]->anot, "double") == 0){
                strcpy(current->anot, "undef");
            }
            else{
                strcpy(current->anot,"boolean");
            }
            
        }

        if(strcmp(current->nodeTypeName, "Minus") == 0 || strcmp(current->nodeTypeName, "Plus") == 0){
            strcpy(current->anot, current->children[0]->anot);
        }


        // Trata das anotações de Sub, Add, Mul, Div e Mod
        if(strcmp(current->nodeTypeName, "Sub") == 0 || strcmp(current->nodeTypeName, "Add") == 0 || strcmp(current->nodeTypeName, "Mul") == 0 || strcmp(current->nodeTypeName, "Div") == 0 || strcmp(current->nodeTypeName, "Mod") == 0){
            if(strcmp(current->children[0]->anot, current->children[1]->anot) == 0 ){        //No caso de serem operadores iguais
                strcpy(current->anot, current->children[0]->anot);
            }
            else if((strcmp(current->children[0]->anot, "int") == 0 && strcmp(current->children[1]->anot, "double") == 0) || // double - int || int - double
                (strcmp(current->children[1]->anot, "int") == 0 && strcmp(current->children[0]->anot, "double") == 0)){
                strcpy(current->anot, "double");
            }
            else if(strcmp(current->children[0]->anot, "undef") == 0 || strcmp(current->children[1]->anot, "undef") == 0){
                strcpy(current->anot, "undef");
            }

        }


    }
    else if(current->nodeType == ID_node){
        if(strcmp(current->var, "args") == 0){
            strcpy(current->anot,"String[]");
        }
        else{
            int localAndGlobal = 0;
            

            if(strcmp(current->parent->nodeTypeName, "MethodHeader") == 0){
                strcpy(actualMethod, current->var);
            }
     
            // Primeiro vai à tabela local
            isGlobal=0;
            atual = atual->next;
            while(atual != NULL){
                if(strcmp(cutType(atual->name), actualMethod) == 0){
                    for(j=0;j<atual->numSymbols;j++){ 
                        if(strcmp(current->var,atual->symbols[j]->name)==0){
                            strcpy(current->anot,atual->symbols[j]->type);
                            isGlobal=1;
                            localAndGlobal++;
                        }
                    }
                }
                atual = atual->next;
            } 

            if(isGlobal == 0){
                if(auxTable2->type == classTable){
                    for(j=0;j<auxTable2->numSymbols;j++){ 
                        if(strcmp(current->var,auxTable2->symbols[j]->name)==0){
                            strcpy(current->anot,auxTable2->symbols[j]->type);
                            isGlobal=1;
                            localAndGlobal++;
                        }
                    }
                }
            }
        }
    }
    else if(current->nodeType == DECLIT_node){
        if(strcmp(current->nodeTypeName, "DecLit") == 0){
            strcpy(current->anot,"int");
        }
    }
    else if(current->nodeType == STRLIT_node){
        if(strcmp(current->nodeTypeName, "Strlit") == 0){
            strcpy(current->anot,"String");
        }
    }
    else if(current->nodeType == REALLIT_node){
        if(strcmp(current->nodeTypeName, "RealLit") == 0){
            strcpy(current->anot,"double");
        }
    }
    else if(current->nodeType == BOOLLIT_node){
        if(strcmp(current->nodeTypeName, "BoolLit") == 0){
            strcpy(current->anot,"boolean");
        }
    }
    else if(current->nodeType == OTHER_node){

        if(strcmp(current->nodeTypeName, "ParseArgs") == 0){
            strcpy(current->anot,"int");
            strcpy(current->children[0]->anot, "String[]");
        }
        else if(strcmp(current->nodeTypeName, "Call") == 0){
        	strcpy(current->anot, "this");
            int numParams = current->numChildren - 1;
            int contaMetodo = 0;
            int contaMetodoVazio = 0;


           
            if(strcmp(current->children[0]->nodeTypeName, "Id") == 0){      //Verifica o primeiro filho do Call
                if(tabela->type == classTable){     //verifica na GLOBAL Table
                    for(j=0;j<tabela->numSymbols;j++){
                        if(strcmp(current->children[0]->var,tabela->symbols[j]->name)==0 && tabela->symbols[j]->isFunction == 1){      //Coloca anotação no filho do Call
                            if(countPointer(tabela->symbols[j]->paramTypes)+1 == numParams ){
                                strcpy(current->children[0]->anot,tabela->symbols[j]->paramTypes);
                                contaMetodo++;
                                strcpy(current->anot, tabela->symbols[j]->type);
                            } 
                            if(countPointer(tabela->symbols[j]->paramTypes) == 0 && numParams-1== -1){
                                contaMetodoVazio++;
                                strcpy(current->children[0]->anot,tabela->symbols[j]->paramTypes);
                                strcpy(current->anot, tabela->symbols[j]->type);
                            }
                      	}
                    }
                }
            }

            // Quando o Call não tem anotação coloca UNDEF nele e no filho
            if(strcmp(current->anot, "") == 0 || contaMetodo >= 2 || contaMetodoVazio >=2){
                strcpy(current->anot, "undef");
                strcpy(current->children[0]->anot,"undef");
            }

        }
        /***************************************************
        *Retira as anotações dos Id's que não queremos aqui*
        ****************************************************/
        else if(strcmp(current->nodeTypeName, "VarDecl") == 0 || strcmp(current->nodeTypeName, "ParamDecl") == 0 ||
                strcmp(current->nodeTypeName, "FieldDecl") == 0 || strcmp(current->nodeTypeName, "MethodHeader") == 0){
            if(strcmp(current->children[1]->nodeTypeName, "Id") == 0){
                strcpy(current->children[1]->anot, "");
            }
        }
        else{
            if(strcmp(current->nodeTypeName, "Assign") == 0){
            	strcpy(current->anot, "this");
            	char nomeId[256];
            	int assignPos = 0;
            	int vardeclPos = 0;

              
            	//Verifica as posições do assign e vardecl para ver se aparece antes ou depois 
            	//É escolhida a global ou local table conforme
                if(strcmp(current->parent->nodeTypeName, "MethodBody") == 0){
                	strcpy(nomeId, current->children[0]->var);
                	for(j=0 ; j<current->parent->numChildren ; j++){
                		if(strcmp(current->parent->children[j]->nodeTypeName, "VarDecl") == 0 && strcmp(nomeId, current->parent->children[j]->children[1]->var) == 0){
                			vardeclPos = j;
                		}
                		if(strcmp(current->parent->children[j]->nodeTypeName, "Assign") == 0 && strcmp(current->parent->children[j]->anot, "this") == 0){
                			assignPos = j;
                		}
                	}
                }

                if(assignPos < vardeclPos){
                	//Global
                	for(k=0;k<auxTable2->numSymbols;k++){ 
                        if(strcmp(current->children[0]->var,auxTable2->symbols[k]->name)==0){
                            strcpy(current->children[0]->anot,auxTable2->symbols[k]->type);
                        }
            		}
                }
                else{
                	//local
                	auxTable3 = auxTable3->next;
		            while(auxTable3 != NULL){
		                if(strcmp(cutType(auxTable3->name), actualMethod) == 0){
		                    for(k=0;k<auxTable3->numSymbols;k++){ 
		                        if(strcmp(current->children[0]->var,auxTable3->symbols[k]->name)==0){
		                            strcpy(current->children[0]->anot,auxTable3->symbols[k]->type);
		                        }
		                    }
		                }
		                auxTable3 = auxTable3->next;
		            }

                }

                if(strcmp(current->children[0]->nodeTypeName, "Id") == 0){      //Verifica o primeiro filho do Call
                    strcpy(current->anot, current->children[0]->anot);
                }
            }
        }
    }
}

void checkGlobalTable(node* current, table* tab){
    int i, j;
    /*Variáveis globais e funçoes com o mesmo nome*/
    if(current->nodeType == ID_node){
      for(i=0 ; i<tab->numSymbols ; i++){
        if(strcmp(tab->symbols[i]->name, current->var) == 0 && tab->symbols[i]->checkSem==False){
          tab->symbols[i]->checkSem = True;
          for(j=i+1 ; j<tab->numSymbols ; j++){
            if(strcmp(tab->symbols[i]->name, tab->symbols[j]->name) == 0){
              printf("Line <linha>, col <coluna>: Symbol %s already defined\n", current->var);
            }
          }
        }
      }
  }
}
