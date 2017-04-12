lex jac.l
yacc -d -v jac.y
gcc -g -o jac y.tab.c lex.yy.c tree.c -ll -ly
./jac -2 <TestFiles/test1.txt 
