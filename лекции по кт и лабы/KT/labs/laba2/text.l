%option noyywrap
%array

hexdecimal [0-9a-fA-F]

%%

"%"{hexdecimal}{2}   {
        int c1 = htoi(yytext[1]), c2 = htoi(yytext[2]);                             
        printf("%c", (c1 << 4) + c2);
    }     

%%

int htoi(int c) {
    if (c >= 'a' && c <= 'z')
        return c - 'a' + 10;
    if (c >= 'A' && c <= 'Z')
        return c - 'A' + 10;
    if (c >= '0' && c <= '9')
        return c - '0';   
}

int main() {
    yylex();
    printf("\n");
    return 0;
}