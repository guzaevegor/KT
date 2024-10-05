%option noyywrap

%{
int line_number = 1;
%}

%%
^.*     { printf("%d: %s \n", line_number++, yytext); }
\n      { }

%%

int main(int argc, char **argv) {
    yylex();
    return 0;
}
