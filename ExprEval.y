%{
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "yaccExample.h"
#include "Semantics.h"
#include "CodeGen.h"

extern int yylex();	/* The next token function. */
extern char *yytext;   /* The matched token text.  */
extern int yyleng;      /* The token text length.   */
extern int yyparse();
void dumpTable();

extern struct SymTab *table;
extern struct SymEntry *entry;

%}


%union {
  long val;
  char * string;
  struct ExprRes * ExprRes;
  struct InstrSeq * InstrSeq;
  struct BExprRes * BExprRes;
  struct ExprResList * ExprResList;
  struct IdList * IdList;
}

%type <string> IntId
%type <string> BoolId
%type <string> BoolArrId
%type <string> IntArrId
%type <string> StringLiteral
%type <string> UndefinedId
%type <ExprRes> Factor
%type <ExprRes> Term
%type <ExprRes> Expr
%type <ExprRes> Expon
%type <ExprRes> BTerm
%type <ExprRes> BOr
%type <ExprRes> BAnd
%type <ExprRes> BNot
%type <InstrSeq> StmtSeq
%type <InstrSeq> Stmt
%type <ExprRes> BoolFactor
%type <ExprRes> BoolEq;
%type <BExprRes> BExpr
%type <ExprResList> List;
%type <IdList> IDList;

%token UndefinedIdent	
%token StrLit
%token IntIdent
%token IntArrIdent
%token BoolArrIdent
%token BoolIdent
%token IntLit 	
%token True
%token False
%token Int
%token Bool
%token Write
%token Println
%token Printsp
%token Printstr
%token IF
%token ELSE
%token EQ
%token NEQ
%token GEQ
%token LEQ	
%token AND 
%token OR 
%token NOT
%token WHILE
%token READ
%token IntArr
%token BoolArr

%%

Prog			:	Declarations StmtSeq						             {Finish($2); } ;
Declarations	             :	Dec Declarations							{ };
Declarations	             :										{ };

Dec                                 :            Int  UndefinedId '[' Factor {EnterName(table, $2, &entry); SetType(entry, INTARR); SetAttr(entry, (void*) strdup(yytext)); } ']' ';'                  {};
Dec                                 :            Bool UndefinedId '[' Factor {EnterName(table, $2, &entry); SetType(entry, BOOLARR); SetAttr(entry, (void*) strdup(yytext));} ']' ';'                {};
Dec			:	Int UndefinedId ';'	                                                                            {EnterName(table, $2, &entry); SetType(entry, INTEGER); };
Dec                                 :            Bool UndefinedId ';'                                                                                   {EnterName(table, $2, &entry); SetType(entry, BOOLEAN);};

StmtSeq 		:	Stmt StmtSeq								{$$ = AppendSeq($1, $2); } ;
StmtSeq		:										{$$ = NULL;} ;

Stmt                                :           Write BoolFactor ';'                                                                                            {$$ = doPrintBool($2,1); };
Stmt                                :           Write Expr ';'                                                                                              {$$ = doPrint($2,1); };
Stmt                                :           Write '(' List ')' ';'                                                                                     {$$ = doPrintList($3);};
Stmt                                :           Write IntArrId '[' Expr ']' ';'                                                                           {$$ = doPrintArr($2, $4, 0);}; //0 for int
Stmt                                :           Write BoolArrId '[' Expr ']' ';'                                                                        {$$ = doPrintArr($2, $4, 1);}; //1 for bool
Stmt                                :           Println '(' ')' ';'                                                                                {$$ = doPrintLine();};
Stmt                                :           Printsp '(' Expr ')' ';'                                                                                {$$ = doPrintSpaces($3);};
Stmt                                :           Printstr  StringLiteral  ';'                                                              {$$ = doPrintStringLit($2);};

Stmt                                :           IntId '=' Expr ';'                                                                                     {$$ = doAssign($1, $3);} ;
Stmt                                :           BoolId '=' BOr ';'                                                                                     {$$ = doAssign($1, $3);} ;
Stmt                                :           IntArrId '[' Expr ']' '=' Expr ';'                                                               {$$ = doAssignArr($1, $3, $6);};
Stmt                                :           BoolArrId '[' Expr ']' '=' BOr ';'                                                              {$$ = doAssignArr($1, $3, $6);};

Stmt			 :	IF '(' BExpr ')' '{' StmtSeq '}'					             {$$ = doIf($3, $6);};
Stmt                                :           IF '(' BExpr ')' '{' StmtSeq '}' ELSE '{' StmtSeq '}'                                      {$$ = doIfElse($3, $6, $10);};
Stmt                                :           WHILE '(' BExpr ')' '{' StmtSeq '}'                                                                 {$$ = doWhile($3, $6);};

Stmt                                :           READ '(' IDList ')' ';'                                                                                                  {$$ = doRead($3);};

StringLiteral                     :             StrLit                                                                                                                { $$ = strdup(yytext);};

