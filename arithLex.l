%{
    #include "IOManager/IOMngr.h"
    #include <stdlib.h>
    #include <stdio.h>
    #include "y.tab.h"

    #define YY_INPUT(buf,result,max_size) \
    { int c = GetSourceChar(); \
    result = (c == EOF) ? YY_NULL : (buf[0] = c, 1);  \
    }

%}

letter [A-Za-z]
digit [0-9]

%% 

{digit}                                                 {return  INT;} 
{letter}({letter}|{digit})*                  {return  Ident;}  
\*                                                        {return  '*';}  
\+                                                        {return '+';}
\-                                                          {return '-';}  
\=                                                       {return  '=';}   
\;                                                         {return  ';';}  
\(                                                         {return  '(';}  
\)                                                         {return  ')';}
[ ]                                                        {}
\t                                                         {}
\n                                                        {}
.                                                          {
                                                                WriteIndicator(GetCurrentColumn());
                                                                WriteMessage("Illegal Character in lex");
                                                            }

%%

int yywrap() {
    return 1;
}