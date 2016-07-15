%{
#include <stdio.h>
#include <string.h>
#include "gen.h"
#include "ast.h"

StmtNodePtr setStmt(StmtNodePtr list, StmtNodePtr stmt);
StmtNodePtr getStmtBottom(StmtNodePtr stmt);
ExprNodePtr setAlist(ExprNodePtr list, ExprNodePtr expr);
void reset(char* id);
SymEntryPtr addGvar(char* id);
void addParam(char* id);
void addLocal(char* id);
void checkParam();

extern int yydebug;
extern SymEntryPtr curfunc;
extern DefNodePtr sourcedefs;
//extern SymEntryPtr symtable;
FILE *af;
int cntGlobal = 0;
int cntParam = 0;
int cntLocal = 0;
int hasProto = 0;
%}


%union{
	int  ival;
	char *name;
	ExprNodePtr expr, alist;
	StmtNodePtr stmt;
	DefNodePtr def;
	SymEntryPtr sym;
}

%token  <ival> NUM
%token  <name> ID
%token IF ELSE WHILE RETURN
%token TINT

%type <expr>  expr
%type <alist> alist alist_l
%type <stmt>  st_list stmt if_part
%type <sym>   decl_gvar
%type <def>   program global_def func_def

%right ASSIGN
%left LOR
%left LAND
%left OR
%left EOR
%left AND
%left BLT BEQ BNE
%left LSL LSR
%left ADD SUB
%left MUL DIV MOD
%left FUNCALL
%right NOT UM CMP
%right DEC INC



%%
//===========================================================================

program
: global_def         { sourcedefs = $1; $$ = $1; }
| program global_def { $1->next = $2; $$ = $2; }
;

global_def
: decl_gvar ';' { 
  fprintf(stdout, "make GlovalVariable:%s\n",$1->name); 
  $$ = makeDef(DEF_GLOBAL,$1,NULL); 
 }
| f_head ';'  {
  if(hasProto == 1){ /* ERROR */ 
    fprintf(stderr,"already exists prototype.\n"); 
  }else{ 
    fprintf(stdout,"Add prototype\n"); 
    paramDel(); 
    $$ = makeDef(PROTOTYPE,curfunc,NULL);
  }  
  }
| func_def  /* implicit */ {
  fprintf(stdout,"func_def is reduced to gloval_def.\n"); /*$$=$1;*/
  }
;

decl_gvar
: TINT ID  { $$ = addGvar($2); }
;

func_def
: f_head '{' {
  fprintf(stdout,"func: %s\n",curfunc->name); 
  fprintf(stdout,"+++LocalVarList+++\n");
 } decl_lvar_list {fprintf(stdout, "+++stmtList+++++++\n"); } st_list {fprintf(stdout, "+++++++++++\n"); } '}'  { curfunc->nVar = cntLocal; $$ = makeDef(DEF_FUNC,curfunc,$6); fprintf(stdout,"func_end\n");}
;

f_head
: TINT ID '(' { reset($2); } p_list ')'  { checkParam(); }

p_list
: /* Empty */
| p_list_l
;

p_list_l
: decl_par
| p_list_l ',' decl_par
;

decl_par
: TINT ID  { addParam($2); }
;

decl_lvar_list
: /* Empty */
| decl_lvar_list decl_lvar ';'
;

decl_lvar
: TINT ID  { addLocal($2); }
;



st_list
: /* Empty */  { $$ = NULL; }
| st_list stmt { 
  if(0){ StmtNodePtr st = getStmtBottom($1); 
    st->next = $2; 
    $$ = $1; }else{ 
    $$ = setStmt($1, $2);
  }
 }
;

stmt
: '{' st_list '}'          { $$ = $2; }
| expr ';'                 { $$ = makeStmt(STMT_EXPR, $1, NULL, NULL); }
| if_part                 
| if_part ELSE {fprintf(stdout, "IF_ELSE\n");} stmt {fprintf(stdout, "IF_ELSE_END\n");}        { StmtNodePtr if_p = $1; if_p->st2 = $4; $$ = if_p;}
| WHILE {fprintf(stdout,"WHILE\n");} '(' expr ')' stmt  { fprintf(stdout,"WHILE_END\n"); $$ = makeStmt(STMT_WHILE, $4, $6, NULL);}
| RETURN {fprintf(stdout,"RETURN*****\n");} expr ';' { fprintf(stdout,"***********\n"); $$ = makeStmt(STMT_RETURN, $3, NULL, NULL); }
;

