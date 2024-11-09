%{
#include "y.tab.h"
%}

%%

"if"       { return IF; }
"else"     { return ELSE; }
"for"      { return FOR; }
"while"    { return WHILE; }
"print"    { return PRINT; }
"in"       { return IN; }
":"        { return COLON; }
[a-zA-Z_][a-zA-Z0-9_]*   { return IDENTIFIER; }
[ \t\n]+   { /* пропуск пробелов и новой строки */ }
.          { yyerror("Invalid character"); }

%%
int yywrap() {
    return 1;
}
