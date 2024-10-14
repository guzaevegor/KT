%option noyywrap

%{
#include <stdio.h>
#include <stdlib.h>

extern FILE *yyin;
extern FILE *yyout;
extern int yylex();
int isOpenOutFile = 0;
int line_number = 1;
%}

%%
^.*     { printf("%d: %s \n", line_number++, yytext); }
\n      { }

%%


int main() {
    char inputFilePath[256];
    char tempFilePath[256];
    char outputFilePath[256];
    int isOpenOutFile = 0;

    // Получаем путь к входному файлу от пользователя
    printf("Enter the path to the input file: ");
    scanf("%255s", inputFilePath);

    // Открываем входной файл
    FILE *inputFile = fopen(inputFilePath, "r");
    if (inputFile == NULL) {
        perror("Error opening input file");
        return 1;
    }

    // Получаем путь к временному файлу от пользователя
    printf("Enter the path to the temporary file: ");
    scanf("%255s", tempFilePath);

    // Открываем временный файл для записи
    FILE *tempFile = fopen(tempFilePath, "w+");
    if (tempFile == NULL) {
        perror("Error opening temp file");
        fclose(inputFile);
        return 1;
    }

    // Копируем содержимое из inputFile в tempFile
    char buffer[256];
    while (fgets(buffer, sizeof(buffer), inputFile)) {
        fprintf(tempFile, "%s", buffer);
    }
    rewind(tempFile);
    yyin = tempFile;

    // Запрашиваем у пользователя, куда выводить результат
    char choice;
    printf("Where do you want to output the result? (c for console, f for file): ");
    scanf(" %c", &choice);

    if (choice == 'f' || choice == 'F') {
        // Получаем путь к выходному файлу от пользователя
        printf("Enter the path to the output file: ");
        scanf("%255s", outputFilePath);

        FILE *outputFile = fopen(outputFilePath, "w");
        if (outputFile == NULL) {
            perror("Error opening output file");
            fclose(inputFile);
            fclose(tempFile);
            return 1;
        }
        yyout = outputFile;
        isOpenOutFile = 1;
    } else if (choice == 'c' || choice == 'C') {
        yyout = stdout;
    } else {
        printf("Incorrect input!");
        fclose(inputFile);
        fclose(tempFile);
        return 1;
    }

    // Запускаем лексический анализ
    yylex();

    // Закрываем файлы
    fclose(inputFile);
    fclose(tempFile);
    if (isOpenOutFile) {
        fclose(yyout);
    }

    return 0;
}

