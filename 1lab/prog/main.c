/*
Задание:
Классический абсолютный путь к файлу в Windows:
•	латинская буква от A до Z;
•	двоеточие;
•	обратный слеш;
•	0 или более блоков из допустимых символов, разделённых обратными слешами.

К допустимым относятся все символы, кроме «*», «|», «\», «:», «"», «<», «>», «?» и «/».

(Для поиска подстрок недопустимым символом пути следует также считать пробел « ».)
Примеры корректных строк:
C:\Windows\winmine.exe
D:\WebServer\home\site.by\www\.htaccess
Z:\autoexec.bat
N:\testfile.
X:\testfile2
*/

#include <stdio.h>
#include <string.h>
#include <ctype.h>

#define MAX_PATH_LENGTH 256

int is_valid_path(const char *path) {
    if (strlen(path) >= MAX_PATH_LENGTH) {
        return 0;
    }

    if (!isalpha(path[0]) || path[0] < 'A' || path[0] > 'Z') {
        return 0;
    }

    // Проверяем наличие ":" после буквы диска
    if (path[1] != ':') {
        return 0;
    }

    // Проверяем наличие "\" после ":"
    if (path[2] != '\\') {
        return 0;
    }

    // Проверяем остальную часть пути
    for (int i = 3; path[i] != '\0'; i++) {
    if (path[i] == '\\' && path[i+1] == '\\') {
        return 0;  // недопустимо использовать двойные обратные слеши
    }
    if (strchr("*|:\"<>?/ ", path[i])) {
        return 0;  // если символ недопустим
    }
}


    return 1;
}

int main() {
    char path[MAX_PATH_LENGTH];
    char choice;
    FILE *file = NULL;

    printf("Выберите режим ввода:\n");
    printf("1. Считывать пути из файла\n");
    printf("2. Вводить пути вручную\n");
    printf("> ");
    choice = getchar();
    getchar(); // Игнорируем символ новой строки после выбора

    if (choice == '1') {
        char filename[MAX_PATH_LENGTH];
        printf("Введите имя файла (в той же директории, что и .exe): ");
        if (fgets(filename, MAX_PATH_LENGTH, stdin) == NULL) {
            printf("Ошибка ввода имени файла.\n");
            return 1;
        }
        // Удаляем символ новой строки, если он есть
        filename[strcspn(filename, "\n")] = 0;

        // Открываем файл для чтения
        file = fopen(filename, "r");
        if (file == NULL) {
            printf("Не удалось открыть файл %s. Убедитесь, что файл находится в той же директории, что и .exe\n", filename);
            return 1;
        }

        while (fgets(path, MAX_PATH_LENGTH, file) != NULL) {
            // Удаляем символ новой строки, если он есть
            path[strcspn(path, "\n")] = 0;

            if (is_valid_path(path)) {
                printf("Путь корректен: %s\n", path);
            } else {
                printf("Путь некорректен: %s\n", path);
            }
        }

        fclose(file);
    } else if (choice == '2') {
        printf("Введите абсолютный путь Windows (или 'q' для выхода):\n");

        while (1) {
            printf("> ");
            if (fgets(path, MAX_PATH_LENGTH, stdin) == NULL) {
                printf("Ошибка ввода. Попробуйте снова.\n");
                continue; // Повторяем ввод
            }
            
            // Проверка на превышение длины строки
            if (strchr(path, '\n') == NULL) {
                printf("Строка слишком длинная. Попробуйте снова.\n");
                // Очищаем буфер
                while (getchar() != '\n');
                continue; // Повторяем ввод
            }
            
            // Удаляем символ новой строки, если он есть
            path[strcspn(path, "\n")] = 0;
            
            // Проверяем, хочет ли пользователь выйти
            if (strcmp(path, "q") == 0) {
                break;
            }
            
            if (is_valid_path(path)) {
                printf("Путь корректен.\n");
            } else {
                printf("Путь некорректен. Попробуйте снова.\n");
            }
        }
    } else {
        printf("Неверный выбор.\n");
        return 1;
    }

    printf("Программа завершена.\n");
    return 0;
}
