#ifndef PARSER_H
#define PARSER_H

// Определение токенов, используемых в лексическом анализаторе.
#define TOKEN_IF       1
#define TOKEN_ELIF     2
#define TOKEN_ELSE     3
#define TOKEN_FOR      4
#define TOKEN_WHILE    5
#define TOKEN_EXPRESSION 8
#define TOKEN_ASSIGN   6
#define TOKEN_IN       7
#define TOKEN_SEMICOLON 8
#define TOKEN_INDENT 9
#define TOKEN_DEDENT 10


// Прототипы функций, если они нужны для анализа
void test(FILE *outputFile);

#endif // PARSER_H
