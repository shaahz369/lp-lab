%{
#include <stdio.h>
#include <stdlib.h>
%}

/* Definitions */
DIGIT [0-9]
LETTER [a-zA-Z_]
ID {LETTER}({LETTER}|{DIGIT})*

INT {DIGIT}+
FLOAT {DIGIT}+"."{DIGIT}+

PREPROCESSOR \#[^\n]*

SINGLE_COMMENT "//".*

MULTI_COMMENT "/"([^]|\+[^/])\+"/"

INVALID_ID {DIGIT}+{LETTER}({LETTER}|{DIGIT})*

%%

{PREPROCESSOR} { printf("Preprocessor Directive: %s\n", yytext); }

{SINGLE_COMMENT} { /* ignore single line comment */ }

{MULTI_COMMENT} { /* ignore multi line comment */ }

"int"|"float"|"char"|"return"|"if"|"else"|"while"|"for" { printf("Keyword: %s\n", yytext); }

{FLOAT} { printf("Decimal: %s\n", yytext); }

{INT} { printf("Number: %s\n", yytext); }

{INVALID_ID} { printf("Invalid Identifier (cannot start with digit): %s\n", yytext); }

{ID} { printf("Identifier: %s\n", yytext); }

\"([^\\\"]|\\.)*\" {
char str[256];
int len = yyleng;
if (len >= 2) {
strncpy(str, yytext + 1, len - 2);
str[len - 2] = '\0';
printf("String Literal: %s\n", str);
}
}

\'([^\\\']|\\.)\' {
char ch = yytext[1];
printf("Character Constant: %c\n", ch);
}

"++"|"--"|"=="|"!="|"<="|">="|"&&"|"||" { printf("Operator: %s\n", yytext); }
"="|"+"|"-"|"*"|"/"|"%"|"!"|"<"|">" { printf("Operator: %s\n", yytext); }

";"|"{"|"}"|"("|")"|","|"["|"]" { printf("Seperator: %s\n", yytext); }

[ \t\n]+ { /* ignore whitespace */ }

. { printf("Invalid Token: %s\n", yytext); }

%%

int main(int argc, char *argv[]) {
if (argc < 2) {
printf("Usage: %s <inputfile>\n", argv[0]);
exit(1);
}

FILE *fp = fopen(argv[1], "r");
if (!fp) {
perror("Cannot open file");
exit(1);
}

yyin = fp;
yylex();
fclose(fp);

return 0;
}
