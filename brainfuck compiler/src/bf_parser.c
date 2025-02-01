#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "bf_parser.h"

char* parse_brainfuck(const char* input_file, int* token_count) {
    FILE* file = fopen(input_file, "r");
    if (!file) {
        perror("Error opening file");
        exit(1);
    }

    char buffer[65536];
    size_t length = fread(buffer, 1, sizeof(buffer), file);
    fclose(file);

    char* tokens = malloc(length + 1);
    int count = 0;

    for (size_t i = 0; i < length; i++) {
        char c = buffer[i];
        if (c == '>' || c == '<' || c == '+' || c == '-' || c == '.' || c == ',' || c == '[' || c == ']') {
            tokens[count++] = c;
        } else if (c != ' ' && c != '\n' && c != '\t') {
            fprintf(stderr, "Invalid character '%c' at position %zu\n", c, i);
            free(tokens);
            exit(1);
        }
    }

    tokens[count] = '\0';
    *token_count = count;
    return tokens;
}

void match_loops(char* tokens, int token_count, int* loop_map) {
    int* stack = malloc(token_count * sizeof(int)); // Dynamically allocate stack
    if (!stack) {
        perror("Failed to allocate memory for stack");
        exit(1);
    }

    int stack_top = 0;

    for (int i = 0; i < token_count; i++) {
        if (tokens[i] == '[') {
            stack[stack_top++] = i;
        } else if (tokens[i] == ']') {
            if (stack_top == 0) {
                fprintf(stderr, "Unmatched ']' at position %d\n", i);
                free(stack);
                exit(1);
            }
            int start = stack[--stack_top];
            loop_map[start] = i;
            loop_map[i] = start;
        }
    }

    if (stack_top > 0) {
        fprintf(stderr, "Unmatched '[' at position %d\n", stack[stack_top - 1]);
        free(stack);
        exit(1);
    }

    free(stack); // Free dynamically allocated stack
}
