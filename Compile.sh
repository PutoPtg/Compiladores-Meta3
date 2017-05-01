# usar a variável seginte para colocar um ficheiro em default
in=TestFiles/3-Call-varivavel.java #default de entrada
out=out.txt #default de saída

#Script de Compilação
lex jac.l
yacc -d -v jac.y
gcc -g -o jac y.tab.c lex.yy.c tree.c symbols.c -ll -ly

tf=${1-$in} #Caso tenha um ficheiro a seguir executa esse, senão usa o default

./jac -s < $tf 2>&1 | tee $out #escreve no stdout e no ficheiro definido em out
