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
extern FILE *af;
FILE *yyin;

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
	//int arg[argc];
	char* filename = "a.out";
	char* openFile = NULL;

	if (argc > 1){
		openFile = malloc(strlen(argv[1])+1);
		strcpy(openFile,argv[1]);
		filename = strtok(argv[1], ".");
	}
	
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
				  exit(1);
				  break;
				case _W:
				  //codes
				  printf("option -w is not yet defined.\n");
				  exit(1);
				  break;
				default:
				;
				  //codes
			}
		}
	}

    // find out source file. -----------
    /*
    int place = 1; //source file's index.
    int count = 0; //option and option's argument count with arg_num[]. if argc - total of arg_num[] != 1, it would error. 
	for(i = 0; i < argc; i++){
		if(arg[i] == 0)
			place = i;
		else
			count++;
	}
	if(argc >2)
		if(argc - count == 1){
		//number of source file postulate one.
			openFile = argv[place];
		}else{
			printf("error: argument.\n");
			exit(1);
		}
	*/
	for(i = 0; i < argc; i++)
	if(strstr(argv[i],".c ") != NULL || strstr(argv[i],".c\n") != NULL)	{
		openFile = argv[i];
	}
	//---------------------------------
	if(openFile != NULL){
		yyin = fopen(openFile, "r+");
		if (yyin == NULL){
			printf("fail to open file.\n");
			exit(1);
		}
	}else{
			printf("error: no input files.\n");
			exit(1);
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