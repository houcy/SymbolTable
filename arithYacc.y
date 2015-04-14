%{  

#include  "IOManager/IOMngr.h"  
#include  "Semantics.h"  
#include  <string.h>  

extern  int  yylex();   /*  The  next  token  funcTon.  */ 
extern  char  *yytext;  /*  The  matched  token  text.    */  
extern  int  yyleng;  
extern  int  yyerror(char  *);  

extern  struct  SymTab  *table;  
extern  struct  SymEntry  *entry;  

%} 

%union  {    
       char  *  string;  
       int num;
}

%type <string> Id
%type  <num>  Expr  
%type  <num>  Term  
%type  <num>  Factor  

%token INT
%token Ident

%%  


Prog                :      StmtSeq                                        {printSymTab();};  
StmtSeq          :      Stmt  StmtSeq                              {  };  
StmtSeq          :                                                            {  };  
Stmt                :      Id  '='  Expr  ';'                              {storeVar($1,  $3);};  
Expr                :      Expr  '+'  Term                              {$$  =  doADD($1,  $3);};  
Expr                :      Term                                              {$$  =  $1;};  
Term               :      Term  '*'  Factor                            {$$  =  doMULT($1,  $3);};  
Term               :      Factor                                            {$$  =  $1;};  
Factor             :      '-'Factor                                         {$$  =  doNEGATE($2);}; 
Factor             :      '('  Expr  ')'                                     {$$  =  $2;};  
Factor             :      Id                                                    {$$  =  getVal($1);};  
Factor             :      INT                                                  {$$  =  strtol(yytext, NULL, 10);}; 
Id                     :      Ident                                               {$$ = strdup(yytext);};


%% 

int  yyerror(char  *s)    {  
       WriteIndicator(GetCurrentColumn());  
       WriteMessage("Illegal  Character  in  YACC");  
       return  1;  
} 