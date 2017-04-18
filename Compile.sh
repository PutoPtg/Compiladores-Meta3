lex jac.l
yacc -d -v jac.y
gcc -g -o jac y.tab.c lex.yy.c tree.c symbols.c -ll -ly
./jac -s < TestFiles/gcd2.ja
