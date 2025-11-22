%{
#include <stdio.h>
#include <stdlib.h>

int count_a = 0;
int count_b = 0;
int yylex(void);
int yyerror(char *s);
%}

%token A B EOL

%%
input:
      /* empty */
    | input line
    ;

line:
    sequence EOL {
        if (count_a == count_b && count_a >= 1)
            printf("Accepted\n");
        else
            printf("Rejected\n");
        count_a = 0;
        count_b = 0;
    }
    | error EOL {     // On syntax error, consume until EOL and print Rejected
        printf("Rejected\n");
        count_a = 0;
        count_b = 0;
    }
    ;

sequence:
      a_sequence b_sequence
    ;

a_sequence:
      A { count_a++; }
    | a_sequence A { count_a++; }
    ;

b_sequence:
      B { count_b++; }
    | b_sequence B { count_b++; }
    ;
%%

// Lexer embedded in YACC
int yylex(void) {
    int c = getchar();
    switch(c) {
        case 'a': return A;
        case 'b': return B;
        case '\n': return EOL;
        case EOF: return 0;
        default:
            // Ignore invalid chars silently, or optionally print a message
            return yylex();
    }
}

// yyerror no longer prints anything
int yyerror(char *s) {
    // suppress error messages here
    return 0;
}

int main() {
    printf("Enter strings (a^n b^n, n>=1):\n");
    yyparse();
    return 0;
}