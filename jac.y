/************************************************************************************
* Ficheiro "jac.y"																	*
* Analisador Sintático da linguagem Ja - Sub-conjunto de Java						*
* Cadeira de Compiladores - 2017 - Licenciatura em Engenharia Informática			*
* Manuel Madeira Amado - 2006131282													*
* Xavier Silva - 2013153577															*
* Versão 0.20																		*
************************************************************************************/

%{
	#include <stdio.h>
	#include <string.h>
	#include <stdlib.h>
	#include <stdio.h>
	#include "y.tab.h"

	#include "tree.h"
	#include "symbols.h"
	#define VAR (char*)calloc(1,sizeof(char))

	int yylex(void);
	int yyerror(const char *s);

	extern long long int contaLinha;
	extern long long int contaColuna;
	extern char* yytext;

	int valor1=0;
	int valorT=0;
	int valorNull=0;
	int valorL=0;
	int valorS = 0;
	int contaErros = 0;

	node* root;
	node* aux;
	node* aux2;
	node* aux3;

	char* auxType;

	table* tab;
%}

%union{
    char* val;
    struct node* no;
};

%token PRECEDENCE
%token <val> STRLIT REALLIT DECLIT BOOLLIT ID
%token BOOL
%token INT
%token DOUBLE
%token RESERVED
%token CLASS
%token DO
%token DOTLENGTH
%token ELSE
%token IF
%token PARSEINT
%token PRINT
%token PUBLIC
%token RETURN
%token STATIC
%token STRING
%token VOID
%token WHILE
%token OCURV
%token CCURV
%token OBRACE
%token CBRACE
%token OSQUARE
%token CSQUARE
%token AND
%token OR
%token LT
%token GT
%token EQ
%token NEQ
%token LEQ
%token GEQ
%token PLUS
%token MINUS
%token STAR
%token DIV
%token MOD
%token NOT
%token ASSIGN
%token SEMI
%token COMMA

%nonassoc LOWER_THAN_ELSE
%nonassoc ELSE

%left COMMA
%right ASSIGN
%left OR
%left AND
%left EQ NEQ
%left GEQ GT LT LEQ
%left PLUS MINUS
%left STAR DIV MOD
%right NOT
%right PRECEDENCE
%left OBRACE OCURV OSQUARE CBRACE CCURV CSQUARE

%type <no> Program InitDeclaration FieldDecl CommaID MethodDecl MethodHeader MethodBody MethodParams FormalParams CommaTypeID VarDecl VarBody Type Statement Assignment MethodInvocation CommaExpr ParseArgs Expr ParamDecl StatementList ExprAux

%%

/*********************************************************************
* Program → CLASS ID OBRACE { FieldDecl | MethodDecl | SEMI } CBRACE *
**********************************************************************/
Program: CLASS ID OBRACE CBRACE	{
										if(contaErros == 0 && valorT == 1){
											$$ = createNode(OTHER_node, "Program", "");
											aux = createNode(ID_node, "Id", $2);
											addChild($$,aux);
											root = $$;
										}
										free($2);

								}
		| CLASS ID OBRACE InitDeclaration CBRACE	{
														if(contaErros == 0 && valorT == 1){
															$$ = createNode(OTHER_node, "Program", "");
															aux = createNode(ID_node, "Id", $2);
															addChild($$,aux);
															addBrother(aux,$4);
															root = $$;
														}
														free($2);
													}
		;

InitDeclaration: FieldDecl						{
													if(contaErros == 0 && valorT == 1){
														$$ = $1;
													}
												}
			| MethodDecl						{
													if(contaErros == 0 && valorT == 1){
														$$ = $1;
													}
												}
			| SEMI								{
													if(contaErros == 0 && valorT == 1){
														$$ = NULL;
													}
												}
			| InitDeclaration FieldDecl			{
													if(contaErros == 0 && valorT == 1){
														$$ = $1;
														addBrother($$,$2);
													}

												}
			| InitDeclaration MethodDecl		{
													if(contaErros == 0 && valorT == 1){
														$$ = $1;
														addBrother($$,$2);
													}
												}
			| InitDeclaration SEMI				{
													if(contaErros == 0 && valorT == 1){
														$$ = $1;
													}
												}
			;

