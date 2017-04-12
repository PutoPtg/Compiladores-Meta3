/***********************************************************************************
*Ficheiro "jac.l"
*Analisador Lexical da linguagem Ja - Sub-conjunto de Java
*Cadeira de Compiladores - 2017 - Licenciatura em Engenharia Informática
*Manuel Madeira Amado - 2006131282
*Xavier Silva - 2013153577
*Versão 2.0 -erro de certeza - 1.11 praí?
************************************************************************************/

%X COMMENT
%X JAVACOMMENT
%X STR

%{
	#include <string.h>
	#include <stdlib.h>
	#include <ctype.h>
	#include <string.h>
	#include <stdio.h>
	#include "y.tab.h"
	#include "tree.h"
	void yyerror(const char *s);

	extern int valorL;
	extern int valor1;
	extern int valorNull;
	extern int valorT;
	long long int contaLinha = 1;
	long long int contaColuna = 1;
	long long int commentPos[2];
	long long int stringPos[2];


	char* auxStr;
%}

digito		[0-9]|[0-9]+[0-9_]*[0-9]+
reserved	"++"|"--"|Integer|System|abstract|continue|for|new|switch|assert|default|package|synchronized|goto|private|this|break|implements|protected|throw|byte|import|throws|case|enum|instanceof|transient|catch|extends|short|try|char|final|interface|finally|long|strictfp|volatile|const|float|native|super|null
seqEscape	\\n|\\t|\\r|\\\\|\\\"|\\f
seqEscInv	"\\."
expoente 	("e"|"E")("+"|"-")?{digito}+


%%

"/*"								{BEGIN COMMENT;  commentPos[0] = contaLinha; commentPos[1] = contaColuna; contaColuna+=yyleng;}
<COMMENT><<EOF>>					{printf("Line %lld, col %lld: unterminated comment\n", commentPos[0], commentPos[1]); contaColuna+=yyleng-1; BEGIN 0; } /*erro de unterminated comment*/
<COMMENT>"*/"						{contaColuna+=yyleng;BEGIN 0;} /*Comentário*/
<COMMENT>\n|\r|\r\n 				{contaLinha++;contaColuna=1;}	/*incrementa linha*/
<COMMENT>.							{contaColuna+=yyleng;}	/*incrementa coluna*/

"//"								{BEGIN JAVACOMMENT; contaColuna+=yyleng;}
<JAVACOMMENT>\n|\r|\r\n 			{contaLinha++;contaColuna=1;BEGIN 0;}
<JAVACOMMENT>.						{contaColuna+=yyleng;}

\"([^\n\r\"\\]|{seqEscape})*\"		{if(valorL==1){printf("STRLIT(%s)\n", yytext);} contaColuna+=yyleng; if(valorNull==1 || valorT==1){yylval.val = (char*)calloc(1+yyleng,sizeof(char)); strncpy(yylval.val,yytext,strlen(yytext));return STRLIT;}}

"\""								{BEGIN STR;  stringPos[0] = contaLinha; stringPos[1] = contaColuna; contaColuna+=yyleng;}
<STR>{seqEscape}					{contaColuna+=yyleng;}
<STR>\\.|\\							{printf("Line %lld, col %lld: invalid escape sequence (%s)\n",contaLinha,contaColuna,yytext); contaColuna+=yyleng;}
<STR><<EOF>>						{contaColuna+=yyleng; printf("Line %lld, col %lld: unterminated string literal\n",stringPos[0],stringPos[1]); BEGIN 0;}
<STR>"\""							{contaColuna+=yyleng; BEGIN 0;}
<STR>\n|\r|\r\n 					{contaLinha++; contaColuna=1; printf("Line %lld, col %lld: unterminated string literal\n",stringPos[0],stringPos[1]); BEGIN 0;}
<STR>.								{contaColuna+=yyleng;}

