#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "lex.h"

static cfbf_token cfbf_tokenize(char input);

extern cfbf_lex_state *cfbf_initialize_lexer(FILE *file, int32_t size)
{
        char c;
        uint32_t commands_length = 0;
        cfbf_lex_state *state, *temp_state;

        temp_state = malloc(sizeof(cfbf_lex_state));

        if (temp_state == NULL) {
                fprintf(stderr, "Could not allocate %lu bytes for lex_state",
                        sizeof(cfbf_lex_state));
                return NULL;
        }

        state = temp_state;
        cfbf_token commands[size];

        // Read into commands from file
        while ((c = (char)fgetc(file)) != EOF) {
                cfbf_token token = cfbf_tokenize(c);

                if (token != UNKNOWN) {
                        commands[commands_length] = token;
                        ++commands_length;
                }
        }

        // Setup initial state
        cfbf_token *commands_temp = malloc(sizeof(cfbf_token) * commands_length);

        if (commands_temp == NULL) {
                fprintf(stderr, "Could not allocate %lu bytes for commands",
                        sizeof(cfbf_token) * (uint32_t)commands_length);
                return NULL;
        }

        state->commands = commands_temp;

        memcpy(state->commands, commands, (sizeof(cfbf_token) * commands_length));
        state->command_index = 0;
        state->commands_length = commands_length;
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