/*********************************************************************
* FieldDecl → PUBLIC STATIC Type ID { COMMA ID } SEMI				 *
*********************************************************************/
FieldDecl: PUBLIC STATIC Type ID SEMI				{
														if(contaErros == 0 && valorT == 1){
															$$ = createNode(OTHER_node, "FieldDecl", "");
															addChild($$,$3);
															aux = createNode(ID_node, "Id", $4);
															addBrother($3,aux);
														}
														free($4);
													}
			| PUBLIC STATIC Type ID CommaID SEMI	{
														if(contaErros == 0 && valorT == 1){
															$$ = createNode(OTHER_node, "FieldDecl", "");
															addChild($$,$3);
															aux = createNode(ID_node, "Id", $4);
															addChild($$,aux);
															addBrother($$,$5);
														}
														free($4);
													}
			| error SEMI							{$$ = NULL; if (contaErros == 0 && valorT == 1) {clearTree(root);}}
			;

CommaID: COMMA ID 						{
											if(contaErros == 0 && valorT == 1){
												$$ = createNode(OTHER_node, "FieldDecl", "");
												aux = createNode(OTHER_node, auxType, "");
												aux2 = createNode(ID_node, "Id", $2);
												addChild($$,aux);
												addBrother(aux,aux2);
											}
										}
			| COMMA ID CommaID			{
											if (contaErros == 0 && valorT == 1){
												$$ = createNode(OTHER_node, "FieldDecl", "");
												aux = createNode(OTHER_node, auxType, "");
												aux2 = createNode(ID_node, "Id", $2);
												addChild($$,aux);
												addBrother(aux,aux2);
												addBrother($$,$3);
											}
										}
			;


/*********************************************************************
* MethodDecl → PUBLIC STATIC MethodHeader MethodBody				 *
*********************************************************************/
MethodDecl: PUBLIC STATIC MethodHeader MethodBody		{
															if(contaErros == 0 && valorT == 1){
																$$ = createNode(OTHER_node, "MethodDecl", "");
																aux = createNode(OTHER_node, "MethodHeader", "");
																aux2 = createNode(OTHER_node, "MethodBody", "");
																addChild($$,aux);
																addBrother(aux,aux2);
																addChild(aux,$3);
																addChild(aux2,$4);
															}
														}
														;


/*********************************************************************
* MethodHeader → ( Type | VOID ) ID OCURV [ FormalParams ] CCURV	 *
*********************************************************************/
MethodHeader: Type ID OCURV FormalParams CCURV		{
														if(contaErros == 0 && valorT == 1){
															$$ = $1;
															aux = createNode(ID_node, "Id", $2);
															addBrother($$,aux);
															aux2 = createNode(OTHER_node, "MethodParams", "");
															addBrother(aux,aux2);
															addChild(aux2,$4);
														}
														free($2);
													}
			| Type ID OCURV CCURV					{
														if(contaErros == 0 && valorT == 1){
															$$ = $1;
															aux = createNode(ID_node, "Id", $2);
															addBrother($$,aux);
															aux2 = createNode(OTHER_node, "MethodParams", "");
															addBrother(aux,aux2);
														}
														free($2);
													}
			| VOID ID OCURV FormalParams CCURV		{
														if(contaErros == 0 && valorT == 1){
															$$ = createNode(OTHER_node, "Void", "");
															aux = createNode(ID_node, "Id", $2);
															addBrother($$,aux);
															aux2 = createNode(OTHER_node, "MethodParams", "");
															addBrother(aux,aux2);
															addChild(aux2,$4);
														}
														free($2);
													}
			| VOID ID OCURV CCURV					{
														if(contaErros == 0 && valorT == 1){
															$$ = createNode(OTHER_node, "Void", "");
															aux = createNode(ID_node, "Id", $2);
															addBrother($$,aux);
															aux2 = createNode(OTHER_node, "MethodParams", "");
															addBrother(aux,aux2);
														}
														free($2);
													}
			;