{digito}"."{digito}{expoente}		{if(valorL==1){printf("REALLIT(%s)\n", yytext);}  contaColuna+=yyleng; if(valorNull==1 || valorT==1){ yylval.val = (char*)calloc(1+yyleng,sizeof(char)); strncpy(yylval.val,yytext,yyleng); return REALLIT;}}
{digito}"."({digito}|{expoente})?	{if(valorL==1){printf("REALLIT(%s)\n", yytext);}  contaColuna+=yyleng; if(valorNull==1 || valorT==1){ yylval.val = (char*)calloc(1+yyleng,sizeof(char)); strncpy(yylval.val,yytext,yyleng); return REALLIT;}}
"."{digito}{expoente}?				{if(valorL==1){printf("REALLIT(%s)\n", yytext);}  contaColuna+=yyleng; if(valorNull==1 || valorT==1){ yylval.val = (char*)calloc(1+yyleng,sizeof(char)); strncpy(yylval.val,yytext,yyleng); return REALLIT;}}
{digito}{expoente}					{if(valorL==1){printf("REALLIT(%s)\n", yytext);}  contaColuna+=yyleng; if(valorNull==1 || valorT==1){ yylval.val = (char*)calloc(1+yyleng,sizeof(char)); strncpy(yylval.val,yytext,yyleng); return REALLIT;}}

0|[1-9]+|[1-9]+[0-9_]*[0-9]+		{if(valorL==1){printf("DECLIT(%s)\n", yytext);}  contaColuna+=yyleng; if(valorNull==1 || valorT==1){ yylval.val = (char*)calloc(1+yyleng,sizeof(char)); strncpy(yylval.val,yytext,yyleng); return DECLIT;}}

