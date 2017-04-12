lex jac.l
yacc -d -v jac.y
gcc -g -o jac y.tab.c lex.yy.c tree.c -ll -ly
./jac -t <TestFiles/test1.txt >out.txt