/*********************************************************************
* MethodBody → OBRACE { VarDecl | Statement } CBRACE				 *
*********************************************************************/
MethodBody: OBRACE CBRACE							{
														if(contaErros == 0 && valorT == 1){
															$$ = NULL;
														}
													}
			| OBRACE MethodParams CBRACE			{
														if(contaErros == 0 && valorT == 1){
															$$ = $2;
														}
													}
			;

MethodParams: VarDecl								{
														if(contaErros == 0 && valorT == 1){
															$$ = $1;
														}
													}
			| Statement								{
														if(contaErros == 0 && valorT == 1){
															$$ = $1;
														}
													}
			| MethodParams VarDecl					{
														if(contaErros == 0 && valorT == 1){
															$$ = $1;
															addBrother($$,$2);
														}
													}
			| MethodParams Statement				{
														if(contaErros == 0 && valorT == 1){
															$$ = $1;
															addBrother($$,$2);
														}
													}
			;


/*********************************************************************
* FormalParams → ParamDecl							                 *
* Limita-se a fazer a ligação entre FormalParams e ParamDecl		 *
*********************************************************************/
FormalParams: ParamDecl					{
											if (contaErros == 0 && valorT == 1){
												$$ = $1;
											}
										}
			;

/*********************************************************************
* FormalParams → Type ID { COMMA Type ID }							 *
* FormalParams → STRING OSQUARE CSQUARE ID							 *
*********************************************************************/
ParamDecl:	Type ID 								{
														if (contaErros == 0 && valorT == 1){
															$$ = createNode(OTHER_node, "ParamDecl", "");
															addChild($$,$1);
															aux = createNode(ID_node, "Id", $2);
															addBrother($1,aux);
														}
														free($2);
													}
			| Type ID CommaTypeID					{
														if (contaErros == 0 && valorT == 1){
															$$ = createNode(OTHER_node, "ParamDecl", "");
															addChild($$,$1);
															aux = createNode(ID_node, "Id", $2);
															addBrother($1,aux);
															addBrother($$,$3);
														}
														free($2);
													}
			| STRING OSQUARE CSQUARE ID 			{
														if (contaErros == 0 && valorT == 1){
															$$ = createNode(OTHER_node, "ParamDecl", "");
															aux = createNode(OTHER_node, "StringArray", "");
															aux2 = createNode(ID_node, "Id", $4);
															addChild($$,aux);
															addChild($$,aux2);
														}
														free($4);
													}
			;

CommaTypeID: COMMA Type ID 							{
														if (contaErros == 0 && valorT == 1){
															$$ = createNode(OTHER_node, "ParamDecl", "");
															addChild($$,$2);
															aux = createNode(ID_node, "Id", $3);
															addBrother($2,aux);
														}
														free($3);
													}
			| COMMA Type ID CommaTypeID				{
														if (contaErros == 0 && valorT == 1){
															$$ = createNode(OTHER_node, "ParamDecl", "");
															addChild($$,$2);
															aux = createNode(ID_node, "Id", $3);
															addBrother($2,aux);
															addBrother($$,$4);
														}
														free($3);
													}
			;


/*********************************************************************
* VarDecl → Type ID { COMMA ID } SEMI 								 *
*********************************************************************/
VarDecl:  Type ID SEMI								{
														if (contaErros == 0 && valorT == 1){
															$$ = createNode(OTHER_node, "VarDecl", "");
															addChild($$,$1);
															aux = createNode(ID_node, "Id", $2);
															addBrother($1,aux);
														}
														free($2);
													}

		| Type ID VarBody SEMI						{
														if (contaErros == 0 && valorT == 1){
															$$ = createNode(OTHER_node, "VarDecl", "");
															addChild($$,$1);
															aux = createNode(ID_node, "Id", $2);
															addBrother($1,aux);
															addBrother($$,$3);
														}
														free($2);
													}
		;

VarBody: COMMA ID 								{
													if (contaErros == 0 && valorT == 1){
														$$ = createNode(OTHER_node, "VarDecl", "");
														aux = createNode(OTHER_node, auxType, "");
														aux2 = createNode(ID_node, "Id", $2);
														addChild($$,aux);
														addBrother(aux,aux2);
													}
													free($2);
												}

		| COMMA ID VarBody						{
													if (contaErros == 0 && valorT == 1){
														$$ = createNode(OTHER_node, "VarDecl", "");
														aux = createNode(OTHER_node, auxType, "");
														aux2 = createNode(ID_node, "Id", $2);
														addChild($$,aux);
														addBrother(aux,aux2);
														addBrother($$,$3);
													}
													free($2);
												}
		;


