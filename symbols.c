/************************************************************************************
* Ficheiro "symbols.c"                                                              *
* Analisador Sintático da linguagem Ja - Sub-conjunto de Java                       *
* Cadeira de Compiladores - 2017 - Licenciatura em Engenharia Informática           *
* Manuel Madeira Amado - 2006131282                                                 *
* Xavier Silva - 2013153577                                                         *
* Versão 0.13                                                                       *
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
char actualMethod[512];
char tempo[512];
char tempoUno[512];
char* tokenUno;


/*******************************************************************************
* Opções de DEBUG:                                                             *
* 1 - Imprime o id encontrado e a tabela atual onde vai pesquisar              *
*******************************************************************************/
int DEBUGS = 0;


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
    tab->declared = 0;
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
    sym->decl = 0;
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
      	char value[512];
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
      	char methodName[512];
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
	char *newType = malloc(512*sizeof(char));
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
/*Retira o tipo de um Id*/
char* cutPar(char* type){
	char *newType = malloc(512*sizeof(char));
	int i;
    int a;
    a = 0;
	for(i=0;i<strlen(type);i++){
        //printf("%s\n", newType);
        if(type[i] != '('){
            if(type[i] != ')'){
                newType[a]=type[i];
                a++;
            }
        }else{
            if(type[i]=='\0'){
                newType[a] = '\0';
                break;
            }
        }
	}
	return newType;
}

