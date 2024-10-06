%option noyywrap
%{

%}

%%

\/\*([^*]|\*+[^/])*\*\/     {  }
\/\/.*                            { }
.                                 { printf("%s", yytext); }

%%

int main(int argc, char **argv) {
    yylex();
    return 0;
}

