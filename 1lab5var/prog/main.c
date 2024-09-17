/*
Задание:
Двоичное, восьмеричное или 16-чное число в синтаксисе FASM (постфиксная форма записи). 16-чное число должно начинаться с цифры от 0 до 9.
•	регулярное выражение;


[0–9] ([0–9] | [A–F])* ('b' | 'O' | 'o' | 'H' | 'h')

Классы символов:

DecDigit ::= [0–9]
HexDigit ::= [A–F]
Letter ::= 'b' | 'O' | 'o' | 'H' | 'h'

Результирующее регулярное выражение для строк:

Number ::= DecDigit (DecDigit | HexDigit)* Letter

*/
#include <stdio.h>
#include <string.h>

// тип перечисления включает классы символов
enum charType {
    ctUnknown,
    ctDecDigit,
    ctHexDigit,
    ctLetter
};

// двумерный массив автоматических переходов
const int Transitions[4][4] = {
        {0, 0, 0, 0},
        {0, 2, 0, 0},
        {0, 2, 2, 3},
        {0, 0, 0, 0}
};

const int isFinalState[4] = {
        0, 0, 0, 1
};

// функция для получения класса символа
int getCharType(char chr) {
    if (chr >= '0' && chr <= '9')
        return ctDecDigit;
    if (chr >= 'A' && chr <= 'F')
        return ctHexDigit;
    if (chr == 'b' || chr == 'O' || chr == 'o' || chr == 'h' || chr == 'H' )
        return ctLetter;
    return ctUnknown;
}

// функция для сброса автомата
void reset(int *state, int *startPos, int *pos, int mas[]) {
    *state = 1;
    *startPos = *pos + 1;
    mas[0] = 1;
    mas[1] = 1;
}

// главная функция для поиска чисел
void searchString(char str[]) {
    int state = 1;
    int i = 0;
    int startPos = 0;
    int isCorrect[] = {1, 1};
    int count = 0;
    int j;
    while (i < strlen(str)) {
        state = Transitions[state][getCharType(str[i])];
        if (isFinalState[state] != 1 && str[i] != '0' && str[i] != '1')
            isCorrect[0] = 0;
        if (isFinalState[state] != 1 && (getCharType(str[i]) != 1 || str[i] > '7'))
            isCorrect[1] = 0;
        if (isFinalState[state] == 1) {
            if (str[i] == 'b' && isCorrect[0] == 0) {
                j = i - 1;
                while (str[j] == '0' || str[j] == '1')
                    j--;
                j++;
                if (j != i) {
                    while (j <= i) {
                        putchar(str[j]);
                        j++;
                    }
                    count++;
                    printf("\n");
                }
                reset(&state, &startPos, &i, isCorrect);
            } else {
                if ((str[i] == 'o' || str[i] == 'O') && isCorrect[1] == 0) {
                    j = i - 1;
                    while (str[j] >= '0' && str[j] <= '7')
                        j--;
                    j++;
                    if (j != i) {
                        while (j <= i) {
                            putchar(str[j]);
                            j++;
                        }
                        count++;
                        printf("\n");
                    }
                    reset(&state, &startPos, &i, isCorrect);
                } else {
                    count++;
                    for (j = startPos; j <= i; j++)
                        putchar(str[j]);
                    printf("\n");
                    reset(&state, &startPos, &i, isCorrect);
                }
            }
        }
        if (state == 0) {
            reset(&state, &startPos, &i, isCorrect);
        }
        i++;
    }
    printf("Найдено чисел: %d\n", count);
}


// функция для поиска всех подстрок и вывода корректных
void printValidSubstrings(char *s) {
    printf("Корректные подстроки: \n");
    char substr[100];
    for (int i = 0; i < strlen(s); i++) {
        for (int j = i + 1; j <= strlen(s); j++) {
            strncpy(substr, s + i, j - i);
            substr[j - i] = '\0';
            printf("Проверка подстроки: %s\n", substr);
            searchString(substr);
        }
    }
}
int main() {
    FILE *out;
    char input[100];
    char filePath[200]; // Буфер для пути к файлу
    char choice;

    do {
        printf("\nВыберите действие:\n");
        printf("0 - ввод из файла\n");
        printf("1 - ввод с клавиатуры\n");
        printf("2 - поиск подстрок\n");
        printf("3 - выход из программы\n");
        printf("> ");

        choice = getchar();
        getchar(); // чтобы захватить символ новой строки после getchar()

        switch (choice) {
            case '0': {
                printf("Введите путь к файлу:\n> ");
                scanf("%s", filePath);

                out = fopen(filePath, "r");
                if (out == NULL) {
                    printf("Ошибка: не удалось открыть файл.\n");
                } else {
                    fscanf(out, "%s", input);
                    printf("Строка из файла: %s\n", input);
                    searchString(input);
                    fclose(out);
                }
                break;
            }

            case '1': {
                printf("Введите строку:\n> ");
                scanf("%s", input);
                searchString(input);
                break;
            }

            case '2': {
                printf("Введите строку для поиска подстрок:\n> ");
                scanf("%s", input);
                printValidSubstrings(input);
                break;
            }

            case '3': {
                printf("Завершение программы. Благодарю за использование.\n");
                return 0;
            }

            default: {
                printf("Ошибка. Введите число заново.\n");
                break;
            }
        }
        
        // Очистка буфера для нового ввода
        getchar(); 

    } while (choice != '3'); // Цикл продолжается, пока не выбран выход ('3')

    return 0;
}