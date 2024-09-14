%option main

PRINT   println!
COND    [0-9]
NEXT    \n|\r\n|\t
LITER   \".*\"
SMCOL   ;

%%

{NEXT}  { printf("0\n"); }

[ ]     { printf("1\n"); }

fn      { printf("2\n"); }

main    { printf("3\n"); }

[(]     { printf("4\n"); }

[)]     { printf("5\n"); }

[{]     { printf("6\n"); }

[}]     { printf("7\n"); }

{PRINT} { printf("8\n"); }

if      { printf("9\n"); }

{COND}  { printf("10\n"); }

else    { printf("11\n"); }

{LITER} { printf("12\n"); }

{SMCOL} { printf("13\n"); }

while   { printf("14\n"); }

%%