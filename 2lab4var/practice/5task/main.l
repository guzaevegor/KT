%option noyywrap
%%
\s+([.,!?:;])   { printf("%s", yytext+1); }  // Удаляем пробел перед знаками препинания
([.,!?:;])      { printf("%s ", yytext); }  // Добавляем пробел после знаков препинания
\(\s*           { printf("( "); }            // Добавляем пробел после открывающей скобки
\s*\)           { printf(")"); }              // Удаляем пробел перед закрывающей скобкой
.               { printf("%s", yytext); }     // Выводим остальные символы

%%

int main(int argc, char **argv) {
    yylex();
    return 0;
}
