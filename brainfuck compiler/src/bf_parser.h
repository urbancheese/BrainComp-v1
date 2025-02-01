#ifndef BF_PARSER_H
#define BF_PARSER_H

char* parse_brainfuck(const char* input_file, int* token_count);
void match_loops(char* tokens, int token_count, int* loop_map);

#endif // BF_PARSER_H
