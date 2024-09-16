/*
Задание:
Двоичное, восьмеричное или 16-чное число в синтаксисе FASM (постфиксная форма записи). 16-чное число должно начинаться с цифры от 0 до 9.
•	регулярное выражение;
([01]+ [Bb]) | ([0-7]+ [Oo]) | ([1-9]+ [A-F]+ [a-f]+ [Hh]) | (0 [A-F]+ [a-f]+ [0-9]+ [Hh])
Классы:
Digit ::= [0-9];
Digit_0 ::= [0];
Digit_1_9 ::= [1-9];
Letters_Lower ::= [a-f];
Letters_Upper ::= [A-F];
Hex ::= [Hh];
Bin ::= [Bb];
Oct ::= [Oo];
Oct_Digit ::= [0-7];
Bin_Digit ::= [01];
Регулярное выражение:
Number ::= (Bin_Digit+ Bin) | (Oct_Digit+ Oct) | (Digit_1_9+ Letters_Upper+ Letters_Lower+ Hex) | (Digit_0+ Letters_Upper+ Letters_Lower+ Digit+ Hex)
Примеры корректных чисел:
01101011B
54212110o
51245H
0B8Ch
*/
#include <stdio.h>
#include <string.h>

// Перечисление для типов символов
typedef enum {
    CT_UNKNOWN,
    CT_DIGIT,
    CT_HEX_LETTER,
    CT_BIN_SUFFIX,
    CT_OCT_SUFFIX,
    CT_HEX_SUFFIX
} TCharType;

// Перечисление для состояний
typedef enum {
    STATE_ERROR,
    STATE_WAIT_FIRST_DIGIT,
    STATE_READ_NUMBER,
    STATE_WAIT_SUFFIX,
    STATE_READ_SUFFIX
} TState;

// Структура для конечного автомата
typedef struct {
    TState transitions[5][6];   // Матрица переходов состояний
    int is_final_state[5];      // Финальные состояния
} DFA;

// Инициализация конечного автомата
void init_dfa(DFA *dfa) {
    // Инициализация переходов
    dfa->transitions[STATE_ERROR][CT_UNKNOWN] = STATE_ERROR;
    dfa->transitions[STATE_ERROR][CT_DIGIT] = STATE_ERROR;
    dfa->transitions[STATE_ERROR][CT_HEX_LETTER] = STATE_ERROR;
    dfa->transitions[STATE_ERROR][CT_BIN_SUFFIX] = STATE_ERROR;
    dfa->transitions[STATE_ERROR][CT_OCT_SUFFIX] = STATE_ERROR;
    dfa->transitions[STATE_ERROR][CT_HEX_SUFFIX] = STATE_ERROR;

    dfa->transitions[STATE_WAIT_FIRST_DIGIT][CT_UNKNOWN] = STATE_ERROR;
    dfa->transitions[STATE_WAIT_FIRST_DIGIT][CT_DIGIT] = STATE_READ_NUMBER;
    dfa->transitions[STATE_WAIT_FIRST_DIGIT][CT_HEX_LETTER] = STATE_ERROR;
    dfa->transitions[STATE_WAIT_FIRST_DIGIT][CT_BIN_SUFFIX] = STATE_ERROR;
    dfa->transitions[STATE_WAIT_FIRST_DIGIT][CT_OCT_SUFFIX] = STATE_ERROR;
    dfa->transitions[STATE_WAIT_FIRST_DIGIT][CT_HEX_SUFFIX] = STATE_ERROR;

    dfa->transitions[STATE_READ_NUMBER][CT_UNKNOWN] = STATE_ERROR;
    dfa->transitions[STATE_READ_NUMBER][CT_DIGIT] = STATE_READ_NUMBER;
    dfa->transitions[STATE_READ_NUMBER][CT_HEX_LETTER] = STATE_READ_NUMBER;
    dfa->transitions[STATE_READ_NUMBER][CT_BIN_SUFFIX] = STATE_READ_SUFFIX;
    dfa->transitions[STATE_READ_NUMBER][CT_OCT_SUFFIX] = STATE_READ_SUFFIX;
    dfa->transitions[STATE_READ_NUMBER][CT_HEX_SUFFIX] = STATE_READ_SUFFIX;

    dfa->transitions[STATE_WAIT_SUFFIX][CT_UNKNOWN] = STATE_ERROR;
    dfa->transitions[STATE_WAIT_SUFFIX][CT_DIGIT] = STATE_ERROR;
    dfa->transitions[STATE_WAIT_SUFFIX][CT_HEX_LETTER] = STATE_ERROR;
    dfa->transitions[STATE_WAIT_SUFFIX][CT_BIN_SUFFIX] = STATE_READ_SUFFIX;
    dfa->transitions[STATE_WAIT_SUFFIX][CT_OCT_SUFFIX] = STATE_READ_SUFFIX;
    dfa->transitions[STATE_WAIT_SUFFIX][CT_HEX_SUFFIX] = STATE_READ_SUFFIX;

    dfa->transitions[STATE_READ_SUFFIX][CT_UNKNOWN] = STATE_ERROR;
    dfa->transitions[STATE_READ_SUFFIX][CT_DIGIT] = STATE_ERROR;
    dfa->transitions[STATE_READ_SUFFIX][CT_HEX_LETTER] = STATE_ERROR;
    dfa->transitions[STATE_READ_SUFFIX][CT_BIN_SUFFIX] = STATE_ERROR;
    dfa->transitions[STATE_READ_SUFFIX][CT_OCT_SUFFIX] = STATE_ERROR;
    dfa->transitions[STATE_READ_SUFFIX][CT_HEX_SUFFIX] = STATE_ERROR;

    // Инициализация финальных состояний
    dfa->is_final_state[STATE_ERROR] = 0;
    dfa->is_final_state[STATE_WAIT_FIRST_DIGIT] = 0;
    dfa->is_final_state[STATE_READ_NUMBER] = 1;
    dfa->is_final_state[STATE_WAIT_SUFFIX] = 0;
    dfa->is_final_state[STATE_READ_SUFFIX] = 1;
}

