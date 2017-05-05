/************************************************************************************
* Ficheiro "symbols.y"                                                              *
* Analisador Sintático da linguagem Ja - Sub-conjunto de Java                       *
* Cadeira de Compiladores - 2017 - Licenciatura em Engenharia Informática           *
* Manuel Madeira Amado - 2006131282                                                 *
* Xavier Silva - 2013153577                                                         *
* Versão 0.2                                                                        *
************************************************************************************/

#ifndef SYMBOLS_H_INCLUDED
#define SYMBOLS_H_INCLUDED

typedef enum {classTable, methodTable} table_type;
typedef enum {paramParam, nullParam} param_type;
typedef enum {True, False} bool;

typedef struct symbol
{
    char* name;
    char *paramTypes;
    char *type;
    param_type param;
    int isFunction; /*0 -não é 1-é*/
    long long int linha;
    long long int coluna;
    bool checkSem;
    int numParam;
    int decl;
} symbol;

typedef struct table
{
    table_type type;
    char* name;
    bool check; /*Verificação - função aparece nas declarações e nas definições*/
    bool checkSem;
    struct table* next;
    struct table* prev;
    int numSymbols;
    int declared;
    symbol** symbols;

} table;

table* createTable(table_type, char* name);
symbol* createSymbol(char* name, char* paramTypes ,char* type, param_type param, long long int line, long long int column, int isFunction, int numParam);
void AddSymbol(table* tab, symbol* sym);
void printTables(table* tab);
void printSymbol(symbol* sym);
table* initTables(node* root);
char* toLower(char* string);
void clearTables(table* tab);
char* cutType(char* type);
void TreeAnt(node* current, int level, table* tabela, table* atual);

#endif // SYMBOLS_H_INCLUDED
