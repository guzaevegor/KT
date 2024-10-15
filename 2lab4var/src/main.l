%option noyywrap
%{
int line_number = 1;
%}
%%
^.*     { printf("%d:%s\n",line_number++, yytext);  }
[ ]{1,} 	{printf("%s",yytext);}

%%
int main(int argc, char **argv) {
    yylex();
    return 0;
}