/*********************************************************************
* Type → BOOL | INT | DOUBLE										 *
*********************************************************************/
Type: BOOL 											{
														if (contaErros == 0 && valorT == 1){
															$$ = createNode(OTHER_node, "Bool", "");
															auxType = (char*)calloc(10,sizeof(char));
															strncpy(auxType, "Bool", strlen("Bool"));
														}
													}
	| INT 											{
														if (contaErros == 0 && valorT == 1){
															$$ = createNode(OTHER_node, "Int", "");
															auxType = (char*)calloc(10,sizeof(char));
															strncpy(auxType, "Int", strlen("Int"));
														}
													}
	| DOUBLE 										{
														if (contaErros == 0 && valorT == 1){
															$$ = createNode(OTHER_node, "Double", "");
															auxType = (char*)calloc(10,sizeof(char));
															strncpy(auxType, "Double", strlen("Double"));
														}
													}
	;


/*********************************************************************
* Statement → OBRACE { Statement } CBRACE							 *
* Statement → IF OCURV Expr CCURV Statement [ ELSE Statement ]		 *
* Statement → WHILE OCURV Expr CCURV Statement						 *
* Statement → DO Statement WHILE OCURV Expr CCURV SEMI				 *
* Statement → PRINT OCURV ( Expr | STRLIT ) CCURV SEMI				 *
* Statement → [ ( Assignment | MethodInvocation | ParseArgs ) ] SEMI *
* Statement → RETURN [ Expr ] SEMI									 *
*********************************************************************/
Statement: OBRACE StatementList CBRACE								{
																		if (contaErros == 0 && valorT == 1){
																			if($2 != NULL && $2->right != NULL){
																				$$ = createNode(OTHER_node, "Block", "");
																				addChild($$, $2);
																			}else{
																				$$ = $2;
																			}
																		}
																	}
		| IF OCURV Expr CCURV Statement %prec LOWER_THAN_ELSE 		{
																		if (contaErros == 0 && valorT == 1){
																			$$ = createNode(OTHER_node, "If", "");
																			addChild($$,$3);

																			if($5 != NULL && $5->right != NULL){
																				aux = createNode(OTHER_node, "Block", "");
																				addChild($$,aux);
																				addChild(aux,$5);
																			}
																			else{
																				addChild($$,$5);
																			}

																			if($5 == NULL){
																				aux = createNode(OTHER_node, "Block", "");
																				addChild($$,aux);
																			}

																			aux = createNode(OTHER_node, "Block", "");
																			addChild($$,aux);
																		}
																	}
		| IF OCURV Expr CCURV Statement ELSE Statement 				{
																		if (contaErros == 0 && valorT == 1){
																			$$ = createNode(OTHER_node, "If", "");
																			addChild($$,$3);

																			if($5 != NULL && $5->right != NULL){
																				aux = createNode(OTHER_node, "Block", "");
																				addChild($$,aux);
																				addChild(aux,$5);
																			}
																			else{
																				addChild($$,$5);
																			}

																			if($5 == NULL){
																				aux = createNode(OTHER_node, "Block", "");
																				addChild($$,aux);
																			}


																			if($7 != NULL && $7->right != NULL){
																				aux = createNode(OTHER_node, "Block", "");
																				addChild($$,aux);
																				addChild(aux,$7);
																			}
																			else{
																				addChild($$,$7);
																			}

																			if($7 == NULL){
																				aux = createNode(OTHER_node, "Block", "");
																				addChild($$,aux);
																			}

																		}
																	}
		| WHILE OCURV Expr CCURV Statement 							{
																		if (contaErros == 0 && valorT == 1){
																			$$ = createNode(OTHER_node, "While", "");
																			addChild($$,$3);

																			if($5 != NULL && $5->right != NULL){
																				aux = createNode(OTHER_node, "Block", "");
																				addChild($$,aux);
																				addChild(aux,$5);
																			}
																			else{
																				addChild($$,$5);
																			}

																			if($5 == NULL){
																				aux = createNode(OTHER_node, "Block", "");
																				addChild($$,aux);
																			}

																		}
																	}
		| DO Statement WHILE OCURV Expr CCURV SEMI 					{
																		if (contaErros == 0 && valorT == 1){
																			$$ = createNode(OTHER_node, "DoWhile", "");

																			if($2 != NULL && $2->right != NULL){
																				aux = createNode(OTHER_node, "Block", "");
																				addChild($$,aux);
																				addChild(aux,$2);
																			}
																			else{
																				addChild($$,$2);
																			}

																			if($2 == NULL){
																				aux = createNode(OTHER_node, "Block", "");
																				addChild($$,aux);
																			}

																			addChild($$,$5);
																		}
																	}
		| PRINT OCURV Expr CCURV SEMI 								{
																		if (contaErros == 0 && valorT == 1){
																			$$ = createNode(OTHER_node, "Print", "");
																			addChild($$,$3);
																		}
																	}
		| PRINT OCURV STRLIT CCURV SEMI 							{
																		if (contaErros == 0 && valorT == 1){
																			$$ = createNode(OTHER_node, "Print", "");
																			aux = createNode(STRLIT_node, "Strlit", $3);
																			addChild($$,aux);
																		}
																		free($3);
																	}
		| SEMI														{
																		if (contaErros == 0 && valorT == 1){
																			$$ = NULL;
																		}

																	}
		| Assignment SEMI											{
																		if (contaErros == 0 && valorT == 1){
																			$$ = $1;
																		}
																	}
		| MethodInvocation SEMI										{
																		if (contaErros == 0 && valorT == 1){
																			$$ = $1;
																		}
																	}
		| ParseArgs SEMI											{
																		if (contaErros == 0 && valorT == 1){
																			$$ = $1;
																		}
																	}
		| RETURN SEMI												{
																		if (contaErros == 0 && valorT == 1){
																			$$ = createNode(OTHER_node, "Return", "");
																		}
																	}
		| RETURN Expr SEMI											{
																		if (contaErros == 0 && valorT == 1){
																			$$ = createNode(OTHER_node, "Return", "");
																			addChild($$,$2);
																		}
																	}
		| error SEMI												{$$ = NULL; if (contaErros == 0 && valorT == 1) {clearTree(root);}}
		;

