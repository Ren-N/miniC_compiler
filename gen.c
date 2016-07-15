#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "ast.h"
#include "gen.h"
//"FILE *af" is variable declared in other programs.
//To use "extern" be able to shere the variable between some programs.
extern FILE *af; //this variable is pointer of assembly file.
int labelNo = 0;
SymEntryPtr curfunc; //current function.
DefNodePtr sourcedefs;

//Generate code to evaluate the expression===============================================
void genCodeExpr(ExprNodePtr expr){
	switch (expr->op){
		case OP_CONST:
		  genCodeExprConst(expr);
		  break;
		case OP_UM:
		  genCodeExprUm(expr);
		  break;
		case OP_NOT:
		  genCodeExprNot(expr);
		  break;
		case OP_VAR:
		  genCodeExprVar(expr);
		  break;
		case OP_SUB:
		  genCodeExprSub(expr);
		  break;
		case OP_ADD:
		  genCodeExprAdd(expr);
		  break;
		case OP_BLT:
		  genCodeExprBlt(expr);
		  break;
		case OP_BEQ:
		  genCodeExprBeq(expr);
		  break;
		case OP_BNE:
		  genCodeExprBne(expr);
		  break;
		case OP_MUL:
		  genCodeExprMul(expr);
		  break;
		case OP_DIV:
		  genCodeExprDiv(expr);
		  break;
		case OP_MOD:
		  genCodeExprMod(expr);
		  break;
		case OP_ASSIGN:
		  genCodeExprAssign(expr);
		  break;
		case OP_LOR:
		  genCodeExprLor(expr);
		  break;
		case OP_LAND:
		  genCodeExprLand(expr);
		  break;
		case OP_OR:
		  genCodeExprOr(expr);
		  break;
		case OP_AND:
		  genCodeExprAnd(expr);
		  break;
		case OP_EOR:
		  genCodeExprEor(expr);
		  break;
		case OP_CMP:
		  genCodeExprCmp(expr);
		  break;
		case OP_LSL:
		  genCodeExprLsl(expr);
		  break;
		case OP_LSR:
		  genCodeExprLsr(expr);
		  break;
		case OP_INC:
		  genCodeExprInc(expr);
		  break;
		case OP_DEC:
		  genCodeExprDec(expr);
		  break;
		case OP_FUNCALL:
		  genCodeExprFuncall(expr);
		  break;
		case OP_ALIST:
		  genCodeExprAlist(expr);
		  break;
		default:
		  fprintf(stderr, "Undefined Expression\n");
		  exit(1);

	}
}

void genCodeExprConst(ExprNodePtr expr){
	fprintf(af, "      ld16 #%d ; Const\n", expr->val);
	fprintf(af, "      push\n");
}

void genCodeExprUm(ExprNodePtr expr){
	genCodeExpr(expr->sub1);
	fprintf(af, "      pop ; Um\n");
	fprintf(af, "      not\n");
	fprintf(af, "      inc\n");
	fprintf(af, "      push\n");
}

void genCodeExprNot(ExprNodePtr expr){
	genCodeExpr(expr->sub1);
	fprintf(af, "      pop ; Unary !\n");
	fprintf(af, "      or #0\n");
	fprintf(af, "      ld #1\n");
	fprintf(af, "      jpz L%04d\n", labelNo);
	fprintf(af, "      ld #0\n");
	fprintf(af, "L%04d:push\n", labelNo);
	labelNo++;
}


