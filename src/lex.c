#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "lex.h"

static cfbf_token cfbf_tokenize(char input);

extern cfbf_lex_state *cfbf_initialize_lexer(FILE *file, int32_t size)
{
        char c;
        uint32_t command_length = 0;
        cfbf_lex_state *state = malloc(sizeof(cfbf_lex_state));
        cfbf_token commands[size];

        // Read into commands from file
        while ((c = (char)fgetc(file)) != EOF) {
                cfbf_token token = cfbf_tokenize(c);

                if (token != UNKNOWN) {
                        commands[command_length] = token;
                        ++command_length;
                }
        }

        // Setup initial state
        state->commands = malloc(sizeof(cfbf_token) * command_length);

        if (state->commands == NULL) {
                fprintf(stderr, "Could not allocate %lu bytes for commands",
                        sizeof(cfbf_token) * (uint32_t)size);
        }

        memcpy(state->commands, commands, (sizeof(cfbf_token) * command_length));
        state->command_index = 0;
        state->jmp_index = 0;
        state->loop_index = 0;
        state->loop_closed = false;

        return state;
}

extern void cfbf_free_lex_state(cfbf_lex_state *state)
{
        if (state->commands != NULL) {
                free(state->commands);
        }

        if (state != NULL) {
                free(state);
        }
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