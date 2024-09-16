/*
Задание:
Двоичное, восьмеричное или 16-чное число в синтаксисе FASM (постфиксная форма записи). 16-чное число должно начинаться с цифры от 0 до 9.
•	регулярное выражение;
([01]+B) | ([0-7]+[Oo]) | ([0-9A-F]+[Hh]) | (0[A-F0-9]+[Hh])

Примеры корректных чисел:
01101011B
54212110o
51245H
0B8Ch
*/

#include <stdio.h>
#include <string.h>

#define MAX_PATH_LENGTH 256

// Функция проверки, является ли символ цифрой
int is_digit(char ch) {
    return ch >= '0' && ch <= '9';
}

// Функция преобразования символа к верхнему регистру (для шестнадцатеричных чисел)
char to_upper(char ch) {
    if (ch >= 'a' && ch <= 'f') {
        return ch - ('a' - 'A');
    }
    return ch;
}

// Функция проверки двоичного числа
int is_valid_binary(const char *num) {
    size_t len = strlen(num);
    if (len < 2 || num[len - 1] != 'B') {
        return 0;
    }
    for (size_t i = 0; i < len - 1; i++) {
        if (num[i] != '0' && num[i] != '1') {
            return 0;
        }
    }
    return 1;
}

// Функция проверки восьмеричного числа
int is_valid_octal(const char *num) {
    size_t len = strlen(num);
    if (len < 2 || (num[len - 1] != 'o' && num[len - 1] != 'O')) {
        return 0;
    }
    for (size_t i = 0; i < len - 1; i++) {
        if (num[i] < '0' || num[i] > '7') {
            return 0;
        }
    }
    return 1;
}

// Функция проверки шестнадцатеричного числа
int is_valid_hex(const char *num) {
    size_t len = strlen(num);
    if (len < 2 || (num[len - 1] != 'H' && num[len - 1] != 'h')) {
        return 0;
    }
    if (!is_digit(num[0])) {  // Число должно начинаться с цифры
        return 0;
    }
    for (size_t i = 0; i < len - 1; i++) {
        char upper_char = to_upper(num[i]);
        if (!is_digit(num[i]) && (upper_char < 'A' || upper_char > 'F')) {
            return 0;
        }
    }
    return 1;
}

// Функция проверки корректности числа в синтаксисе FASM
int is_valid_number(const char *num) {
    return is_valid_binary(num) || is_valid_octal(num) || is_valid_hex(num);
}

// Функция для обработки ввода чисел из файла
void process_file_input() {
    char filename[MAX_PATH_LENGTH];
    char number[MAX_PATH_LENGTH];
    FILE *file = NULL;

    printf("Введите имя файла (в той же директории, что и .exe): ");
    fgets(filename, MAX_PATH_LENGTH, stdin);
    filename[strcspn(filename, "\n")] = 0;  // Удаляем символ новой строки

    file = fopen(filename, "r");
    if (file == NULL) {
        printf("Не удалось открыть файл %s. Убедитесь, что файл находится в той же директории.\n", filename);
        return;
    }

    while (fgets(number, MAX_PATH_LENGTH, file) != NULL) {
        number[strcspn(number, "\n")] = 0;  // Удаляем символ новой строки
        if (is_valid_number(number)) {
            printf("Число корректно: %s\n", number);
        } else {
            printf("Число некорректно: %s\n", number);
        }
    }

    fclose(file);
}

// Функция для ручного ввода чисел
void process_manual_input() {
    char number[MAX_PATH_LENGTH];

    printf("Введите число в синтаксисе FASM (или 'q' для выхода):\n");

    int exit_flag = 0;
    while (!exit_flag) {
        printf("> ");
        fgets(number, MAX_PATH_LENGTH, stdin);

        // Убедимся, что строка не превышает допустимую длину
        if (strchr(number, '\n') == NULL) {
            printf("Строка слишком длинная. Попробуйте снова.\n");
            // Очищаем буфер
            while (getchar() != '\n');
            continue;
        }

        // Удаляем символ новой строки, если он есть
        number[strcspn(number, "\n")] = 0;

        // Проверяем, хочет ли пользователь выйти
        if (strcmp(number, "q") == 0) {
            exit_flag = 1;
        } else {
            if (is_valid_number(number)) {
                printf("Число корректно.\n");
            } else {
                printf("Число некорректно. Попробуйте снова.\n");
            }
        }
    }
}

// Основная функция программы
int main() {
    char choice;

    printf("Выберите режим ввода:\n");
    printf("1. Считывать числа из файла\n");
    printf("2. Вводить числа вручную\n");
    printf("> ");
    choice = getchar();
    getchar();  // Игнорируем символ новой строки после выбора

    if (choice == '1') {
        process_file_input();
    } else if (choice == '2') {
        process_manual_input();
    } else {
        printf("Неверный выбор.\n");
        return 1;
    }

    printf("Программа завершена.\n");
    return 0;
}