StatementList: 								{$$ = NULL;}

		|	Statement StatementList   	{
												if (contaErros == 0 && valorT == 1){
													if($1 != NULL){
														if($2 != NULL){
															addBrother($1, $2);
														}
														$$ = $1;

													}else{
														$$ = $2;
													}
												}
											}
		;


/*********************************************************************
* Assignment → ID ASSIGN Expr										 *
*********************************************************************/
Assignment: ID ASSIGN Expr 									{
																if (contaErros == 0 && valorT == 1){
																	$$ = createNode(OTHER_node, "Assign", "");
																	aux = createNode(ID_node, "Id", $1);
																	addChild($$,aux);
																	addChild($$,$3);
																}
																free($1);
															}


/*********************************************************************
* MethodInvocation → ID OCURV [ Expr { COMMA Expr } ] CCURV 		 *
*********************************************************************/
MethodInvocation: ID OCURV CCURV							{
																if (contaErros == 0 && valorT == 1){
																	$$ = createNode(OTHER_node, "Call", "");
																	aux = createNode(ID_node, "Id", $1);
																	addChild($$,aux);
																}
																free($1);
															}
				| ID OCURV Expr CCURV						{
																if (contaErros == 0 && valorT == 1){
																	$$ = createNode(OTHER_node, "Call", "");
																	aux = createNode(ID_node, "Id", $1);
																	addChild($$,aux);
																	addBrother(aux,$3);
																}
																free($1);
															}
				| ID OCURV Expr CommaExpr CCURV				{
																if (contaErros == 0 && valorT == 1){
																	$$ = createNode(OTHER_node, "Call", "");
																	aux = createNode(ID_node, "Id", $1);
																	addChild($$,aux);
																	addBrother(aux,$3);
																	addBrother(aux,$4);
																}
																free($1);
															}
				| ID OCURV error CCURV						{$$ = NULL; if (contaErros == 0 && valorT == 1) {clearTree(root);}}
				;

