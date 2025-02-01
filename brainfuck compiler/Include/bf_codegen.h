#ifndef BF_CODEGEN_H
#define BF_CODEGEN_H

#include <stdio.h>

// Function to generate assembly code from Brainfuck tokens
void generate_assembly(char* tokens, int token_count, int* loop_map, const char* output_file);

// Function to generate JIT machine code from Brainfuck tokens
void* generate_jit_code(char* tokens, int token_count, int* loop_map);

#endif // BF_CODEGEN_H