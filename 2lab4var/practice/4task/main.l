%option noyywrap
%%
\n{2,}              { printf("\n"); }  // Две и более пустых строки
^[^\n]+             { printf("    %s\n", yytext); }  // Начало строки с текстом, добавляем 4 пробела

%%

int main(int argc, char **argv) {
    yylex();
    return 0;
}