if_part
: IF {fprintf(stdout,"IF\n");} '(' expr ')' stmt  { $$ = makeStmt(STMT_IF, $4, $6, NULL);fprintf(stdout,"IF_END\n");}
;



expr
: NUM            { $$ = makeExpr(OP_CONST, $1, NULL, NULL, NULL); fprintf(stdout," NUM[%d]\n",$1);}
| expr LOR {fprintf(stdout," OR\n");} expr  { $$ = makeExpr(OP_LOR,0,NULL,$1,$4);}
| expr LAND {fprintf(stdout," AND\n");} expr { $$ = makeExpr(OP_LAND,0,NULL,$1,$4);}
| expr BLT {fprintf(stdout," BLT\n");} expr  { $$ = makeExpr(OP_BLT,0,NULL,$1,$4);}
| expr BEQ {fprintf(stdout," BEQ\n");} expr  { $$ = makeExpr(OP_BEQ,0,NULL,$1,$4);}
| expr BNE {fprintf(stdout," BNE\n");} expr  { $$ = makeExpr(OP_BNE,0,NULL,$1,$4);}
| expr ADD {fprintf(stdout," ADD\n");} expr  { $$ = makeExpr(OP_ADD,0,NULL,$1,$4);}
| expr SUB {fprintf(stdout," SUB\n");} expr  { $$ = makeExpr(OP_SUB,0,NULL,$1,$4);}
| expr MUL {fprintf(stdout," MUL\n");} expr  { $$ = makeExpr(OP_MUL,0,NULL,$1,$4);}
| expr DIV {fprintf(stdout," DIV\n");} expr  { $$ = makeExpr(OP_DIV,0,NULL,$1,$4);}
| expr MOD {fprintf(stdout," MOD\n");} expr  { $$ = makeExpr(OP_MOD,0,NULL,$1,$4);}
| expr AND {fprintf(stdout," AND\n");} expr  { $$ = makeExpr(OP_AND,0,NULL,$1,$4);}
| expr OR  {fprintf(stdout," OR\n");}  expr  { $$ = makeExpr(OP_OR,0,NULL,$1,$4);}
| expr EOR {fprintf(stdout," EOR\n");} expr  { $$ = makeExpr(OP_EOR,0,NULL,$1,$4);}
| expr CMP {fprintf(stdout," CMP\n");} expr  { $$ = makeExpr(OP_CMP,0,NULL,$1,$4);}
| expr LSL {fprintf(stdout," LSL\n");} expr  { $$ = makeExpr(OP_LSL,0,NULL,$1,$4);}
| expr LSR {fprintf(stdout," LSR\n");} expr  { $$ = makeExpr(OP_LSR,0,NULL,$1,$4);}
| NOT {fprintf(stdout," NOT\n");} expr       { $$ = makeExpr(OP_NOT,0,NULL,$3,NULL);}
| INC {fprintf(stdout," INC\n");} expr       { $$ = makeExpr(OP_INC,0,NULL,$3,NULL);}
| DEC {fprintf(stdout," DEC\n");} expr       { $$ = makeExpr(OP_DEC,0,NULL,$3,NULL);}
| SUB {fprintf(stdout," UM\n");} expr %prec UM { $$ = makeExpr(OP_UM,0,NULL,$3,NULL);}
| '(' expr ')'   { $$ = $2;}
| ID ASSIGN {fprintf(stdout," ID:%s\n ASSIGN\n",$1);} expr { $$ = makeExpr(OP_ASSIGN,0,symLookup($1),$4,NULL); fprintf(stdout,"ASSIGN_END\n");}
| ID  { $$ = makeExpr(OP_VAR, 0,symLookup($1),NULL,NULL);fprintf(stdout," ID:%s\n",$1);}
| ID '(' {fprintf(stdout,"FUNCALL:%s #######\n",$1);} alist ')' { $$ = makeExpr(OP_FUNCALL,0,symLookup($1),$4,NULL); fprintf(stdout,"################\n");}
;



alist
: /* Empty */ { $$ = NULL; }
| alist_l
;

alist_l
: expr            {$$ = makeExpr(OP_ALIST, 0, NULL, $1,NULL);}
| alist_l ',' expr  {$$ = setAlist($1,$3);}
;

%%
//==========================================================================

