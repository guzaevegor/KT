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

#define MAX_PATH_LENGTH 256

// Функция проверки символа на допустимость в начале пути
int is_valid_drive_letter(char ch) {
    return ch >= 'A' && ch <= 'Z';
}

// Функция проверки, что строка не слишком длинная
int is_valid_length(const char *path) {
    return strlen(path) < MAX_PATH_LENGTH;
}

// Функция проверки основного тела пути
int is_valid_path_body(const char *path) {
    for (int i = 3; path[i] != '\0'; i++) {
        // Запрещаем двойные слеши
        if (path[i] == '\\' && path[i + 1] == '\\') {
            return 0;
        }
        // Проверяем недопустимые символы
        if (strchr("*|:\"<>?/ ", path[i])) {
            return 0;
        }
    }
    return 1;
}

// Функция полной проверки пути
int is_valid_path(const char *path) {
    if (!is_valid_length(path)) {
        return 0;
    }
    if (!is_valid_drive_letter(path[0])) {
        return 0;
    }
    if (path[1] != ':' || path[2] != '\\') {
        return 0;
    }
    return is_valid_path_body(path);
}

// Функция для чтения путей из файла
void process_file_input() {
    char filename[MAX_PATH_LENGTH];
    char path[MAX_PATH_LENGTH];
    FILE *file = NULL;

    printf("Введите имя файла (в той же директории, что и .exe): ");
    fgets(filename, MAX_PATH_LENGTH, stdin);
    filename[strcspn(filename, "\n")] = 0;  // Удаляем символ новой строки

    file = fopen(filename, "r");
    if (file == NULL) {
        printf("Не удалось открыть файл %s. Убедитесь, что файл находится в той же директории.\n", filename);
        return;
    }

    while (fgets(path, MAX_PATH_LENGTH, file) != NULL) {
        path[strcspn(path, "\n")] = 0;  // Удаляем символ новой строки
        if (is_valid_path(path)) {
            printf("Путь корректен: %s\n", path);
        } else {
            printf("Путь некорректен: %s\n", path);
        }
    }

    fclose(file);
}

// Функция для ручного ввода путей
void process_manual_input() {
    char path[MAX_PATH_LENGTH];

    printf("Введите абсолютный путь Windows (или 'q' для выхода):\n");

    int exit_flag = 0;
    while (!exit_flag) {
        printf("> ");
        fgets(path, MAX_PATH_LENGTH, stdin);

        // Убедимся, что строка не превышает допустимую длину
        if (strchr(path, '\n') == NULL) {
            printf("Строка слишком длинная. Попробуйте снова.\n");
            // Очищаем буфер
            while (getchar() != '\n');
            continue;
        }

        // Удаляем символ новой строки, если он есть
        path[strcspn(path, "\n")] = 0;

        // Проверяем, хочет ли пользователь выйти
        if (strcmp(path, "q") == 0) {
            exit_flag = 1;
        } else {
            if (is_valid_path(path)) {
                printf("Путь корректен.\n");
            } else {
                printf("Путь некорректен. Попробуйте снова.\n");
            }
        }
    }
}

// Основная функция программы
int main() {
    char choice;

    printf("Выберите режим ввода:\n");
    printf("1. Считывать пути из файла\n");
    printf("2. Вводить пути вручную\n");
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
