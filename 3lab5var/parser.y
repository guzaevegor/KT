%{
#include <stdio.h>
%}
%token IF 
%token ELSE 
%token FOR 
%token WHILE 
%token PRINT 
%token IN 
%token COLON 
%token IDENTIFIER

%%
statement:
	if_statement	
      | if_statement
      | if_else_statement
      | for_statement
      | while_statement
      | print_statement
;

if_statement: IF COLON statement { printf("Valid if statement\n"); }
;

if_else_statement: IF COLON statement ELSE COLON statement { printf("Valid if-else statement\n"); }
;

for_statement: FOR IDENTIFIER IN IDENTIFIER COLON statement { printf("Valid for statement\n"); }
;

while_statement: WHILE COLON statement { printf("Valid while statement\n"); }
;

print_statement: PRINT COLON statement { printf("Valid print statement\n"); }
;
%%
int main() {
    yyparse();
    return 0;
}

void yyerror(char *s) {
    fprintf(stderr, "%s\n", s);
}