void addParam(char* id){
	fprintf(stdout,"addParam:%s\n",id);
	if(symLookup(id) == NULL){
	  symAdd(SYM_PARAM, id, ++cntParam, 0, 0, curfunc);
	}else{
	  /* ERROR */
	  fprintf(stderr,"%s that is parameter is overloaded.",id);
	  //exit(1);
	}
}

void addLocal(char *id){
	fprintf(stdout,"addLocal:%s\n",id);
	if(symLookup(id) == NULL){
	  symAdd(SYM_VAR,id,++cntLocal,0,0,curfunc);
	}else{
	  /* ERROR */
	  fprintf(stderr,"%s that is local variable is overloaded.",id);
	  //exit(1);
	}
}

void checkParam(){
	fprintf(stdout,"checkParam()\n");
	if(hasProto){
	  if(curfunc->nParam != cntParam){
	    /* ERROR */
	    fprintf(stderr,"Prototype's parameter isn't equals to parameter of defined function.");
	    //exit(1);
	  }
	}else{
	  curfunc->nParam = cntParam;
	}
}

void reset(char* id){
	fprintf(stdout,"\nreset: %s\n",id);
    SymEntryPtr found;
    SymEntryPtr added;
    /* if I don't call symLookup(). 
    found = symtable;
    while(found!=NULL){
      if(strcmp(found->name, id) == 0)
        break;
      found = found->next;
    }*/
    curfunc = NULL;

    if((found = symLookup(id)) == NULL){
      added = symAdd(SYM_FUNC, id, cntGlobal++, 0, 0, NULL);
      hasProto = 0;
      curfunc = added;
    }else{
      if(found->sort == SYM_VAR && found->belong == NULL){
        /* ERROR */
        fprintf(stderr, "%s is global variable alredy exists.", found->name);
      }else if( found->sort == SYM_FUNC){
        DefNodePtr dnp = sourcedefs;
        while(dnp != NULL){
          if(strcmp(dnp->sym->name,id) == 0)
          //if(dnp->sym == found)
            break;
          dnp = dnp->next;
        }
        if(dnp!=NULL && dnp->sort == DEF_FUNC){
          //Error
        }else if(dnp!=NULL && dnp->sort == PROTOTYPE){
          hasProto = 1;
        }
        /*
        if(dnp->body != NULL){
          //Error
          //already exist function.
        }else{
          hasProto = 1;
          curfunc = found;
        }*/
        
      }
    }
    cntParam = 0;
    cntLocal = 0;
}

SymEntryPtr addGvar(char* id){
	fprintf(stdout,"addGvar()\n");
	if(symLookup(id) == NULL)
	  return symAdd(SYM_VAR,id,cntGlobal++,0,0,NULL);
	else{
	  /* ERROR */
	}
	return NULL;
}

StmtNodePtr getStmtBottom(StmtNodePtr stmt){
	fprintf(stdout,"getStmtBottom()\n");
	if(stmt == NULL)
	  return NULL;
	StmtNodePtr target = stmt;
	while(target->next != NULL){
	  target = target->next;
	}
	return target;
}
/*
StmtNodePtr setStmt(StmtNddePtr list, StmtNodePtr stmt){
	StmtNodePtr btm = list;
	if(btm != NULL)
	 while(btm->next != NULL)
	  btm = btm->next;
	btm->next = stmt;
	return btm;
}*/
StmtNodePtr setStmt(StmtNodePtr list, StmtNodePtr stmt){
	fprintf(stdout,"setStmt()\n");
	StmtNodePtr btm = list;
	if(list == NULL){
	  return stmt;
	}else{
	  while(btm->next != NULL)
	    btm = btm->next;
	  btm->next = stmt;
	}
	return list;
}


ExprNodePtr setAlist(ExprNodePtr list, ExprNodePtr expr){
	fprintf(stdout,"setAlist();\n");
	ExprNodePtr btm = list;
	if(btm == NULL)
	  return makeExpr(OP_ALIST,0,NULL,expr,NULL);
	else{
	  while(btm->sub2 != NULL)
	    btm = btm->sub2;
	  btm->sub2 = makeExpr(OP_ALIST,0,NULL,expr,NULL);
	}
	return list;
}
/*
int yylex(void) {
	return 0;
}*/


int yyerror(char *msg){
	fprintf(stderr, "yyerror(): \"%s\".\n", msg);
	return 0;
}

/*
int main(int argc, char *argv[]){
	//if(argc == 2 && strcmp(argv[1],"-d") == 0)
	  //yydebug = 1;
	yyparse();
	return 0;
}*/
