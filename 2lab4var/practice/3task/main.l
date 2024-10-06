%option noyywrap
%{
%}

%%

\<\!\--(.|\n)*?--\>   { /* Удаляем комментарии <!-- ... --> */ }
.                 { printf("%s", yytext); }

%%

int main(int argc, char **argv) {
    yylex();
    return 0;
}

