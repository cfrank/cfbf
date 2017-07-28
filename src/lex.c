#include <stdio.h>
#include <stdlib.h>
#include "lex.h"

static cfbf_token cfbf_tokenize(char input);

extern int cfbf_initialize_lexer(FILE *file, int32_t size)
{
        char c;
        uint32_t index = 0;
        cfbf_lex_state state = {0};
        cfbf_token commands[size];
        while ((c = (char)fgetc(file)) != EOF) {
                cfbf_token token = cfbf_tokenize(c);

                if (token == UNKNOWN) {
                        fprintf(stderr, "Invalid token found at index: %d\n", index);
                        return EXIT_FAILURE;
                } else {
                        commands[index] = token;
                }
                ++index;
        }
        printf("%lu\n", (sizeof(commands) / sizeof(cfbf_token)));

        return 1;
}

static cfbf_token cfbf_tokenize(char input)
{
        switch (input) {
        case '>':
                return INCR_PTR;
        case '<':
                return DECR_PTR;
        case '+':
                return INCR_BYTE;
        case '-':
                return DECR_BYTE;
        case '.':
                return OUTP_BYTE;
        case ',':
                return ACCEPT_BYTE;
        case '[':
                return JMP_FWRD;
        case ']':
                return JMP_BACK;
        default:
                return UNKNOWN;
        }
}