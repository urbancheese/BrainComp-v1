#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "bf_parser.h"
#include "bf_codegen.h"

void print_usage(const char* program_name) {
    fprintf(stderr, "Usage: %s <input_file> <output_file> [jit_mode]\n", program_name);
    fprintf(stderr, "  input_file: Path to the Brainfuck source file\n");
    fprintf(stderr, "  output_file: Path to save the generated assembly (ignored in JIT mode)\n");
    fprintf(stderr, "  jit_mode: 0 for assembly generation, 1 for JIT execution\n");
}

int main(int argc, char* argv[]) {
    if (argc < 4) {
        print_usage(argv[0]);
        return 1;
    }

    const char* input_file = argv[1];
    const char* output_file = argv[2];
    int jit_mode = atoi(argv[3]);

    if (jit_mode != 0 && jit_mode != 1) {
        fprintf(stderr, "Error: jit_mode must be 0 (assembly) or 1 (JIT).\n");
        print_usage(argv[0]);
        return 1;
    }

    int token_count;
    char* tokens = parse_brainfuck(input_file, &token_count);

    int* loop_map = malloc(token_count * sizeof(int));
    if (!loop_map) {
        perror("Failed to allocate memory for loop_map");
        free(tokens);
        return 1;
    }
    match_loops(tokens, token_count, loop_map);

    if (jit_mode == 0) {
        generate_assembly(tokens, token_count, loop_map, output_file);
        printf("Assembly code written to %s\n", output_file);
    } else {
        void* machine_code = generate_jit_code(tokens, token_count, loop_map);
        if (!machine_code) {
            fprintf(stderr, "Failed to generate JIT code.\n");
            free(tokens);
            free(loop_map);
            return 1;
        }
        printf("Executing JIT-compiled Brainfuck program...\n");
        execute_jit(machine_code);
    }

    free(tokens);
    free(loop_map);

    return 0;
}
