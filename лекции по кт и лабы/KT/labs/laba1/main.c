#include <stdio.h>
#include <string.h>

enum charType {
    ctUnknown,
    ctBigLetter,
    ctLetter,
    ctColon,
    ctSlash,
};

const int CurrentState[6][5] = {
        {0, 0, 0, 0, 0},
        {0, 2, 0, 0, 0},
        {0, 0, 0, 3, 0},
        {0, 0, 0, 0, 4},
        {0, 0, 5, 0, 0},
        {0, 0, 5, 0, 4}
};

const int isFinalState[6] = {
        0, 0, 0, 0, 0, 1
};

int GetFirstChar(int c) {
    if (c >= 'A' && c <= 'Z')
        return ctBigLetter;
    if (c == ':')
        return ctColon;
    if (c == '\\')
        return ctSlash;
    return ctUnknown;
}

int GetLetterForCheck(int c) {
    if (c == '\\')
        return ctSlash;
    if (!(c == '*' || c == '|' || c == '"' || c == '<' || c == '>' || c == '?' || c == '/' || c == ':'))
        return ctLetter;
    return ctUnknown;
}

int GetLetterForSearch(int c) {
    if (c == '\\')
        return ctSlash;
    if (!(c == '*' || c == '|' || c == '"' || c == '<' || c == '>' || c == '?' || c == '/' || c == ':' || c == ' '))
        return ctLetter;
    return ctUnknown;
}

void Check() {
    char input[100];
    gets(input);
    int p = 1, i = 0;
    while (input[i]) {
        if (p < 3)
            p = CurrentState[p][GetFirstChar(input[i])];
        else
            p = CurrentState[p][GetLetterForCheck(input[i])];
        i++;
    }
    printf("%d\n", isFinalState[p]);
}

void Search() {
    char input[100];
    gets(input);
    strcat(input, " ");
    int curr = 1, prev = 1, i = 0, first = i, last = i;
    while (input[i]) {
        if (curr == 0) {
            curr = 1;
            first = i;
        }
        if (curr < 3) {
            prev = curr;
            curr = CurrentState[curr][GetFirstChar(input[i])];
        }
        else {
            prev = curr;
            curr = CurrentState[curr][GetLetterForSearch(input[i])];
        }
        if (isFinalState[prev] == 1 && curr == 0) {
            last = i;
            for (int j = first; j < last; j++)
                printf("%c", input[j]);
            printf("\n");
        }
        i++;
    }
}

int main() {
    //Check(); // 0 - не соответствует, 1 - соответствует
    Search();
    return 0;
}
