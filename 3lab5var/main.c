#include <stdio.h>
#include <stdbool.h>

#define MAX 500

bool operator();
bool println();
bool oif();
bool owhile();
bool block();

typedef enum Types {
    NEXT = 0,
    SPACE,
    FN,
    MAIN,
    TAB,
    OPEN_BRACKET,
    CLOSE_BRACKET,
    PRINTLN,
    IF,
    CONDITION,
    ELSE,
    LITTER,
    SEMICOLON,
    WHILE,
} lexeme;

int next = 0, count = 0;
int token[MAX];

bool term(const lexeme expected) {
    return expected == token[next++];
}

bool curr(const lexeme expected) {
    return expected == token[next];
}

bool start() {
    bool res = true;
    if (!term(FN)) res = false;
    if (!term(MAIN)) res = false;
    if (!term(OPEN_BRACKET)) res = false;
    if (!term(CLOSE_BRACKET)) res = false;
    if (!block()) res = false;
    return res;
}

bool operator() {
    int save = next;
    return (next = save, println()) ||
            (next = save, oif()) ||
            (next = save, owhile()) ||
            (next = save, block());
}

bool println() {
    return term(PRINTLN) && term(OPEN_BRACKET) && term(LITTER) && term(CLOSE_BRACKET);
}

bool oif() {
    int save = next;
    return (next = save, term(IF) && term(CONDITION) && block() && term(ELSE) && block())
    || (next = save, term(IF) && term(CONDITION) && block());
}

bool owhile() {
    int save = next;
    return (next = save, term(WHILE) && term(CONDITION) && block());
}

bool block() {
    int initial_indent = 0;
    bool res = true;
    if (curr(TAB)) {
        initial_indent = token[next++];
    }

    while (curr(TAB) == initial_indent && res) {
        if (!operator()) {
            res = false;
        }
    }

    return res;
}

void read() {
    int temp;
    FILE *f;
    f = fopen("/home/stepanio/projects/bsuir/2-sem/KT/3lab5var/out.txt", "r+t");
    while (fscanf(f, "%d", &temp) == 1) {
        if ((temp != SPACE) && (temp != NEXT)) {
            token[count++] = temp;
        }
    }
    fclose(f);
}

int main() {
    read();
    printf(start() ? "accept\n" : "reject\n");
    return 0;
}
