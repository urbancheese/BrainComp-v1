#ifndef BF_PARSER_H
#define BF_PARSER_H

#include <stdio.h>

// Function to parse Brainfuck code from a file and return a filtered list of tokens
char* parse_brainfuck(const char* input_file, int* token_count);

// Function to match '[' and ']' loops and populate the loop_map array
void match_loops(char* tokens, int token_count, int* loop_map);

#endif // BF_PARSER_H