#include <stdio.h>
#include <stdbool.h>
#include <stdlib.h>
#include <string.h>
#include "brainfuck.h"
#include "stack.h"

static const uint32_t TAPE_SIZE = 0x7530;

static cfbf_token cfbf_tokenize(char input);
static bool cfbf_generate_jumps(cfbf_state *state);

extern cfbf_state *cfbf_initialize_state(FILE *file, int32_t size)
{
        char c;
        uint32_t commands_length = 0;
        cfbf_state *state;

        // Initialize a state object
        state = malloc(sizeof(cfbf_state));

        if (state == NULL) {
                fprintf(stderr, "Could not allocate %lu bytes for cfbf_state",
                        sizeof(cfbf_state));
                return NULL;
        }

        // Initialize a commands array on the stack
        cfbf_command commands[size];

        while ((c = (char)fgetc(file)) != EOF) {
                cfbf_token token = cfbf_tokenize(c);

                if (token != UNKNOWN) {
                        commands[commands_length].token = token;
                        commands[commands_length].jmp_ptr = -1;
                        ++commands_length;
                }
        }

        // Allocate just enough memory to store all the commands
        state->commands = malloc(sizeof(cfbf_command) * commands_length);

        if (state->commands == NULL) {
                fprintf(stderr, "Could not allocate %lu bytes for commands array",
                        sizeof(cfbf_command) * commands_length);
                return NULL;
        }

        // Copy commands array to state
        memcpy(state->commands, commands, (sizeof(cfbf_command) * commands_length));
        state->commands_length = commands_length;

        // Set up tape
        state->tape = malloc(sizeof(uint8_t) * TAPE_SIZE);

        if (state->tape == NULL) {
                fprintf(stderr, "Could not allocate %lu bytes for tape",
                        sizeof(uint8_t) * TAPE_SIZE);
                return NULL;
        }

        // Initialize tape with 0's
        memset(state->tape, 0, sizeof(uint8_t) * TAPE_SIZE);

        state->head = 0;

        return state;
}

static bool cfbf_generate_jumps(cfbf_state *state)
{
        cfbf_stack *stack = cfbf_create_stack();

        for (size_t i = 0; i < state->commands_length; ++i) {
                if (state->commands[i].token == JMP_FWRD) {
                        cfbf_stack_push(stack, (uint32_t)i);
                } else if (state->commands[i].token == JMP_BACK) {
                        uint32_t jmp_fwrd;
                        if (cfbf_stack_pop(stack, &jmp_fwrd)) {
                                state->commands[jmp_fwrd].jmp_ptr = (int32_t)i;
                                state->commands[i].jmp_ptr = (int32_t)jmp_fwrd;
                        } else {
                                fprintf(stderr, "Unopened close bracket found!\n");
                                goto err;
                        }
                }
        }

        if (!cfbf_stack_is_empty(stack)) {
                fprintf(stderr, "Unclosed open bracket found!\n");
                goto err;
        }

        cfbf_destroy_stack(stack);
        return true;

err:
        cfbf_destroy_stack(stack);
        return false;
}

extern bool cfbf_run_commands(cfbf_state *state)
{
        uint32_t code_ptr = 0;

        if (!cfbf_generate_jumps(state)) {
                // Unbalanced brackets. Return error
                return false;
        }

        while (code_ptr < state->commands_length) {
                switch (state->commands[code_ptr].token) {
                case INCR_PTR:
                        ++state->head;
                        break;
                case DECR_PTR:
                        --state->head;
                        break;
                case INCR_BYTE:
                        ++state->tape[state->head];
                        break;
                case DECR_BYTE:
                        --state->tape[state->head];
                        break;
                case OUTP_BYTE:
                        putchar(state->tape[state->head]);
                        break;
                case ACCEPT_BYTE:
                        state->tape[state->head] = (uint8_t)getchar();
                        break;
                case JMP_FWRD:
                        if (state->tape[state->head] == 0) {
                                code_ptr = (uint32_t)state->commands[code_ptr].jmp_ptr;
                        }
                        break;
                case JMP_BACK:
                        if (state->tape[state->head] != 0) {
                                code_ptr = (uint32_t)state->commands[code_ptr].jmp_ptr;
                        }
                        break;
                default:
                        break;
                }

                ++code_ptr;
        }

        return true;
}

extern void cfbf_destroy_state(cfbf_state *state)
{
        if (state->commands != NULL) {
                free(state->commands);
        }

        if (state->tape != NULL) {
                free(state->tape);
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