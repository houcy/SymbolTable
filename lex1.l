%{
#include "yaccExample.h"
#include "y.tab.h"

#define YY_INPUT(buf,result,max_size) \
	{ int c = GetSourceChar(); \
          result = (c == EOF) ? YY_NULL : (buf[0] = c, 1); \
        }

int determineType(char* yytext);

extern struct SymTab *table;
extern struct SymEntry *entry;

%}



letter [A-Za-z]
digit [0-9]

%%
if					{return IF;}
int					{return Int;}
bool                                                           {return Bool;}
print					{return Write;}
println                                                        {return Println;}
printsp                                                        {return Printsp;}
printstr                                                        {return Printstr;}
read                                                            {return READ;}
else                                                            {return ELSE;}
True                                                          {return True;}
False                                                          {return False;}
while                                                           {return WHILE;}
["][^"]*["]                                                   {return StrLit;}
{letter}({letter}|{digit})*	            {return determineType(yytext);}
{digit}{digit}*		                          {return IntLit;}
\=\=					{return EQ;}
\<\=                                                          {return LEQ;}
\>\=                                                          {return GEQ;}
\!\=                                                            {return NEQ;}
\!                                                                {return NOT;}
\,                                                                 {return ',';}
\&\&                                                            {return AND;}
\|\|                                                               {return OR;}
\<                                                              {return '<';}
\>                                                              {return '>';}
\=					{return '=';}
\+					{return '+';}
\-                                                               {return '-';}
\%                                                             {return '%';}
\^                                                              {return '^';}
\*					{return '*';}
\/                                                               {return '/';}
\;					{return ';';}
\{					{return '{';}
\}					{return '}';}
\(					{return '(';}
\)					{return ')';}

[ ]				{}
\t				{}
\r				{}
\n				{}

.				{WriteIndicator(GetCurrentColumn());
				   WriteMessage("Illegal Character in lex");}

%%


int determineType(char* yytext) {
       struct SymEntry *entry = FindName(table, yytext);

       if(entry == NULL) {
            return UndefinedIdent;
       } else {
            if(entry->Type == INTEGER)
                return IntIdent;
            
            return BoolIdent;
       }
}
	
yywrap () {
}
			
	