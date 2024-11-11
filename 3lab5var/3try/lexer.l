%option noyywrap

%{
#include <stdio.h>
#include <stdlib.h>

FILE *outputFile;
int in_expression = 0;
int construction_stack[100];
int stack_top = -1;

void push_construction() {
    construction_stack[++stack_top] = 1;
}

void pop_construction() {
    if (stack_top >= 0) stack_top--;
}

int is_in_construction() {
    return stack_top >= 0;
}
%}

%%

"if"         { fprintf(outputFile, "IF "); in_expression = 0; push_construction(); }
"elif"       { fprintf(outputFile, "ELIF "); in_expression = 0; }
"else"       {
                if (is_in_construction()) {
                    fprintf(outputFile, "ELSE ");
                } else {
                    fprintf(outputFile, "Error: unmatched ELSE\n");
                    exit(1);
                }
                in_expression = 0;
            }
"for"        { fprintf(outputFile, "FOR "); in_expression = 0; push_construction(); }
"while"      { fprintf(outputFile, "WHILE "); in_expression = 0; push_construction(); }
"in"         { fprintf(outputFile, "IN "); in_expression = 0; }
"="          { fprintf(outputFile, "ASSIGN "); in_expression = 0; }
":"          { fprintf(outputFile, "COLON "); in_expression = 0; }
";"          { fprintf(outputFile, "SEMICOLON "); in_expression = 0; }

[a-zA-Z_][a-zA-Z0-9_]* {
    if (!in_expression) {
        fprintf(outputFile, "IDENTIFIER ");
        in_expression = 1;
    }
}

[0-9]+ {
    if (!in_expression) {
        fprintf(outputFile, "NUMBER ");
        in_expression = 1;
    }
}

[><=!+\-*/] {
    if (!in_expression) {
        fprintf(outputFile, "OPERATOR ");
        in_expression = 1;
    }
}

\n {
    if (!is_in_construction() || in_expression) {
        if (!is_in_construction()) {
            fprintf(outputFile, "\n");
        }
    }
    in_expression = 0;
}

[ \t] ;

%%

int main() {
    outputFile = fopen("tokens.txt", "w");
    if (!outputFile) {
        puts("Error opening output file");
        return 1;
    }

    FILE *inputFile = fopen("in.txt", "r");
    if (!inputFile) {
        puts("Error opening input file");
        return 1;
    }

    yyin = inputFile;
    yylex();

    fclose(inputFile);
    fclose(outputFile);

    return 0;
}