void genCodeExprVar(ExprNodePtr expr){
	SymEntryPtr s = expr->sym;
	int offset;
	switch(s->sort){
		case SYM_VAR:
		  if (s->belong==NULL)
		  {
		  	fprintf(af, "      ld G%04d ;  %s ; globalVar\n", s->no, s->name);
		  	fprintf(af, "      push\n");
		  }else{
		  	offset = - (s->no);
		  	fprintf(af, "      ld _FP ;  %s ; localVar\n",s->name);
		  	fprintf(af, "      mv ixr, acc\n");
		  	fprintf(af, "      ld ixr, %d\n", offset);
		  	fprintf(af, "      push\n");
		  }
		  break;
		case SYM_PARAM:
		  offset = 2+ (s->belong->nParam) - (s->no);
		  fprintf(af, "      ld _FP ;  %s ; funcParam\n", s->name);
		  fprintf(af, "      mv ixr, acc\n");
		  fprintf(af, "      ld ixr, %d\n", offset);
		  fprintf(af, "      push\n");
		  break;
		default:
		  fprintf(stderr, "Undefined variable Class\n");
	}
}
void genCodeExprVar_old(ExprNodePtr expr){
	SymEntryPtr s = expr->sym;
	switch(s->sort){
		case SYM_GLOBAL:
		  	fprintf(af, "      ld G%04d ;  %s ; Var\n", s->no, s->name);
		  	fprintf(af, "      push\n");
		  	break;
		default:
		  fprintf(stderr, "Undefined variable Class\n");
	}
}

void genCodeExprSub(ExprNodePtr expr){
	genCodeExpr(expr->sub1);
	genCodeExpr(expr->sub2);
	fprintf(af, "      mv ixr, sp ; Sub\n");
	fprintf(af, "      ld ixr, 1\n");
	fprintf(af, "      sub ixr, 0\n");
	fprintf(af, "      st ixr, 1\n");
	fprintf(af, "      inc sp\n");
}

void genCodeExprAdd(ExprNodePtr expr){
	genCodeExpr(expr->sub1);
	genCodeExpr(expr->sub2);
	fprintf(af, "      mv ixr, sp ; Add\n");
	fprintf(af, "      ld ixr, 1\n");
	fprintf(af, "      add ixr, 0\n");
	fprintf(af, "      st ixr, 1\n");
	fprintf(af, "      inc sp\n");
}

void genCodeExprBlt(ExprNodePtr expr){
	genCodeExpr(expr->sub1);
	genCodeExpr(expr->sub2);
	fprintf(af, "      mv ixr, sp ; BLT\n");
	fprintf(af, "      ld ixr, 1\n");
	fprintf(af, "      sub ixr, 0\n");
	fprintf(af, "      ld #1\n");
	fprintf(af, "      jps L%04d\n", labelNo);
	fprintf(af, "      ld #0\n");
	fprintf(af, "L%04d:st ixr, 1\n", labelNo);
	fprintf(af, "      inc sp\n");
	labelNo++;
}

void genCodeExprBeq(ExprNodePtr expr){
	genCodeExpr(expr->sub1);
	genCodeExpr(expr->sub2);
	fprintf(af, "      mv ixr, sp ; BEQ\n");
	fprintf(af, "      ld ixr, 1\n");
	fprintf(af, "      sub ixr, 0\n");
	fprintf(af, "      ld #1\n");
	fprintf(af, "      jpz L%04d\n", labelNo);
	fprintf(af, "      ld #0\n");
	fprintf(af, "L%04d:st ixr, 1\n", labelNo);
	fprintf(af, "      inc sp\n");
	labelNo++;
}

void genCodeExprBne(ExprNodePtr expr){
	genCodeExpr(expr->sub1);
	genCodeExpr(expr->sub2);
	fprintf(af, "      mv ixr, sp ; BNE\n");
	fprintf(af, "      ld ixr, 1\n");
	fprintf(af, "      sub ixr, 0\n");
	fprintf(af, "      ld #0\n");
	fprintf(af, "      jpz L%04d\n", labelNo);
	fprintf(af, "      ld #1\n");
	fprintf(af, "L%04d:st ixr, 1\n", labelNo);
	fprintf(af, "      inc sp\n");
	labelNo++;
}

void genCodeExprMul(ExprNodePtr expr){
	genCodeExpr(expr->sub1);
	genCodeExpr(expr->sub2);
	fprintf(af, "      call _mul ; Mul\n");
	fprintf(af, "      inc sp\n");
}

