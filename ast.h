#ifndef AST_H
#define AST_H


//OpSort is the constant value of the operating code.
//There are enumerated types.--------------------------------------
typedef enum{
	OP_ASSIGN,
	OP_LOR, OP_LAND, OP_BLT, OP_BEQ, OP_BNE,//LAND is LogicalAdd.
	OP_ADD, OP_SUB, OP_MUL, OP_DIV, OP_MOD,
	OP_NOT, OP_UM,
	OP_VAR,
	OP_CONST,
	//Bit Operation
	OP_AND, OP_OR, OP_EOR, OP_CMP, OP_LSL, OP_LSR,
	OP_INC,OP_DEC,
	//function
	OP_ALIST, OP_FUNCALL

} OpSort;

typedef enum{
	SYM_GLOBAL,
	SYM_VAR, SYM_PARAM, SYM_FUNC
} SymbolSort;

typedef enum{
	STMT_EXPR, STMT_IF, STMT_WHILE, STMT_RETURN
} StmtSort;
/*Define the function*/
typedef enum{
	DEF_FUNC, DEF_GLOBAL, PROTOTYPE
} DefSort;

//There are structures and pointers.--------------------------------
typedef struct SymEntry *SymEntryPtr;
typedef struct SymEntry {
	SymbolSort sort;
	char *name;
	int no;
	int nParam, nVar;
	SymEntryPtr belong;
	SymEntryPtr prev;
} SymEntry;   


typedef struct ExprNode *ExprNodePtr;
typedef struct ExprNode {
	OpSort op;
	int val;
	SymEntryPtr sym;
	ExprNodePtr sub1;
	ExprNodePtr sub2;	
} ExprNode;


typedef struct StmtNode *StmtNodePtr;
typedef struct StmtNode {
	StmtSort sort;
	StmtNodePtr next;
	ExprNodePtr expr;
	StmtNodePtr st1, st2;
} StmtNode;

/*Define the function*/
typedef struct DefNode *DefNodePtr;
typedef struct DefNode{
	DefSort sort;
	SymEntryPtr sym;
	DefNodePtr next;
	StmtNodePtr body;	
} DefNode;


extern SymEntryPtr symtable;
extern SymEntryPtr curfunc;

//Prototype declaration.-------------------------------------------
ExprNodePtr makeExpr(OpSort opr, int value, SymEntryPtr symbol,
	ExprNodePtr left, ExprNodePtr right);
StmtNodePtr makeStmt(StmtSort sort, ExprNodePtr expr,
	StmtNodePtr st1, StmtNodePtr st2);
DefNodePtr makeDef(DefSort sort, SymEntryPtr sym, StmtNodePtr body);
//Functions that related to Symbol Entry.--------------------------
SymEntryPtr symAdd(SymbolSort sort, char *name, int no, int nParam,
	int nVar, SymEntryPtr belong);
SymEntryPtr symLookup(char *name);



#endif