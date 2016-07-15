#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "ast.h"

SymEntryPtr symtable = NULL;
SymEntryPtr curfunc = NULL;

//return value is pointer of abstract syntax tree.
ExprNodePtr makeExpr(OpSort opr, int value, SymEntryPtr symbol,
	ExprNodePtr left, ExprNodePtr right){
	ExprNodePtr enp;
	if((enp = (ExprNode *)malloc(sizeof(ExprNode))) == NULL){
		fprintf(stderr, "fail to allocate the memorry space.--makeExpr.\n");
		exit(1);
	}else{
		enp->op = opr;
		enp->val = value;
		enp->sym = symbol;
		enp->sub1 = left;
		enp->sub2 = right;
	}
	return enp;
}

StmtNodePtr makeStmt(StmtSort sort, ExprNodePtr expr,
	StmtNodePtr st1, StmtNodePtr st2){
	StmtNodePtr snp;
	if((snp = (StmtNode *)malloc(sizeof(StmtNode))) == NULL){
		fprintf(stderr, "fail to allocate the memorry space.--makeStmt.\n");
		exit(1);
	}else{
		snp->sort = sort;
		snp->next = NULL;
		snp->expr = expr;
		snp->st1 = st1;
		snp->st2 = st2;
	}
	return snp;
}

DefNodePtr makeDef(DefSort sort, SymEntryPtr sym, StmtNodePtr body){
	DefNodePtr dnp;
	if ((dnp = (DefNode *)malloc(sizeof(DefNode))) == NULL)	{
		fprintf(stderr, "fail to allocate the memorry space.--makeDef\n");
		exit(1);
	}else{
		dnp->sort = sort;
		dnp->sym = sym;
		dnp->body = body;
		dnp->next = NULL;
	}
	return dnp;
}

//Functions that related to Symbol Entry.
SymEntryPtr symAdd(SymbolSort sort, char *name, int no, int nParam,
	int nVar, SymEntryPtr belong){
//	fprintf(stdout, "aaaaaaaa\n");
	SymEntryPtr sep;
	if ((sep = (SymEntry *)malloc(sizeof(SymEntry))) == NULL)	{
		fprintf(stderr, "fail to allocate the memorry space.--symAdd\n");
		exit(1);
	}else{
		//fprintf(stdout, "bbbbbb\n");
		sep->sort = sort;
		sep->name = name;
		sep->no = no;
		sep->nParam = nParam;
		sep->nVar = nVar;
		sep->belong = belong;
		sep->prev = symtable;
		symtable = sep;
		// if (sort==SYM_VAR && belong != NULL)
			// belong->nVar = no;
	}
	return sep;
}

void paramDel(){
	SymEntryPtr pursue = NULL;
	SymEntryPtr follow = symtable;
	if(follow != NULL){
		pursue = follow->prev;
	//this loop continue while symtable's belong is equals to curfunc.
		while(follow != NULL && follow->belong == curfunc && follow->sort == SYM_PARAM){
			SymEntryPtr tmp = follow;
			pursue = pursue->prev;
			follow = follow->prev;
			symtable = follow;
			free(tmp);
		}
		while(pursue != NULL){
			if(pursue->belong == curfunc && pursue->sort == SYM_PARAM){
				follow->prev = pursue->prev;
				free(pursue);
				pursue = follow->prev;
			}else{
				pursue = pursue->prev;
				follow = follow->prev;
			}
		}
	}
}

SymEntryPtr symLookup(char *name){
	SymEntryPtr pursue = symtable;
	while(pursue != NULL){
		if (pursue->belong == curfunc || pursue->belong == NULL /*|| pursue->sort == SYM_FUNC*/){
			if (strcmp(pursue->name,name) == 0)
				return pursue;
		}
		pursue = pursue->prev;

	}
	return NULL;
}