void genCodeExprDiv(ExprNodePtr expr){
	genCodeExpr(expr->sub1);
	genCodeExpr(expr->sub2);
	fprintf(af, "      call _div ; Div\n");
	fprintf(af, "      mv ixr, sp\n");
	fprintf(af, "      ld ixr, 0\n");
	fprintf(af, "      st ixr, 1\n");
	fprintf(af, "      inc sp\n");
}

void genCodeExprMod(ExprNodePtr expr){
	genCodeExpr(expr->sub1);
	genCodeExpr(expr->sub2);
	fprintf(af, "      call _div ; Mod\n");
	fprintf(af, "      inc sp\n");
}

void genCodeExprAssign(ExprNodePtr expr){
	SymEntryPtr s = expr->sym;
	int offset;
	genCodeExpr(expr->sub1);
	switch(s->sort){
		case SYM_VAR:
		  if (s->belong == NULL){
		  	//GLOBAL
		  	fprintf(af, "      pop ; globalVar=Assignment\n");
		  	fprintf(af, "      st G%04d\n",s->no);
		  	fprintf(af, "      push\n");
		  }else{
		  	//LOCAL
		  	offset = - (s->no);
		  	fprintf(af, "      ld _FP ; localVar=Assignment\n");
		  	fprintf(af, "      mv ixr, acc\n");
		  	fprintf(af, "      pop\n");
		  	fprintf(af, "      st ixr, %d\n", offset);
		  	fprintf(af, "      push\n");
		  }
		  break;
		case SYM_PARAM:
		  offset = 2+ (s->belong->nParam) - (s->no);
		  fprintf(af, "      ld _FP ; funcParam=Assignment\n");
		  fprintf(af, "      mv ixr, acc\n");
		  fprintf(af, "      pop\n");
		  fprintf(af, "      st ixr, %d\n", offset);
		  fprintf(af, "      push\n");
		  break;
		default:
		  fprintf(stderr, "Assignment Error.\n");
	}
	
}
void genCodeExprAssign_old(ExprNodePtr expr){
	genCodeExpr(expr->sub1);
	fprintf(af, "      pop ; =Assignment\n");
	fprintf(af, "      st G%04d\n",expr->sym->no);
	fprintf(af, "      push\n");
}

void genCodeExprLor(ExprNodePtr expr){
	int l1 = labelNo++;
	int l2 = labelNo++;
	int l3 = labelNo++;
	genCodeExpr(expr->sub1);
	genCodeExpr(expr->sub2);
	fprintf(af, "      pop ; ||\n");
	fprintf(af, "      or #0\n");
	fprintf(af, "      jpz L%04d\n", l2);
	fprintf(af, "      jp L%04d\n", l3);
	fprintf(af, "L%04d:pop\n", l2);
	fprintf(af, "      or #0\n");
	fprintf(af, "      jpz L%04d\n", l1);
	fprintf(af, "L%04d:ld #1\n", l3);
	fprintf(af, "L%04d:push\n", l1);
}

void genCodeExprLand(ExprNodePtr expr){
	genCodeExpr(expr->sub1);
	genCodeExpr(expr->sub2);
	fprintf(af, "      pop ; &&\n");
	fprintf(af, "      or #0\n");
	fprintf(af, "      jpz L%04d\n", labelNo);
	fprintf(af, "      pop\n");
	fprintf(af, "      or #0\n");
	fprintf(af, "      jpz L%04d\n", labelNo);
	fprintf(af, "      ld #1\n");
	fprintf(af, "L%04d:push\n", labelNo);
	labelNo++;
}

