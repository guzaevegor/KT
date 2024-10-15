%option noyywrap
%{
int counter = 1;
%}
%%
\n{2,}              { printf("\n"); }  // Две и более пустых строки
^[^\n]+             { printf("    %d %s\n", counter++, yytext); }  // Добавляем 4 пробела в начало строки

%%

int main(int argc, char **argv) {
    yylex();
    return 0;
}

