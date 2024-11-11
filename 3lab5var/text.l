%{
#include <stdio.h>
int curr_indent = 0;
%}


%%

\n|\r\n         { printf("NEWLINE\n"); }

[ ]+            { int new_indent = yyleng; 
                  if (new_indent > curr_indent) { 
                      printf("INDENT\n"); 
                      curr_indent = new_indent;
                  } else if (new_indent < curr_indent) { 
                      printf("DEDENT\n"); 
                      curr_indent = new_indent;
                  } 
                }

def             { printf("FN_DEF\n"); }

main            { printf("MAIN\n"); }

[(]             { printf("OPEN_PAREN\n"); }

[)]             { printf("CLOSE_PAREN\n"); }

[:]             { printf("COLON\n"); }

print           { printf("PRINT\n"); }    /* PRINT изменено на текст, не используем имя шаблона */

if              { printf("IF\n"); }

[0-9]+          { printf("CONDITION\n"); }  /* COND изменено на текст, не используем имя шаблона */

else            { printf("ELSE\n"); }

\".*?\"         { printf("LITERAL\n"); }   /* LITER изменено на текст, не используем имя шаблона */

while           { printf("WHILE\n"); }

%%