void genCodeExprOr(ExprNodePtr expr){
	genCodeExpr(expr->sub1);
	genCodeExpr(expr->sub2);
	fprintf(af, "      mv ixr, sp ; or_|\n");
	fprintf(af, "      ld ixr, 1\n");
	fprintf(af, "      or ixr, 0\n");
	fprintf(af, "      st ixr, 1\n");
	fprintf(af, "      inc sp\n");
}
void genCodeExprAnd(ExprNodePtr expr){
	genCodeExpr(expr->sub1);
	genCodeExpr(expr->sub2);
	fprintf(af, "      mv ixr, sp ; and_&\n");
	fprintf(af, "      ld ixr, 1\n");
	fprintf(af, "      and ixr, 0\n");
	fprintf(af, "      st ixr, 1\n");
	fprintf(af, "      inc sp\n");
}
void genCodeExprEor(ExprNodePtr expr){
	genCodeExpr(expr->sub1);
	genCodeExpr(expr->sub2);
	fprintf(af, "      mv ixr, sp ; eor_^\n");
	fprintf(af, "      ld ixr, 1\n");
	fprintf(af, "      eor ixr, 0\n");
	fprintf(af, "      st ixr, 1\n");
	fprintf(af, "      inc sp\n");
}
void genCodeExprCmp(ExprNodePtr expr){
	genCodeExpr(expr->sub1);
	fprintf(af, "      pop ; cmp_~\n");
	fprintf(af, "      not\n");
	fprintf(af, "      push\n");
}
void genCodeExprLsl(ExprNodePtr expr){
	genCodeExpr(expr->sub1);
	genCodeExpr(expr->sub2);
	int l1 = labelNo++;
	int l2 = labelNo++;
	fprintf(af, "      mv ixr, sp ; LSL_<<\n");
	fprintf(af, "L%04d:ld ixr, 0\n", l2);
	fprintf(af, "      or #0\n");
	fprintf(af, "      jpz L%04d\n", l1);
	fprintf(af, "      jps L%04d\n", l1);
	fprintf(af, "      dec\n");
	fprintf(af, "      st ixr, 0\n");
	fprintf(af, "      ld ixr, 1\n");
	fprintf(af, "      lsl\n");
	fprintf(af, "      st ixr, 1\n");
	fprintf(af, "      jp L%04d\n", l2);
	fprintf(af, "L%04d:\n", l1);
	/*Q. if sub2 is variable, you couldn't get integer value.
	//sub1 << sub2 , sub2 is constant value.
	int num = sub2->val;
	fprintf(af, "      mv ixr, sp\n");
	fprintf(af, "      ld ixr, 1\n");
	for (int i = 0; i < num; ++i) {
		fprintf(af, "      lsl\n");
	}
	fprintf(af, "      st ixr, 1\n");
	fprintf(af, "      inc sp\n");*/
}
void genCodeExprLsr(ExprNodePtr expr){
	genCodeExpr(expr->sub1);
	genCodeExpr(expr->sub2);
	int l1 = labelNo++;
	int l2 = labelNo++;
	fprintf(af, "      mv ixr, sp ; LSR_>>\n");
	fprintf(af, "L%04d:ld ixr, 0\n", l2);
	fprintf(af, "      or #0\n");
	fprintf(af, "      jpz L%04d\n", l1);
	fprintf(af, "      jps L%04d\n", l1);
	fprintf(af, "      dec\n");
	fprintf(af, "      st ixr, 0\n");
	fprintf(af, "      ld ixr, 1\n");
	fprintf(af, "      lsr\n");
	fprintf(af, "      st ixr, 1\n");
	fprintf(af, "      jp L%04d\n", l2);
	fprintf(af, "L%04d:\n", l1);
}

void genCodeExprInc(ExprNodePtr expr){
	genCodeExpr(expr->sub1);
	fprintf(af, "      pop ; Increment_++\n");
	fprintf(af, "      inc acc, acc\n");
	if (expr->sub1->sym != NULL)
		fprintf(af, "      st G%04d\n", expr->sub1->sym->no);
	fprintf(af, "      push\n");
}
void genCodeExprDec(ExprNodePtr expr){
	genCodeExpr(expr->sub1);
	fprintf(af, "      pop ; Decrement_--\n");
	fprintf(af, "      dec\n");
	if (expr->sub1->sym != NULL)
		fprintf(af, "      st G%04d\n", expr->sub1->sym->no);
	fprintf(af, "      push\n");
}