IDList                                :            IntArrId '[' Expr ']' ',' IDList                                                                              {$$ = addToIDList($1, $6, $3);};
IDList                                :            BoolArrId '[' Expr ']' ',' IDList                                                                            {$$ = addToIDList($1, $6, $3);};
IDList                                :            IntId ',' IDList                                                                                                   {$$ = addToIDList($1, $3, NULL);};
IDList                                :            BoolId ',' IDList                                                                                                {$$ = addToIDList($1, $3, NULL);};
IDList                                :            IntArrId '[' Expr ']'                                                                                             {$$ = addToIDList($1, NULL, $3);};
IDList                                :            BoolArrId '[' Expr ']'                                                                                          {$$ = addToIDList($1, NULL, $3);};
IDList                                :            IntId                                                                                                                  {$$ = addToIDList($1, NULL, NULL);};
IDList                                :            BoolId                                                                                                              {$$ = addToIDList($1, NULL, NULL);};

List                                    :           BOr ',' List                                                                                                        {$$ = addToExpressionList($1, $3, 1);};  
List                                    :           Expr ',' List                                                                                                       {$$ = addToExpressionList($1, $3, 0);};                                                                                                       
List                                    :           Expr                                                                                                                  {$$ = makeExprResList($1, 0);};
List                                    :           BOr                                                                                                                   {$$ = makeExprResList($1, 1);};

BExpr                              :             BOr                                                                                                                {$$ = doBExpr($1);};
BOr                                  :              BOr OR BAnd                                                                                                 {$$ = doBoolOr($1, $3);};
BOr                                  :              BAnd                                                                                                               {$$ = $1;};
BAnd                                 :            BAnd AND BNot                                                                                             {$$ = doBoolAnd($1, $3);};
BAnd                                :             BNot                                                                                                              {$$ = $1;};
BNot                                 :             NOT BNot                                                                                                            {$$ = doBoolNot($2);};
BNot                                :           BoolEq                                                                                                               {$$ = $1;};

BoolEq		             :	  Expr EQ Expr								{$$ = doBoolOperation($1, $3, "seq");};
BoolEq                              :            Expr NEQ Expr                                                                                                {$$ = doBoolOperation($1, $3, "sne");};
BoolEq                              :            BTerm                                                                                                              {$$ = $1;};
BTerm                              :            Expr GEQ Expr                                                                                                {$$ = doBoolOperation($1, $3, "sge");};
BTerm                              :            Expr LEQ Expr                                                                                                {$$ = doBoolOperation($1, $3, "sle");};
BTerm                              :            Expr '>' Expr                                                                                                {$$ = doBoolOperation($1, $3, "sgt");};
BTerm                              :            Expr '<' Expr                                                                                                {$$ = doBoolOperation($1, $3, "slt");};
BTerm                              :            BoolFactor                                                                                                             {$$ = $1;};

Expr			:	Expr '+' Term								{$$ = doAdd($1, $3); } ;
Expr                                :            Expr '-' Term                                                                                                  {$$ = doSub($1, $3); } ;
Expr			:	Term									{$$ = $1; } ;
Term		             :	Term '*' Expon								{ $$ = doMult($1, $3); } ;
Term                              :             Term '/' Expon                                                                                            { $$ = doDiv($1, $3); } ;
Term                               :            Term '%' Expon                                                                                           { $$ = doMod($1, $3); } ;
Term		             :	Expon									{ $$ = $1; } ;
Expon                               :           Expon '^' Factor                                                                                            { $$ = doExpon($1, $3); } ;
Expon                               :           Factor                                                                                                            { $$ = $1; } ;
Factor                              :           '-'Factor                                                                                                         {$$ = doNegate($2);};
Factor                             :            '(' Expr ')'                                                                                                      { $$ = $2; };
Factor		             :	IntLit									{ $$ = doIntLit(yytext); };
Factor		             :	IntIdent									{ $$ = doRval(yytext); };
IntId                                  :          IntIdent                                                                                                         { $$ = strdup(yytext);};
IntArrId                            :           IntArrIdent                                                                                                     { $$ = strdup(yytext);};

UndefinedId                     :           UndefinedIdent                                                                                              { $$ = strdup(yytext);};

BoolFactor                      :          '(' BOr ')'                                                                                                           { $$ = $2;};
BoolFactor                      :          True                                                                                                                 { $$ = doIntLit("1");};
BoolFactor                      :           False                                                                                                               { $$ = doIntLit("0");};
BoolFactor                      :           BoolIdent                                                                                                      { $$ = doRval(yytext);};
BoolId			: 	BoolIdent							            { $$ = strdup(yytext);}
BoolArrId                        :            BoolArrIdent                                                                                                 { $$ = strdup(yytext);};

%%

yyerror(char *s)  {
  WriteIndicator(GetCurrentColumn());
  WriteMessage("Illegal Character in YACC");
}