CommaExpr: COMMA Expr 									{
															if (contaErros == 0 && valorT == 1){
																$$ = $2;
															}
														}
		| CommaExpr COMMA Expr 							{
															if (contaErros == 0 && valorT == 1){
																$$ = $1;
																addBrother($$,$3);
															}
														}
		;


/*********************************************************************
* ParseArgs → PARSEINT OCURV ID OSQUARE Expr CSQUARE CCURV			 *
*********************************************************************/
ParseArgs: PARSEINT OCURV ID OSQUARE Expr CSQUARE CCURV			{
																	if (contaErros == 0 && valorT == 1){
																		$$ = createNode(OTHER_node, "ParseArgs", "");
																		aux = createNode(ID_node, "Id", $3);
																		addChild($$,aux);
																		addChild($$,$5);
																	}
																	free($3);
																}
		| PARSEINT OCURV error CCURV							{$$ = NULL; if (contaErros == 0 && valorT == 1) {clearTree(root);}}
		;


/*********************************************************************
* Expr → Assignment | MethodInvocation | ParseArgs					 *
* Expr → Expr ( AND | OR ) Expr										 *
* Expr → Expr ( EQ | GEQ | GT | LEQ | LT | NEQ ) Expr				 *
* Expr → Expr ( PLUS | MINUS | STAR | DIV | MOD ) Expr				 *
* Expr → ( PLUS | MINUS | NOT ) Expr								 *
* Expr → ID [ DOTLENGTH ]											 *
* Expr → OCURV Expr CCURV											 *
* Expr → BOOLLIT | DECLIT | REALLIT									 *
*********************************************************************/

Expr: Assignment								{
													if (contaErros == 0 && valorT == 1){
														$$ = $1;
													}
												}
	| ExprAux									{
													if (contaErros == 0 && valorT == 1){
														$$ = $1;
													}
												}
	;