void genCodeExprFuncall(ExprNodePtr expr){
	fprintf(stdout, "      exprFUNCALL:%s\n",expr->sym->name);
	int i = 0;
	fprintf(af, "      dec sp ; FunctionCall\n");
	//fprintf(stdout, "bbbbbbb\n");
	if (expr->sub1 != NULL){
	//	if(expr->sub1->op == OP_ALIST)
	//	fprintf(stdout, "alist\n");
		genCodeExpr(expr->sub1);
	}
    //	fprintf(stdout, "aaaaaa\n");
	if (strcmp(expr->sym->name, "in")==0){
		fprintf(af, "      call _in\n");
	}else if(strcmp(expr->sym->name, "out")==0){
		fprintf(af, "      call _out\n");
	}else{
		fprintf(af, "      call F%04d\n", expr->sym->no);
	}

	//fprintf(af, "mv acc, sp\n");
	//fprintf(af, "add #%d\n",expr->sym->nParam );
	//fprintf(af, "mv sp,acc\n");
	for (i = 0; i < expr->sym->nParam; i++)
		fprintf(af, "inc sp\n");
	fprintf(stdout, "      exprFUNCALL_END\n");
}

void genCodeExprAlist(ExprNodePtr expr){
	genCodeExpr(expr->sub1);
	if (expr->sub2 != NULL)
		genCodeExpr(expr->sub2);
}


//Generate Statement Function===========================================================
void genCodeStmt(StmtNodePtr stmt){
	while(stmt != NULL){
		switch(stmt->sort){
			case STMT_EXPR:
			  genCodeStmtExpr(stmt);
			  break;
			case STMT_IF:
			  genCodeStmtIf(stmt);
			  break;
			case STMT_WHILE:
			  genCodeStmtWhile(stmt);
			  break;
			case STMT_RETURN:
			  genCodeStmtReturn(stmt);
			  break;
			default:
			  fprintf(stderr, "Undefined Statement\n");
			  exit(1);
		}
		stmt = stmt->next;
	}
}

void genCodeStmtExpr(StmtNodePtr stmt){
	fprintf(stdout, "    stmtEXPR:\n");
	//if(stmt->expr != NULL)
	//	fprintf(stdout, "genCodeStmtExpr: not NULL\n");
	genCodeExpr(stmt->expr);
	fprintf(af, "      inc sp\n");
}

void genCodeStmtIf(StmtNodePtr stmt){
	fprintf(stdout, "    stmtIF:\n");
    int no = labelNo;
    labelNo++;
	genCodeExpr(stmt->expr);
	fprintf(af, "      pop ; IF\n");
	fprintf(af, "      or #0\n");
	fprintf(af, "      jpz IFELSE%04d\n", no);
	genCodeStmt(stmt->st1); //Generate code by st1.
	if (stmt->st2 != NULL)
		fprintf(af, "     jp IFEND%04d\n", no);
	fprintf(af, "IFELSE%04d:\n", no);
	genCodeStmt(stmt->st2); //if st2 is NULL, some code wouldn't be generated.
	fprintf(af, "IFEND%04d:\n", no);
}

void genCodeStmtWhile(StmtNodePtr stmt){
	fprintf(stdout, "    stmtWHILE:\n");
  int no = labelNo;
  labelNo++;
	fprintf(af, "WHILE%04d: ; while\n", no);
	genCodeExpr(stmt->expr);
	fprintf(af, "      pop\n");
	fprintf(af, "      or #0\n");
	fprintf(af, "      jpz WHILEEND%04d\n", no);
	genCodeStmt(stmt->st1);
	fprintf(af, "      jp WHILE%04d\n", no);
	fprintf(af, "WHILEEND%04d:\n", no);
}

