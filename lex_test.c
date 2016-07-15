#include <stdio.h>
#include <stdlib.h>
#include "parser.tab.h"
FILE *yyin;

int main(int argc, char* argv[]){
	int tok;
	if(argc == 2){
		yyin = fopen(argv[1], "r+");
		if (yyin == NULL){
			printf("fail to open file.\n");
			exit(1);
		}
	}else{
		if (yyin == NULL){
			printf("error: no input files.\n");
			exit(1);
		}
	}
	while(tok = yylex()){
		if(tok < 128){
			printf("CHAR: %c\n", tok);
		}else{
			switch(tok){
				case NUM:    printf("NUMBER: %d\n", yylval.ival); break;
				case ID:     printf("ID: %s\n", yylval.name);     break;
				case IF:     printf("IF\n");     break;
				case ELSE:   printf("ELSE\n");   break;
				case WHILE:  printf("WHILE\n");  break;
				case RETURN: printf("RETURN\n"); break;
				case TINT:   printf("TINT\n");   break;
				case BEQ:    printf("BEQ\n");    break;
				case BLT:    printf("BLT\n");    break;
				case BNE:    printf("BNE\n");    break;
				case LOR:    printf("LOR\n");    break;
				case LAND:   printf("LAND\n");   break;
				case LSL:    printf("LSL\n");    break;
				case LSR:    printf("LSR\n");    break;
				case INC:    printf("INC\n");    break;
				case DEC:    printf("DEC\n");    break;
				case ASSIGN: printf("ASSIGN\n"); break;
		        case ADD:    printf("ADD\n");    break;
		        case SUB:    printf("SUB\n");    break;
		        case MUL:    printf("MUL\n");    break;
		        case DIV:    printf("DIV\n");    break;
		        case MOD:    printf("MOD\n");    break;
		        case NOT:    printf("NOT\n");    break;
		        case OR:     printf("OR\n");     break;
		        case AND:    printf("AND\n");    break;
		        case EOR:    printf("EOR\n");    break;
		        case CMP:    printf("CMP\n");    break;
				default:     printf("*** %d : unknown ***\n", tok);
			}
		}
	}
	return 0;
}