{reserved}							{if(valorL==1){printf("RESERVED(%s)\n", yytext);}  contaColuna+=yyleng; if(valorNull==1 || valorT==1){return RESERVED;}}
"boolean"							{if(valorL==1){printf("BOOL\n");}  contaColuna+=yyleng; if(valorNull==1 || valorT==1){return BOOL;}}
"true"|"false"						{if(valorL==1){printf("BOOLLIT(%s)\n",yytext);}  contaColuna+=yyleng; if(valorNull==1 || valorT==1){ yylval.val = (char*)calloc(1+yyleng,sizeof(char)); strncpy(yylval.val,yytext,yyleng); return BOOLLIT;}}
"class"								{if(valorL==1){printf("CLASS\n");}  contaColuna+=yyleng; if(valorNull==1 || valorT==1){return CLASS;}}
"do"								{if(valorL==1){printf("DO\n");}  contaColuna+=yyleng; if(valorNull==1 || valorT==1){return DO;}}
".length"							{if(valorL==1){printf("DOTLENGTH\n");}  contaColuna+=yyleng; if(valorNull==1 || valorT==1){return DOTLENGTH;}}
"double"							{if(valorL==1){printf("DOUBLE\n");}  contaColuna+=yyleng; if(valorNull==1 || valorT==1){return DOUBLE;}}
"else"								{if(valorL==1){printf("ELSE\n");}  contaColuna+=yyleng; if(valorNull==1 || valorT==1){ return ELSE;}}
"if"								{if(valorL==1){printf("IF\n");}  contaColuna+=yyleng; if(valorNull==1 || valorT==1){ return IF;}}
"int"								{if(valorL==1){printf("INT\n");}  contaColuna+=yyleng; if(valorNull==1 || valorT==1){return INT;}}
"Integer.parseInt"					{if(valorL==1){printf("PARSEINT\n");}  contaColuna+=yyleng; if(valorNull==1 || valorT==1){ return PARSEINT;}}
"System.out.println"				{if(valorL==1){printf("PRINT\n");}  contaColuna+=yyleng; if(valorNull==1 || valorT==1){ return PRINT;}}
"public"							{if(valorL==1){printf("PUBLIC\n");}  contaColuna+=yyleng; if(valorNull==1 || valorT==1){ return PUBLIC;}}
"return"							{if(valorL==1){printf("RETURN\n");}  contaColuna+=yyleng; if(valorNull==1 || valorT==1){ return RETURN;}}
"static"							{if(valorL==1){printf("STATIC\n");}  contaColuna+=yyleng; if(valorNull==1 || valorT==1){ return STATIC;}}
"String"							{if(valorL==1){printf("STRING\n");}  contaColuna+=yyleng; if(valorNull==1 || valorT==1){ return STRING;}}
"void"								{if(valorL==1){printf("VOID\n");}  contaColuna+=yyleng; if(valorNull==1 || valorT==1){ return VOID;}}
"while"								{if(valorL==1){printf("WHILE\n");}  contaColuna+=yyleng; if(valorNull==1 || valorT==1){ return WHILE;}}
"("									{if(valorL==1){printf("OCURV\n");}  contaColuna+=yyleng; if(valorNull==1 || valorT==1){ return OCURV;}}
")"									{if(valorL==1){printf("CCURV\n");}  contaColuna+=yyleng; if(valorNull==1 || valorT==1){ return CCURV;}}
"{"									{if(valorL==1){printf("OBRACE\n");}  contaColuna+=yyleng; if(valorNull==1 || valorT==1){return OBRACE;}}
"}"									{if(valorL==1){printf("CBRACE\n");}  contaColuna+=yyleng; if(valorNull==1 || valorT==1){ return CBRACE;}}
"["									{if(valorL==1){printf("OSQUARE\n");}  contaColuna+=yyleng; if(valorNull==1 || valorT==1){ return OSQUARE;}}
"]"									{if(valorL==1){printf("CSQUARE\n");}  contaColuna+=yyleng; if(valorNull==1 || valorT==1){ return CSQUARE;}}
"&&"								{if(valorL==1){printf("AND\n");}  contaColuna+=yyleng; if(valorNull==1 || valorT==1){ return AND;}}
"||"								{if(valorL==1){printf("OR\n");}  contaColuna+=yyleng; if(valorNull==1 || valorT==1){ return OR;}}
"<"									{if(valorL==1){printf("LT\n");}  contaColuna+=yyleng; if(valorNull==1 || valorT==1){ return LT;}}
">"									{if(valorL==1){printf("GT\n");}  contaColuna+=yyleng; if(valorNull==1 || valorT==1){ return GT;}}
"=="								{if(valorL==1){printf("EQ\n");}  contaColuna+=yyleng; if(valorNull==1 || valorT==1){ return EQ;}}
"!="								{if(valorL==1){printf("NEQ\n");}  contaColuna+=yyleng; if(valorNull==1 || valorT==1){ return NEQ;}}
"<="								{if(valorL==1){printf("LEQ\n");}  contaColuna+=yyleng; if(valorNull==1 || valorT==1){ return LEQ;}}
">="								{if(valorL==1){printf("GEQ\n");}  contaColuna+=yyleng; if(valorNull==1 || valorT==1){ return GEQ;}}
"+"									{if(valorL==1){printf("PLUS\n");}  contaColuna+=yyleng; if(valorNull==1 || valorT==1){ return PLUS;}}
"-"									{if(valorL==1){printf("MINUS\n");}  contaColuna+=yyleng; if(valorNull==1 || valorT==1){ return MINUS;}}
"*"									{if(valorL==1){printf("STAR\n");}  contaColuna+=yyleng; if(valorNull==1 || valorT==1){ return STAR;}}
"/"									{if(valorL==1){printf("DIV\n");}  contaColuna+=yyleng; if(valorNull==1 || valorT==1){ return DIV;}}
"%"									{if(valorL==1){printf("MOD\n");}  contaColuna+=yyleng; if(valorNull==1 || valorT==1){ return MOD;}}
"!"									{if(valorL==1){printf("NOT\n");}  contaColuna+=yyleng; if(valorNull==1 || valorT==1){ return NOT;}}
"="									{if(valorL==1){printf("ASSIGN\n");}  contaColuna+=yyleng; if(valorNull==1 || valorT==1){ return ASSIGN;}}
";"									{if(valorL==1){printf("SEMI\n");}  contaColuna+=yyleng; if(valorNull==1 || valorT==1){ return SEMI;}}
","									{if(valorL==1){printf("COMMA\n");}  contaColuna+=yyleng; if(valorNull==1 || valorT==1){ return COMMA;}}
[a-zA-Z_$][a-zA-Z0-9_$]*			{if(valorL==1){printf("ID(%s)\n", yytext);}  contaColuna+=yyleng; if(valorNull==1 || valorT==1){ yylval.val = (char*)calloc(1+yyleng,sizeof(char)); strncpy(yylval.val,yytext,yyleng); return ID;}}
"\n"								{contaLinha++; contaColuna=1; }
"\r"								{contaLinha++;contaColuna=1; }
"\r\n"								{contaLinha++;contaColuna=1; }
" "									{contaColuna+=1; }
"\t"								{contaColuna+=1; }
"\f"								{contaColuna+=1; }
.									{ contaColuna++; printf("Line %lld, col %lld: illegal character (%s)\n",contaLinha,contaColuna-yyleng,yytext); }


%%


int yywrap()
{
return 1;
}