// Функция классификации символов
TCharType get_char_type(char c) {
    if (c == '0' || c == '1') return CT_DIGIT;  // для двоичного числа
    if (c >= '2' && c <= '7') return CT_DIGIT;  // для восьмеричного числа
    if (c >= '8' && c <= '9') return CT_DIGIT;  // для десятичных и шестнадцатеричных
    if (c >= 'A' && c <= 'F') return CT_HEX_LETTER;  // шестнадцатеричные буквы
    if (c >= 'a' && c <= 'f') return CT_HEX_LETTER;  // шестнадцатеричные буквы в нижнем регистре
    if (c == 'B' || c == 'b') return CT_BIN_SUFFIX;  // суффикс для двоичного числа
    if (c == 'O' || c == 'o') return CT_OCT_SUFFIX;  // суффикс для восьмеричного числа
    if (c == 'H' || c == 'h') return CT_HEX_SUFFIX;  // суффикс для шестнадцатеричного числа
    return CT_UNKNOWN;  // Любой другой символ считается неизвестным
}


// Функция для проверки строки с помощью конечного автомата
int check_string(DFA *dfa, const char *s) {
    TState state = STATE_WAIT_FIRST_DIGIT;
    int has_hex_letter = 0;  // Флаг наличия шестнадцатеричной буквы
    for (int i = 0; s[i] != '\0'; i++) {
        TCharType char_type = get_char_type(s[i]);

        // Если встречен неизвестный символ, сразу переходим в состояние ошибки
        if (char_type == CT_UNKNOWN) {
            state = STATE_ERROR;
            break;
        }

        state = dfa->transitions[state][char_type];

        // Проверяем наличие шестнадцатеричных букв для шестнадцатеричных чисел
        if (char_type == CT_HEX_LETTER) {
            has_hex_letter = 1;
        }
    }

    // Проверяем финальное состояние
    if (dfa->is_final_state[state]) {
        // Если число заканчивается на суффикс для шестнадцатеричных чисел, проверяем наличие букв
        if (state == STATE_READ_SUFFIX && has_hex_letter) {
            return 1;  // Корректное шестнадцатеричное число
        }
        return 1;  // Корректное двоичное или восьмеричное число
    }

    return 0;  // Некорректное число
}


// Функция для ручного ввода строки
void process_manual_input(DFA *dfa) {
    char s[256];

    printf("Введите строку для проверки (или 'q' для выхода):\n");
    while (1) {
        printf("> ");
        fgets(s, 256, stdin);

        // Удаляем символ новой строки
        s[strcspn(s, "\n")] = 0;

        if (strcmp(s, "q") == 0) {
            break;
        }

        if (check_string(dfa, s)) {
            printf("Строка правильная\n");
        } else {
            printf("Строка неправильная\n");
        }
    }
}

// Функция для чтения строки из файла и проверки ее с помощью конечного автомата
void process_file_input(DFA *dfa) {
    char filename[256];
    printf("Введите имя файла для чтения: ");
    fgets(filename, sizeof(filename), stdin);
    filename[strcspn(filename, "\n")] = 0;  // Удаляем символ новой строки

    FILE *file = fopen(filename, "r");  // Открываем файл для чтения
    if (!file) {
        printf("Не удалось открыть файл.\n");
        return;
    }

    char s[256];
    while (fgets(s, sizeof(s), file)) {
        // Удаляем символ новой строки
        s[strcspn(s, "\n")] = 0;

        if (check_string(dfa, s)) {
            printf("Строка '%s' правильная\n", s);
        } else {
            printf("Строка '%s' неправильная\n", s);
        }
    }

    fclose(file);  // Закрываем файл
}

// Главная функция
int main() {
    DFA dfa;
    int choice;

    // Инициализируем автомат
    init_dfa(&dfa);

    // Выбор между вводом строки вручную или чтением из файла
    printf("Выберите метод ввода:\n");
    printf("1. Ввод вручную\n");
    printf("2. Чтение из файла\n");
    printf("> ");
    scanf("%d", &choice);
    getchar();  // Очистка буфера после scanf

    if (choice == 1) {
        process_manual_input(&dfa);
    } else if (choice == 2) {
        process_file_input(&dfa);
    } else {
        printf("Неправильный выбор.\n");
    }

    return 0;
}
