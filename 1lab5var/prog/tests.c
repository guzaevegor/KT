#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
#include <windows.h>

#define STATES_COUNT 6
#define CHAR_TYPES_COUNT 6

enum CharType {
    UNKNOWN,
    DIGIT_01,           // Цифры 0 и 1 для двоичных чисел
    DIGIT_07,           // Цифры от 0 до 7 для восьмеричных чисел
    DIGIT_09,           // Цифры от 0 до 9 для шестнадцатеричных чисел
    HEX_LETTER,         // Буквы от A до F для шестнадцатеричных чисел
    POSTFIX             // Постфикс (b, o, h)
};

const int TRANSITIONS[STATES_COUNT][CHAR_TYPES_COUNT] = {
    // UNKNOWN, DIGIT_01, DIGIT_07, DIGIT_09, HEX_LETTER, POSTFIX
    {0, 1, 1, 1, 0, 0},   // State 0: initial
    {0, 3, 4, 5, 2, 0},   // State 1: первая цифра
    {0, 2, 2, 2, 2, 5},   // State 2: шестнадцатеричное число (до постфикса h)
    {0, 3, 0, 0, 0, 3},   // State 3: двоичное число (до постфикса b)
    {0, 0, 4, 0, 0, 4},   // State 4: восьмеричное число (до постфикса o)
    {0, 0, 0, 0, 0, 0}    // State 5: шестнадцатеричное число (после постфикса h)
};

const bool IS_FINAL_STATE[STATES_COUNT] = {false, false, false, true, true, true};

enum CharType getCharType(char c, bool firstChar) {
    if (c == '0' || c == '1')
        return DIGIT_01;

    if (c >= '0' && c <= '7')
        return DIGIT_07;

    if (c >= '0' && c <= '9')
        return DIGIT_09;

    if (c >= 'A' && c <= 'F')
        return HEX_LETTER;

    if (c == 'b' || c == 'o' || c == 'h')
        return POSTFIX;

    return UNKNOWN;
}

bool isValidString(char *s) {
    int state = 0;
    int i = 0;
    bool firstChar = true;

    while (state != 0 && s[i] != '\0') {
        enum CharType type = getCharType(s[i], firstChar);
        state = TRANSITIONS[state][type];
        firstChar = false;
        i++;
    }

    return IS_FINAL_STATE[state];
}

void printValidSubstrings(char *s) {
    int len = strlen(s);
    for (int i = 0; i < len; i++) {
        for (int j = i; j < len; j++) {
            char substr[256];
            strncpy(substr, &s[i], j - i + 1);
            substr[j - i + 1] = '\0';

            if (isValidString(substr))
                printf("%s\n", substr);
        }
    }
}

int main() {
    SetConsoleOutputCP(CP_UTF8);

    int action, inputType;
    char buffer[256];

    while (1) {
        printf("Выберите действие:\n1. Проверка числа на правильность\n2. Поиск всех подстрок\n3. Выход\n");

        if (scanf("%d", &action) != 1 || (action != 1 && action != 2 && action != 3)) {
            printf("Неверное действие. Повторите попытку\n");
            while (getchar() != '\n'); // Очищаем буфер
            continue;
        }

        if (action == 3)
            break;

        printf("Выберите способ ввода:\n1. Консоль\n2. Файл\n");

        if (scanf("%d", &inputType) != 1 || (inputType != 1 && inputType != 2)) {
            printf("Неверный способ ввода. Повторите попытку\n");
            while (getchar() != '\n'); // Очищаем буфер
            continue;
        }

        FILE *file = NULL;
        char *s = NULL;

        if (inputType == 1) {
            printf("Введите строку:\n");
            getchar(); // Очищаем буфер после ввода действия
            fgets(buffer, sizeof(buffer), stdin);
            buffer[strcspn(buffer, "\n")] = '\0'; // Удаляем символ новой строки
            s = buffer;
        } else if (inputType == 2) {
            char filePath[256];
            printf("Введите путь к файлу:\n");
            getchar(); // Очищаем буфер

            while (1) {
                fgets(filePath, sizeof(filePath), stdin);
                filePath[strcspn(filePath, "\n")] = '\0'; // Удаляем символ новой строки
                file = fopen(filePath, "r");
                if (file != NULL)
                    break;
                printf("Невозможно открыть файл. Повторите попытку\n");
            }

            fgets(buffer, sizeof(buffer), file);
            buffer[strcspn(buffer, "\n")] = '\0'; // Удаляем символ новой строки
            fclose(file);
            s = buffer;
        }

        if (action == 1) {
            if (isValidString(s))
                printf("Число правильное\n\n");
            else
                printf("Число неправильное\n\n");
        } else if (action == 2) {
            printf("Правильные подстроки:\n");
            printValidSubstrings(s);
            printf("\n");
        }
    }

    return 0;
}
