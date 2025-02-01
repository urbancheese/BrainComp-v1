#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <keystone/keystone.h> // Keystone Engine header
#include "bf_codegen.h"

void* generate_jit_code(char* tokens, int token_count, int* loop_map) {
    ks_engine* ks;
    ks_err err;

    // Initialize Keystone for x86 architecture
    err = ks_open(KS_ARCH_X86, KS_MODE_32, &ks);
    if (err != KS_ERR_OK) {
        fprintf(stderr, "Failed to initialize Keystone Engine\n");
        exit(1);
    }

    // Allocate space for machine code
    size_t size = 65536; // Arbitrary size
    unsigned char* machine_code = malloc(size);
    size_t machine_code_size = 0;

    // Generate machine code for each Brainfuck instruction
    for (int i = 0; i < token_count; i++) {
        switch (tokens[i]) {
            case '>': {
                const char* asm_code = "inc esi";
                size_t count;
                unsigned char* encoded;
                if (ks_asm(ks, asm_code, 0, &encoded, &count, NULL) != KS_ERR_OK) {
                    fprintf(stderr, "Keystone assembly failed: %s\n", ks_strerror(ks_errno(ks)));
                    ks_close(ks);
                    exit(1);
                }
                memcpy(machine_code + machine_code_size, encoded, count);
                machine_code_size += count;
                ks_free(encoded);
                break;
            }
            case '<': {
                const char* asm_code = "dec esi";
                size_t count;
                unsigned char* encoded;
                if (ks_asm(ks, asm_code, 0, &encoded, &count, NULL) != KS_ERR_OK) {
                    fprintf(stderr, "Keystone assembly failed: %s\n", ks_strerror(ks_errno(ks)));
                    ks_close(ks);
                    exit(1);
                }
                memcpy(machine_code + machine_code_size, encoded, count);
                machine_code_size += count;
                ks_free(encoded);
                break;
            }
            // Add cases for other Brainfuck instructions...
            default:
                break;
        }
    }

    ks_close(ks); // Close Keystone Engine

    return machine_code;
}