ExprAux: MethodInvocation						{
													if (contaErros == 0 && valorT == 1){
														$$ = $1;
													}
												}
		| ParseArgs								{
													if (contaErros == 0 && valorT == 1){
														$$ = $1;
													}
												}
		| ExprAux AND ExprAux 					{
													if (contaErros == 0 && valorT == 1){
														$$ = createNode(OTHER_node, "And", "");
														addChild($$,$1);
														addChild($$,$3);
													}
												}
		| ExprAux OR ExprAux 					{
													if (contaErros == 0 && valorT == 1){
														$$ = createNode(OTHER_node, "Or", "");
														addChild($$,$1);
														addChild($$,$3);
													}
												}
		| ExprAux EQ ExprAux 					{
													if (contaErros == 0 && valorT == 1){
														$$ = createNode(OTHER_node, "Eq", "");
														addChild($$,$1);
														addChild($$,$3);
													}
												}
		| ExprAux GEQ ExprAux 					{
													if (contaErros == 0 && valorT == 1){
														$$ = createNode(OTHER_node, "Geq", "");
														addChild($$,$1);
														addChild($$,$3);
													}
												}
		| ExprAux GT ExprAux					{
													if (contaErros == 0 && valorT == 1){
														$$ = createNode(OTHER_node, "Gt", "");
														addChild($$,$1);
														addChild($$,$3);
													}
												}
		| ExprAux LEQ ExprAux 					{
													if (contaErros == 0 && valorT == 1){
														$$ = createNode(OTHER_node, "Leq", "");
														addChild($$,$1);
														addChild($$,$3);
													}
												}
		| ExprAux LT ExprAux 					{
													if (contaErros == 0 && valorT == 1){
														$$ = createNode(OTHER_node, "Lt", "");
														addChild($$,$1);
														addChild($$,$3);
													}
												}
		| ExprAux NEQ ExprAux 					{
													if (contaErros == 0 && valorT == 1){
														$$ = createNode(OTHER_node, "Neq", "");
														addChild($$,$1);
														addChild($$,$3);
													}
												}
		| ExprAux PLUS ExprAux 					{
													if (contaErros == 0 && valorT == 1){
														$$ = createNode(OTHER_node, "Add", "");
														addChild($$,$1);
														addChild($$,$3);
													}
												}
		| ExprAux MINUS ExprAux 				{
													if (contaErros == 0 && valorT == 1){
														$$ = createNode(OTHER_node, "Sub", "");
														addChild($$,$1);
														addChild($$,$3);
													}
												}
		| ExprAux STAR ExprAux 					{
													if (contaErros == 0 && valorT == 1){
														$$ = createNode(OTHER_node, "Mul", "");
														addChild($$,$1);
														addChild($$,$3);
													}
												}
		| ExprAux DIV ExprAux					{
													if (contaErros == 0 && valorT == 1){
														$$ = createNode(OTHER_node, "Div", "");
														addChild($$,$1);
														addChild($$,$3);
													}
												}
		| ExprAux MOD ExprAux					{
													if (contaErros == 0 && valorT == 1){
														$$ = createNode(OTHER_node, "Mod", "");
														addChild($$,$1);
														addChild($$,$3);
													}
												}
		| PLUS ExprAux %prec PRECEDENCE 		{
													if (contaErros == 0 && valorT == 1){
														$$ = createNode(OTHER_node, "Plus","");
														addChild($$, $2);
													}
												}
		| MINUS ExprAux %prec PRECEDENCE 		{
													if (contaErros == 0 && valorT == 1){
														$$ = createNode(OTHER_node, "Minus","");
														addChild($$, $2);
													}
												}
		| NOT ExprAux %prec PRECEDENCE 			{
													if (contaErros == 0 && valorT == 1){
														$$ = createNode(OTHER_node, "Not","");
														addChild($$, $2);
													}
												}
		| ID 									{
													if (contaErros == 0 && valorT == 1){
														$$ = createNode(ID_node, "Id",$1);
													}
													free($1);
												}
		| ID DOTLENGTH 							{
													if (contaErros == 0 && valorT == 1){
														$$ = createNode(OTHER_node, "Length", "");
														aux = createNode(ID_node, "Id",$1);
														addChild($$,aux);
													}
													free($1);
												}
		| OCURV Expr CCURV 						{
													if (contaErros == 0 && valorT == 1){
														$$ = $2;
													}
												}
		| BOOLLIT 								{
													if (contaErros == 0 && valorT == 1){
														$$ = createNode(BOOLLIT_node, "BoolLit",$1);
													}
													free ($1);
												}
		| DECLIT 								{
													if (contaErros == 0 && valorT == 1){
														$$ = createNode(DECLIT_node, "DecLit",$1);
													}
													free ($1);
												}
		| REALLIT								{
													if (contaErros == 0 && valorT == 1){
														$$ = createNode(REALLIT_node, "RealLit",$1);
													}
													free ($1);
												}
		| OCURV error CCURV						{$$ = NULL; if (contaErros == 0 && valorT == 1) {clearTree(root);}}
		;

%%

/* Função de erros */
int yyerror(const char *s){
	printf("Line %lld, col %lld: %s: %s\n", contaLinha, contaColuna-strlen(yytext), s, yytext);
	contaErros++;
    return 0;
}

int main(int argc, char *argv[])
{
    if(argv[1] != NULL){
        /*flag de compilação*/
        if(strcmp(argv[1], "-1") == 0) {
            valor1 = 1;
            yylex();
        }
        else if(strcmp(argv[1], "-l") == 0) {
            valorL=1;
            yylex();
        }
        else if(strcmp(argv[1], "-t") == 0){
        	valorT = 1;
        	yyparse();
        }
        else if(strcmp(argv[1], "-2") == 0){
        	valorT = 1;
            yyparse();
        }
        else if(strcmp(argv[1], "-s") == 0){
        	valorT = 1;
        	valorS = 1;
        	yyparse();
        }
    }
    else if(argv[1] == NULL){
            valorNull = 1;
            yyparse();
    }

    /* Imprime as tabelas */
    if(valorS && contaErros==0){
        tab = initTables(root);
        printTables(tab);
        printf("\n");
    }

    if(contaErros == 0 && valorT == 1){
    	printTree(root, 0);
    }
    clearTree(root);
    free(auxType);

    return 0;
}