void genCodeStmtReturn(StmtNodePtr stmt){
	fprintf(stdout, "    stmtRETURN\n");
	genCodeExpr(stmt->expr);
	fprintf(af, "      jp E%04d\n", curfunc->no);
}

//Generate Define the function of code.================================================
void genCodeFunc(DefNode *func){
	fprintf(stdout, "  genCodeFunc:%s+++++++++++++\n",func->sym->name);
	curfunc = func->sym;
	/*function head*/
	fprintf(af, "F%04d:ld _FP ;funcName:%s\n", curfunc->no,curfunc->name);
	fprintf(af, "      push\n");
	fprintf(af, "      mv acc, sp\n");
	fprintf(af, "      st _FP\n");
	fprintf(af, "      sub #%d\n", curfunc->nVar);
	fprintf(af, "      mv sp, acc\n");
	/*fucntion body*/
	genCodeStmt(func->body);
	fprintf(stdout, "end body\n");
	/*function footer*/
	fprintf(af, "      push\n");
	fprintf(af, "E%04d:ld _FP\n", curfunc->no);
	fprintf(af, "      mv ixr, acc\n");
	fprintf(af, "      pop\n");
	fprintf(af, "      st ixr, %d\n", curfunc->nParam + 2);
	fprintf(af, "      mv sp, ixr\n");
	fprintf(af, "      pop\n");
	fprintf(af, "      st _FP\n");
	fprintf(af, "      ret\n");
}
void genCodeFuncs(){
	fprintf(af, ".text\n");
	DefNodePtr curnode = sourcedefs;
	while(curnode != NULL){
		if (curnode->sort == DEF_FUNC && curnode->body != NULL){
			//fprintf(af, "_%s:",curnode->name ); //function label.
			//genCodeStmt(curnode->body);
			genCodeFunc(curnode);
		}
		curnode = curnode->next;
	}
}

void genCodeGlobals(){
	fprintf(af, ".data\n");
	fprintf(af, "_FP:.space 1\n");
	DefNodePtr curnode = sourcedefs;
	while(curnode != NULL){
		if (curnode->sort == DEF_GLOBAL){
			fprintf(af, "G%04d:.space 1\n", sourcedefs->sym->no);
		}
		curnode = curnode->next;
	}
}

//===============================================================
void genCode(){
	int main_no;
	DefNodePtr curnode = sourcedefs;
	while(curnode != NULL){
		fprintf(stdout, "search sourcedefs.\n");
		if (curnode->sort == DEF_FUNC && strcmp(curnode->sym->name, "main")==0){
			//if (curnode->sym->nParam == 0){
				fprintf(stdout, "find out main function.\n");
				main_no = curnode->sym->no;
			//}
			break;
		}
		curnode = curnode->next;
	}
	//main function
	fprintf(af, ".text\n");
	fprintf(af, "CRT0:ld16 #0xFFE\n");
	fprintf(af, "      mv sp, acc\n");
	fprintf(af, "      call F%04d\n", main_no);
	fprintf(af, "      halt\n");
	//datasegment
	fprintf(stdout,"Generate code of the global variables.\n");
	genCodeGlobals();
	fprintf(stdout,"Generate code of the Functions.\n");
	genCodeFuncs();
	//Subroutine--------------------------------
	//library routine. execute lib.sh. "cat _*.sh"
	//system("bash lib.sh");
	fprintf(stdout, "Generate code of the Subroutine.\n");
	FILE* lib_inout = fopen("_in_out.s","r");
	if (lib_inout == NULL){
		printf("cannot open _in_out.s\n");
		exit(1);
	}
	//char* line;
	char line[24];
	while(fgets(line,sizeof(line),lib_inout) != NULL){
		fprintf(af, "%s",line);
	}
	fclose(lib_inout);
	//-------------------------------------------
	//start address
	fprintf(af, "\n.const 0xfff\n");
	fprintf(af, ".word CRT0\n");
}

