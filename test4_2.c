#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "ast.h"
#include "gen.h"
//CONSTANT_OPTION
#define _O 0
#define _C 1
#define _W 2

extern DefNodePtr sourcedefs;
FILE *af;

int optionCheck(char* option){
	if (!strcmp(option,"-o")){
		return _O;
	}else if(!strcmp(option, "-c")){
		return _C;
	}else if(!strcmp(option, "-w")){
		return _W;
	}
	return -1;
}

int main(int argc, char* argv[]){
	int i;
	char* filename = "a.out";
	if (argc > 1)
	  filename = strtok(argv[1], ".");
	
	for (i = 0; i < argc; i++){
		if (*argv[i] == '-'){
			int sort = optionCheck(argv[i]);
			switch(sort){
				case _O:
				  if (i < argc-1)
				  	filename  = argv[(i+1)%argc];
				  break;
				case _C:
				  //codes
				  printf("option -c is not yet defined.\n");
				  break;
				case _W:
				  //codes
				  printf("option -w is not yet defined.\n");
				  break;
				default:
				;
				  //codes
			}
		}
	}

	af = fopen(filename, "w+");
	if (af == NULL){
		printf("fail to open file.\n");
		exit(1);
	}
	//Compile=================
	fprintf(stdout, "PARSE=====================\n");
	yyparse();
	fprintf(stdout, "GENERATE CODE=============\n");
	genCode();
	//========================
	fclose(af);
	return 0;
}