char* cutDerefPointer(char* type,int pos){
	char *newType = malloc(512*sizeof(char));
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
	char *newType = malloc(512*sizeof(char));
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
/********************************************************************************
* DESCRIÇÃO: Conta o número de argumentos de uma dada string    				*
* RECEBE: ponteiro para a string e int objectivo				            	*
* DEVOLVE: 1 se confirma, 0 se não confirma										*
*********************************************************************************/
int ContaArgumento(char* str, int obj){
    int comprimento, conta, i;
    comprimento = strlen(str);
    conta = 0;
    for(i=0; i<=comprimento; i++){
        if(str[i]==','){ //conta as virgulas
            conta++;
        }
    }
    if(conta+1 == obj){
        return 1;
    }
    return 0;
}

/*******************************************************************************
* Cria Árvore Anotada RECURSIVAMENTE                                           *
*******************************************************************************/
void TreeAnt(node* current, int level, table* tabela, table* atual){
	int i = 0;
    int j = 0;
    //int k;
    int isGlobal=0;

    //table* auxTable2 = tabela;
    //table* auxTable3 = tabela;

/*--Condição de Paragem (Folha)-----------------------------------------------*/
	if(current == NULL){
		return;
	}

    if(strcmp(current->nodeTypeName, "Program") == 0){
        tabela->declared = 1;
    }
/*..Início de sub-árvore de Método.............................................*/
    if(strcmp(current->nodeTypeName, "MethodDecl") == 0){
        //printf("Método: %s\n", current->children[0]->children[1]->var);
        atual = tabela;
        for(i=0; i<tabela->numSymbols; i++){
            if(strcmp(cutType(atual->name), current->children[0]->children[1]->var) == 0 && atual->declared == 0){
                atual->declared = 1;
                i=tabela->numSymbols;
            }else{
                atual = atual->next;
            }

        }
        //while((strcmp(cutType(atual->name), current->children[0]->children[1]->var) != 0 && atual->declared == 0) ){
    	//	atual = atual->next;
    	//}
        //printf("Tabela atual: %s\n", atual->name);
    }
/*..Activa uma variável após a sua declaração..................................*/
    if( strcmp(current->nodeTypeName, "VarDecl") == 0 ||
        strcmp(current->nodeTypeName, "ParamDecl") == 0){
        for(i=0; i<atual->numSymbols; i++){
            if(strcmp(current->children[1]->var, atual->symbols[i]->name) == 0){
                atual->symbols[i]->decl = 1;
                //printf("Activou %s act=%d\n", current->children[1]->var, atual->symbols[j]->decl);
            }
        }
    }
    //printf("Global:%s Atual:%s\n", tabela->name, atual->name);
/*--Chamada Recursiva---------------------------------------------------------*/
	for(i=0; i<current->numChildren; i++){
        TreeAnt(current->children[i], level+1,tabela,atual);
    }

/*--ID_node--Sem Olhar a VarDecl-----------------------------------------------*/
        if(current->nodeType == ID_node){
            isGlobal=0; //flag de entrada numa tabela
            for(j=0;j<atual->numSymbols;j++){
                if(strcmp(current->var,atual->symbols[j]->name)==0 && atual->symbols[j]->decl == 1){
                    strcpy(current->anot,atual->symbols[j]->type);
                    //strcpy(current->anot,"test");//aqui está a falha! Não sei porquê
                    isGlobal=1;
                }
            }
            if(isGlobal == 0){
                for(j=0;j<tabela->numSymbols;j++){
                    if(strcmp(current->var,tabela->symbols[j]->name)==0){
                        strcpy(current->anot,tabela->symbols[j]->type);
                        isGlobal=1;
                    }
                }
            }
            if(strcmp(current->anot, "") == 0 && strcmp(cutType(tabela->name),current->var)){
                strcpy(current->anot, "undef");
            }
        }

 /*--EXP_node------------------------------------------------------------------*/
 /*..Eq..Geq..Gt..Leq..Lt..Neq..And..Or........................................*/
    if(current->nodeType == EXP_node){
        if( strcmp(current->nodeTypeName, "Eq") == 0 ||
            strcmp(current->nodeTypeName, "Geq") == 0 ||
            strcmp(current->nodeTypeName, "Gt") == 0 ||
            strcmp(current->nodeTypeName, "Leq") == 0 ||
            strcmp(current->nodeTypeName, "Lt") == 0 ||
            strcmp(current->nodeTypeName, "Neq") == 0 ||
            strcmp(current->nodeTypeName, "And") == 0 ||
            strcmp(current->nodeTypeName, "Or") == 0)
        {
            strcpy(current->anot,"boolean");
        }
/*..Length.....................................................................*/
        if(strcmp(current->nodeTypeName, "Length") == 0){
            strcpy(current->anot,"int");
        }
/*..Not........................................................................*/
        if(strcmp(current->nodeTypeName, "Not") == 0){
            if( strcmp(current->children[0]->anot, "int") == 0 ||
                strcmp(current->children[0]->anot, "double") == 0)
            {
                strcpy(current->anot, "undef");
            }else{
                strcpy(current->anot,"boolean");
            }
        }
/*..Minus..Plus................................................................*/
        if( strcmp(current->nodeTypeName, "Minus") == 0 ||
            strcmp(current->nodeTypeName, "Plus") == 0)
        {
            strcpy(current->anot, current->children[0]->anot);
        }
/*..Sub..Add..Mul..Div..Mod....................................................*/
        if( strcmp(current->nodeTypeName, "Sub") == 0 ||
            strcmp(current->nodeTypeName, "Add") == 0 ||
            strcmp(current->nodeTypeName, "Mul") == 0 ||
            strcmp(current->nodeTypeName, "Div") == 0 ||
            strcmp(current->nodeTypeName, "Mod") == 0)
        {
            //No caso de serem operadores iguais
            if(strcmp(current->children[0]->anot, current->children[1]->anot) == 0 ){
                strcpy(current->anot, current->children[0]->anot);
            }else{
            // double - int || int - double
                if((strcmp(current->children[0]->anot, "int") == 0 &&
                    strcmp(current->children[1]->anot, "double") == 0) ||
                    (strcmp(current->children[1]->anot, "int") == 0 &&
                    strcmp(current->children[0]->anot, "double") == 0))
                {
                    strcpy(current->anot, "double");
                }else{
            //um dos operadores é indefinido
                    if(strcmp(current->children[0]->anot, "undef") == 0 ||
                        strcmp(current->children[1]->anot, "undef") == 0)
                    {
                        strcpy(current->anot, "undef");
                    }
                }
            }
        }
    }
/*--DECLIT_node---------------------------------------------------------------*/
    else{
        if(current->nodeType == DECLIT_node){
            if(strcmp(current->nodeTypeName, "DecLit") == 0){
                strcpy(current->anot,"int");
            }
        }
/*--STRLIT_node---------------------------------------------------------------*/
        else{
            if(current->nodeType == STRLIT_node){
                if(strcmp(current->nodeTypeName, "Strlit") == 0){
                    strcpy(current->anot,"String");
                }
            }
/*--REALLIT_node--------------------------------------------------------------*/
            else{
                if(current->nodeType == REALLIT_node){
                    if(strcmp(current->nodeTypeName, "RealLit") == 0){
                        strcpy(current->anot,"double");
                    }
                }
/*--BOOLLIT_node---------------------------------------------------------------*/
                else{
                    if(current->nodeType == BOOLLIT_node){
                        if(strcmp(current->nodeTypeName, "BoolLit") == 0){
                            strcpy(current->anot,"boolean");
                        }
                    }
/*--OTHER_node----------------------------------------------------------------*/
                    else{
                        if(current->nodeType == OTHER_node){
                            if(strcmp(current->nodeTypeName, "ParseArgs") == 0){
                                strcpy(current->anot,"int");
                                strcpy(current->children[0]->anot, "String[]");
                            }else{
/*--Call----------------------------------------------------------------------*/
                                if(strcmp(current->nodeTypeName, "Call") == 0){
                                    strcpy(current->anot, ""); //coloca a anotação vazia
                                    int numParams = current->numChildren - 1; //conta quantos parâmetros tem
                                    int contaMetodo = 0; //conta o número de métodos iguais encontrados
                                    int argumentos = 0;
/*--Call Sem Parâmetros--------------------------------------------------------*/
                                    if(numParams==0){
                                        contaMetodo = 0; //reset da variável
                                        for(i=0; i<tabela->numSymbols; i++){
                                            if( strcmp(tabela->symbols[i]->name, current->children[0]->var)==0 && //mesmo nome
                                                strcmp(tabela->symbols[i]->paramTypes, "()") == 0 &&                   //sem args
                                                tabela->symbols[i]->isFunction == 1)                              //é func e não var
                                            {
                                                contaMetodo++;
                                                if(contaMetodo == 1){                                         //anota o 1º que encontra
                                                    strcpy(current->anot, tabela->symbols[i]->type);
                                                    strcpy(current->children[0]->anot, tabela->symbols[i]->paramTypes);
                                                }
                                            }
                                        }
                                        //demasiados matches - ambíguo
                                        if(contaMetodo >= 2){
                                            strcpy(current->anot, "undef");
                                            strcpy(current->children[0]->anot, "undef");
                                        }
                                        //não encontrado método com esse nome
                                        if(contaMetodo == 0){
                                            strcpy(current->anot, "undef");
                                            strcpy(current->children[0]->anot, "undef");
                                        }
                                    }
/*--Call C/ 1 Parâmetro--------------------------------------------------------*/
                                    if(numParams==1){
                                        contaMetodo = 0; //reset da variável
                                        for(i=0; i<tabela->numSymbols; i++){
                                            tokenUno = cutPar(tabela->symbols[i]->paramTypes);
                                            if( (strcmp(tabela->symbols[i]->name, current->children[0]->var)==0) &&
                                             (strcmp(tokenUno, current->children[1]->anot) == 0 ) &&
                                             (tabela->symbols[i]->isFunction == 1))                              //é func e não var
                                            {
                                                contaMetodo++;
                                                if(contaMetodo == 1){                                         //anota o 1º que encontra
                                                    strcpy(current->anot, tabela->symbols[i]->type);
                                                    strcpy(current->children[0]->anot, tabela->symbols[i]->paramTypes);
                                                }
                                            }
                                        }
                                        // caso não haja um exact match usa casos compatíveis -ints em doubles
                                        if(contaMetodo == 0){
                                            for(i=0; i<tabela->numSymbols; i++){
                                                tokenUno = cutPar(tabela->symbols[i]->paramTypes);
                                                if( (strcmp(tabela->symbols[i]->name, current->children[0]->var)==0) &&
                                                (strcmp(tokenUno, "double") == 0 && strcmp(current->children[1]->anot, "int") == 0) && //Casos Compatíveis
                                                (tabela->symbols[i]->isFunction == 1))                              //é func e não var
                                                {
                                                    contaMetodo++;
                                                    if(contaMetodo == 1){                                         //anota o 1º que encontra
                                                        strcpy(current->anot, tabela->symbols[i]->type);
                                                        strcpy(current->children[0]->anot, tabela->symbols[i]->paramTypes);
                                                    }
                                                }
                                            }
                                        }

                                        //demasiados matches - ambíguo
                                        if(contaMetodo >= 2){
                                            strcpy(current->anot, "undef");
                                            strcpy(current->children[0]->anot, "undef");
                                        }
                                        //não encontrado método com esse nome
                                        if(contaMetodo == 0){
                                            strcpy(current->anot, "undef");
                                            strcpy(current->children[0]->anot, "undef");
                                        }
                                    }
/*--Call COM 2 ou + Parâmetros--------------------------------------------------------*/
                                    if(numParams>1){
                                        int argCertos = 0;
                                        contaMetodo = 0; //reset da variável
                                        argumentos = 0;
                                        for(i=0; i<tabela->numSymbols; i++){
                                            argCertos = 0;
                                            if(strcmp(tabela->symbols[i]->name, current->children[0]->var)==0){ //mesmo nome
                                                if(tabela->symbols[i]->isFunction == 1){
                                                    argumentos = ContaArgumento(tabela->symbols[i]->paramTypes, numParams);
                                                    if(argumentos == 1){ //mesmo número de args
                                                        char* token;
                                                        strcpy(tempo, tabela->symbols[i]->paramTypes);
                                                        token = strtok(tempo, ",()");
//--------------ciclo dos argumentos--------------------------------------------
                                                        for(j=1; j<=numParams; j++){
                                                            if(strcmp(token, current->children[j]->anot)==0){
                                                                token = strtok(NULL, ",()");
                                                                argCertos++;
                                                            }else{
                                                                argCertos = 0;
                                                                j = numParams;
                                                            }
                                                        }
                                                        if(argCertos == numParams){
                                                            contaMetodo++;
                                                            if(contaMetodo == 1){                                         //anota o 1º que encontra
                                                                strcpy(current->anot, tabela->symbols[i]->type);
                                                                strcpy(current->children[0]->anot, tabela->symbols[i]->paramTypes);
                                                            }
                                                        }

                                                    }
                                                }

                                            }
                                        }
                                        if(contaMetodo == 0){
                                            for(i=0; i<tabela->numSymbols; i++){
                                                argCertos = 0;
                                                if(strcmp(tabela->symbols[i]->name, current->children[0]->var)==0){ //mesmo nome
                                                    if(tabela->symbols[i]->isFunction == 1){
                                                        argumentos = ContaArgumento(tabela->symbols[i]->paramTypes, numParams);
                                                        if(argumentos == 1){ //mesmo número de args
                                                            char* token;
                                                            strcpy(tempo, tabela->symbols[i]->paramTypes);
                                                            token = strtok(tempo, ",()");
    //--------------ciclo dos argumentos--------------------------------------------
                                                            for(j=1; j<=numParams; j++){
                                                                if(strcmp(token, current->children[j]->anot)==0 ||(strcmp(token, "double")==0 && strcmp(current->children[j]->anot, "int")==0)){
                                                                    token = strtok(NULL, ",()");
                                                                    argCertos++;
                                                                }else{
                                                                    argCertos = 0;
                                                                    j = numParams;
                                                                }
                                                            }
                                                            if(argCertos == numParams){
                                                                contaMetodo++;
                                                                if(contaMetodo == 1){                                         //anota o 1º que encontra
                                                                    strcpy(current->anot, tabela->symbols[i]->type);
                                                                    strcpy(current->children[0]->anot, tabela->symbols[i]->paramTypes);
                                                                }
                                                            }

                                                        }
                                                    }

                                                }
                                            }
                                        }
                                        //demasiados matches - ambíguo
                                        if(contaMetodo >= 2){
                                            strcpy(current->anot, "undef");
                                            strcpy(current->children[0]->anot, "undef");
                                        }
                                        //não encontrado método com esse nome
                                        if(contaMetodo == 0){
                                            strcpy(current->anot, "undef");
                                            strcpy(current->children[0]->anot, "undef");
                                        }
                                    }
                                }
                            }
                        }
                    }
                }
            }
        }
    }
    if( strcmp(current->nodeTypeName, "VarDecl") == 0 ||
        strcmp(current->nodeTypeName, "ParamDecl") == 0 ||
        strcmp(current->nodeTypeName, "FieldDecl") == 0 ||
        strcmp(current->nodeTypeName, "MethodHeader") == 0)
    {
        if(strcmp(current->children[1]->nodeTypeName, "Id") == 0){
            strcpy(current->children[1]->anot, "");
        }
    }else{
        if(strcmp(current->nodeTypeName, "Assign") == 0){
            strcpy(current->anot, "this");

            if(strcmp(current->children[0]->nodeTypeName, "Id") == 0){      //Verifica o primeiro filho do Call
                strcpy(current->anot, current->children[0]->anot);
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
