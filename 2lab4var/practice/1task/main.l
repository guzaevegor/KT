%option noyywrap
%{
%}

%%
\n   { printf("i'm here!\n");}
.      { printf("%s \n", yytext);}

%%

int main(int argc, char **argv) {
    yylex();
    return 0;
}

