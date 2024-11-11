#ifndef PARSER_H
#define PARSER_H

// Определение токенов, используемых в лексическом анализаторе.
#define TOKEN_IF       1
#define TOKEN_THEN     2
#define TOKEN_ELSE     3
#define TOKEN_END      4
#define TOKEN_FOR      5
#define TOKEN_DO       6
#define TOKEN_WHILE    7
#define TOKEN_EXPRESSION 8
#define TOKEN_ASSIGN   9
#define TOKEN_TO       10
#define TOKEN_SEMICOLON 11

// Прототипы функций, если они нужны для анализа
void test(FILE *outputFile);

#endif // PARSER_H
