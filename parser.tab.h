/* A Bison parser, made by GNU Bison 2.3.  */

/* Skeleton interface for Bison's Yacc-like parsers in C

   Copyright (C) 1984, 1989, 1990, 2000, 2001, 2002, 2003, 2004, 2005, 2006
   Free Software Foundation, Inc.

   This program is free software; you can redistribute it and/or modify
   it under the terms of the GNU General Public License as published by
   the Free Software Foundation; either version 2, or (at your option)
   any later version.

   This program is distributed in the hope that it will be useful,
   but WITHOUT ANY WARRANTY; without even the implied warranty of
   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
   GNU General Public License for more details.

   You should have received a copy of the GNU General Public License
   along with this program; if not, write to the Free Software
   Foundation, Inc., 51 Franklin Street, Fifth Floor,
   Boston, MA 02110-1301, USA.  */

/* As a special exception, you may create a larger work that contains
   part or all of the Bison parser skeleton and distribute that work
   under terms of your choice, so long as that work isn't itself a
   parser generator using the skeleton or a modified version thereof
   as a parser skeleton.  Alternatively, if you modify or redistribute
   the parser skeleton itself, you may (at your option) remove this
   special exception, which will cause the skeleton and the resulting
   Bison output files to be licensed under the GNU General Public
   License without this special exception.

   This special exception was added by the Free Software Foundation in
   version 2.2 of Bison.  */

/* Tokens.  */
#include "ast.h"

#ifndef YYTOKENTYPE
# define YYTOKENTYPE
   /* Put the tokens into the symbol table, so that GDB and other debuggers
      know about them.  */
   enum yytokentype {
     NUM = 258,
     ID = 259,
     IF = 260,
     ELSE = 261,
     WHILE = 262,
     RETURN = 263,
     TINT = 264,
     ASSIGN = 265,
     LOR = 266,
     LAND = 267,
     OR = 268,
     EOR = 269,
     AND = 270,
     BNE = 271,
     BEQ = 272,
     BLT = 273,
     LSR = 274,
     LSL = 275,
     SUB = 276,
     ADD = 277,
     MOD = 278,
     DIV = 279,
     MUL = 280,
     FUNCALL = 281,
     CMP = 282,
     UM = 283,
     NOT = 284,
     INC = 285,
     DEC = 286
   };
#endif
/* Tokens.  */
#define NUM 258
#define ID 259
#define IF 260
#define ELSE 261
#define WHILE 262
#define RETURN 263
#define TINT 264
#define ASSIGN 265
#define LOR 266
#define LAND 267
#define OR 268
#define EOR 269
#define AND 270
#define BNE 271
#define BEQ 272
#define BLT 273
#define LSR 274
#define LSL 275
#define SUB 276
#define ADD 277
#define MOD 278
#define DIV 279
#define MUL 280
#define FUNCALL 281
#define CMP 282
#define UM 283
#define NOT 284
#define INC 285
#define DEC 286




#if ! defined YYSTYPE && ! defined YYSTYPE_IS_DECLARED
typedef union YYSTYPE
#line 28 "parser.y"
{
	int  ival;
	char *name;
	ExprNodePtr expr, alist;
	StmtNodePtr stmt;
	DefNodePtr def;
	SymEntryPtr sym;
}
/* Line 1529 of yacc.c.  */
#line 120 "parser.tab.h"
	YYSTYPE;
# define yystype YYSTYPE /* obsolescent; will be withdrawn */
# define YYSTYPE_IS_DECLARED 1
# define YYSTYPE_IS_TRIVIAL 1
#endif

extern YYSTYPE yylval;

