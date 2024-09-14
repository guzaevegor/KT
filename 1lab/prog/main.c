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
        if (path[i] == '*' || path[i] == '|' || 
            path[i] == ':' || path[i] == '"' || path[i] == '<' || 
            path[i] == '>' || path[i] == '?' || path[i] == '/' || 
            path[i] == ' ') {
            return 0;
        }
    }

    return 1;
}

int main() {
    char path[MAX_PATH_LENGTH];
    
    printf("Введите абсолютный путь Windows (или 'q' для выхода):\n");
    
    while (1) {
        printf("> ");
        if (fgets(path, MAX_PATH_LENGTH, stdin) == NULL) {
            break;
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
            printf("Путь некорректен.\n");
        }
    }
    
    printf("Программа завершена.\n");
    return 0;
}
