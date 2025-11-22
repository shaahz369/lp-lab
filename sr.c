#include <stdio.h>
#include <string.h>
#include <ctype.h>
#include <stdlib.h>

char input[100][10];        // tokens (id, +, -, (, ), $ or invalid char)
int isValidToken[100];      // 1 if valid, 0 if invalid
int tokenCount = 0;
char stack[100][10];
int top = -1;
int i = 0;

// ---------- Helper for aligned printing ----------
void printStep(const char *action) {
    char stackStr[200] = "$";
    char inputStr[200] = "";

    for (int j = 0; j <= top; j++) strcat(stackStr, stack[j]);
    for (int j = i; j < tokenCount; j++) strcat(inputStr, input[j]);
    strcat(inputStr, "$");

    printf("%-20s %-20s %-20s\n", stackStr, inputStr, action);
}

void push(char *c) {
    strcpy(stack[++top], c);
}

void pop(int n) {
    top -= n;
}

int check() {
    // Rule: S → id
    if (top >= 0 && strcmp(stack[top], "id") == 0) {
        pop(1);
        push("S");
        printStep("Reduced: S -> id");
        return 1;
    }

    // Rule: S → (S)
    if (top >= 2 && strcmp(stack[top], ")") == 0 && strcmp(stack[top - 2], "(") == 0 && strcmp(stack[top - 1], "S") == 0) {
        pop(3);
        push("S");
        printStep("Reduced: S -> (S)");
        return 1;
    }

    // Rule: S → S+S
    if (top >= 2 && strcmp(stack[top], "S") == 0 && strcmp(stack[top - 1], "+") == 0 && strcmp(stack[top - 2], "S") == 0) {
        pop(3);
        push("S");
        printStep("Reduced: S -> S+S");
        return 1;
    }

    // Rule: S → S-S
    if (top >= 2 && strcmp(stack[top], "S") == 0 && strcmp(stack[top - 1], "-") == 0 && strcmp(stack[top - 2], "S") == 0) {
        pop(3);
        push("S"); 
        printStep("Reduced: S -> S-S");
        return 1;
    }

    return 0;
}

// Function to tokenize the input and add them to the input array after removing whitespaces
void tokenize(char *expr) {
    int len = strlen(expr);
    for (int k = 0; k < len;) {
        if (isspace(expr[k])) {
            k++;
        } else if (expr[k] == '+' || expr[k] == '-' || expr[k] == '(' || expr[k] == ')') {
            input[tokenCount][0] = expr[k];
            input[tokenCount][1] = '\0';
            isValidToken[tokenCount++] = 1;
            k++;
        } else if (expr[k] == 'i' && expr[k + 1] == 'd') {
            strcpy(input[tokenCount], "id");
            isValidToken[tokenCount++] = 1;
            k += 2;
        } else {
            // Store the invalid character itself
            input[tokenCount][0] = expr[k];
            input[tokenCount][1] = '\0';
            isValidToken[tokenCount++] = 0;
            k++;
        }
    }
}

int main() {
    char expr[100];
    printf("Enter input string : ");
    fgets(expr, sizeof(expr), stdin);
    
    size_t len = strlen(expr);
    if (len > 0 && expr[len - 1] == '\n') {
        expr[len - 1] = '\0';
    }
    if(expr[0] == '\0'){
      printf("Error: Empty string..!\n");
      exit(0);
    }

    tokenize(expr);
    printf("======================== PARSE TABLE ========================\n------------------------------------------------------------\n");
    printf("\n%-20s %-20s %-20s\n------------------------------------------------------------\n", "Stack", "Input", "Action");

    // 🔹 Print initial empty stack
    printStep("Initial");

    while (i < tokenCount) {
        // ❗ If invalid token, stop and show error
        if (!isValidToken[i]) {
            printStep("Syntax error");
            printf("\nSyntax error: Unexpected token '%s' at position %d\n", input[i], i+1);
            return 1;
        }

        push(input[i]);
        i++;
        printStep("Shift");

        while (check()); // keep reducing after each shift
    }

    // Final reductions after input is consumed
    while (check());

    if (top == 0 && strcmp(stack[0], "S") == 0) {
        // 🔹 Pop final S to make stack empty again
        pop(1);
        printStep("Accept");
        printf("\nString Accepted!\n");
    } else {
        printf("\nString Rejected!\n");
    }

    return 0;
}