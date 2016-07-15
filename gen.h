#ifndef GEN_H
#define GEN_H

#include "ast.h"
//Generate code of expression
void genCodeExpr(ExprNodePtr expr);
void genCodeExprConst(ExprNodePtr expr);
void genCodeExprUm(ExprNodePtr expr);
void genCodeExprNot(ExprNodePtr expr);
void genCodeExprVar(ExprNodePtr expr);
void genCodeExprSub(ExprNodePtr expr); //-
void genCodeExprAdd(ExprNodePtr expr); //+
void genCodeExprBlt(ExprNodePtr expr); //<
void genCodeExprBeq(ExprNodePtr expr); //==
void genCodeExprBne(ExprNodePtr expr); //!=
void genCodeExprMul(ExprNodePtr expr);
void genCodeExprDiv(ExprNodePtr expr);
void genCodeExprMod(ExprNodePtr expr);
void genCodeExprAssign(ExprNodePtr expr);
void genCodeExprLor(ExprNodePtr expr);
void genCodeExprLand(ExprNodePtr expr);
void genCodeExprAnd(ExprNodePtr expr); //Bit Operation
void genCodeExprOr(ExprNodePtr expr);
void genCodeExprEor(ExprNodePtr expr);
void genCodeExprCmp(ExprNodePtr expr);
void genCodeExprLsl(ExprNodePtr expr);
void genCodeExprLsr(ExprNodePtr expr);
void genCodeExprInc(ExprNodePtr expr);
void genCodeExprDec(ExprNodePtr expr);
void genCodeExprFuncall(ExprNodePtr expr);
void genCodeExprAlist(ExprNodePtr expr);
void genCodeFunc(DefNode *func);
void genCodeFuncs();
void genCodeGlobals();
void genCode();
//Generate code of Statement
void genCodeStmt(StmtNodePtr stmt);
void genCodeStmtExpr(StmtNodePtr stmt);
void genCodeStmtIf(StmtNodePtr stmt);
void genCodeStmtWhile(StmtNodePtr stmt);
void genCodeStmtReturn(StmtNodePtr stmt);
#endif
