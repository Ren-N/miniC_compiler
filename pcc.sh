cc lex.yy.c parser.tab.c test5.c gen.c ast.c -ll -o pcc
# bison -d parser.y
# or
# yacc -dy parser.y
# and
# lex lexer.l

# add '#include "ast.h"' to parser.tab.h generated by bison with option '-d'.