%option noyywrap
%{
#include <stdio.h>
#include <stdlib.h>

FILE *outputFile;
int in_expression = 0;
int construction_stack[100];
int stack_top = -1;
int indent_stack[100];
int indent_stack_top = -1;
int current_indent_level = 0;
int needs_indent = 0;

void push_construction() {
    construction_stack[++stack_top] = 1;
}

void pop_construction() {
    if (stack_top >= 0) stack_top--;
}

int is_in_construction() {
    return stack_top >= 0;
}

void push_indent_level(int level) {
    indent_stack[++indent_stack_top] = level;
}

int pop_indent_level() {
    if (indent_stack_top >= 0) {
        return indent_stack[indent_stack_top--];
    }
    return 0;
}

void handle_indent_dedent(const char* text) {
    int new_indent_level = 0;
    for (int i = 0; text[i] != '\0'; i++) {
        if (text[i] == '\t') {
            new_indent_level += 4;  // Настраиваемое значение ширины табуляции
        } else if (text[i] == ' ') {
            new_indent_level += 1;
        }
    }

    // Проверка флага needs_indent
    if (needs_indent) {
        push_indent_level(current_indent_level);
        current_indent_level += 4;
        fprintf(outputFile, "INDENT ");
        needs_indent = 0; // Сбрасываем флаг после добавления INDENT
    } else if (new_indent_level > current_indent_level) {
        while (new_indent_level > current_indent_level) {
            push_indent_level(current_indent_level);
            current_indent_level += 4;
            fprintf(outputFile, "INDENT ");
        }
    } else if (new_indent_level < current_indent_level) {
        while (indent_stack_top >= 0 && new_indent_level < current_indent_level) {
            current_indent_level = pop_indent_level();
            fprintf(outputFile, "DEDENT ");
        }
    }
}

%}

%%

"if"         { fprintf(outputFile, "IF "); in_expression = 0; push_construction(); }
"elif"       { fprintf(outputFile, "ELIF "); in_expression = 0; }
"else" {
    if (is_in_construction()) {
        fprintf(outputFile, "ELSE ");
        needs_indent = 1; // Устанавливаем флаг для принудительного добавления INDENT
        pop_construction(); // Завершаем предыдущую конструкцию if, т.к. else должно быть новым блоком
        push_construction(); // Начинаем новую конструкцию для else
    } else {
        fprintf(outputFile, "Error: unmatched ELSE\n");
        exit(1);
    }
    in_expression = 0; // Сброс флага выражения после else
}



"for"        { fprintf(outputFile, "FOR "); in_expression = 0; push_construction(); }
"while" {
    fprintf(outputFile, "WHILE ");
    in_expression = 0;
    push_construction();
    needs_indent = 1;  // Установка отступа для WHILE
}

"in"         { fprintf(outputFile, "IN "); in_expression = 0; }
"="          { fprintf(outputFile, "ASSIGN "); in_expression = 0; }
":"          { fprintf(outputFile, "COLON "); in_expression = 0; }
";"          { fprintf(outputFile, "SEMICOLON "); in_expression = 0; }

[a-zA-Z_][a-zA-Z0-9_]* {
    if (!in_expression) {
        fprintf(outputFile, "EXPRESSION ");
        in_expression = 1;
    }
}

[0-9]+ {
    if (!in_expression) {
        fprintf(outputFile, "EXPRESSION ");
        in_expression = 1;
    }
}

[><=!+\-*/] {
    fprintf(outputFile, "OPERATOR ");
    in_expression = 1;
}

\n {
    if (!is_in_construction() || in_expression) {
        if (!is_in_construction()) {
            fprintf(outputFile, "\n");
        }
    }
    in_expression = 0;
}

^[ \t]* {
    handle_indent_dedent(yytext);
}

%%
void emit_remaining_dedents() {
    while (indent_stack_top >= 0) {
        pop_indent_level();
        fprintf(outputFile, "DEDENT ");
    }
}

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

    // Вызовем функцию, чтобы закрыть все оставшиеся уровни отступов
    emit_remaining_dedents();

    fclose(inputFile);
    fclose(outputFile);

    // Вызываем тест, чтобы напечатать дерево разбора
    FILE *tokenFile = fopen("tokens.txt", "r");
    if (tokenFile) {
        test(tokenFile);
        fclose(tokenFile);
    } else {
        puts("Error opening tokens.txt for parsing");
    }

    return 0;
}

