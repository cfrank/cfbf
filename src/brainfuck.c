#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "brainfuck.h"

static const uint32_t TAPE_SIZE = 0x7530;
static const uint32_t BRANCH_TABLE_SIZE = 0x32;

static cfbf_token cfbf_tokenize(char input);
static cfbf_branch_table *cfbf_initialize_branch_table(void);
static int cfbf_create_branch_table(cfbf_state *state, cfbf_branch_table *bt);

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
        cfbf_token commands[size];

        while ((c = (char)fgetc(file)) != EOF) {
                cfbf_token token = cfbf_tokenize(c);

                if (token != UNKNOWN) {
                        commands[commands_length] = token;
                        ++commands_length;
                }
        }

        // Allocate just enough memory to store all the commands
        state->commands = malloc(sizeof(cfbf_token) * commands_length);

        if (state->commands == NULL) {
                fprintf(stderr, "Could not allocate %lu bytes for commands array",
                        sizeof(cfbf_token) * commands_length);
                return NULL;
        }

        // Copy commands array to state
        memcpy(state->commands, commands, (sizeof(cfbf_token) * commands_length));
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

static cfbf_branch_table *cfbf_initialize_branch_table(void)
{
        // Allocate branch table
        cfbf_branch_table *bt = malloc(sizeof(cfbf_branch_table));

        if (bt == NULL) {
                fprintf(stderr, "Could not allocate %lu bytes for branch table",
                        sizeof(cfbf_branch_table));
                return NULL;
        }

        // Allocate some room for indicies
        bt->table = malloc(sizeof(uint32_t) * BRANCH_TABLE_SIZE);

        if (bt->table == NULL) {
                fprintf(stderr, "Could not allocate %lu bytes for branch table \
                                indicies",
                        sizeof(uint32_t) * BRANCH_TABLE_SIZE);
                return NULL;
        }

        bt->index = 0;
        bt->size = BRANCH_TABLE_SIZE;

        return bt;
}

static int cfbf_create_branch_table(cfbf_state *state, cfbf_branch_table *bt)
{
        for (uint32_t i = 0; i < state->commands_length; ++i) {
                // The table is not large enough to add any more indicies we
                // need to expand it.
                if (bt->index >= bt->size) {
                        printf("%s\n", "Reallocating memory");
                        uint32_t *temp_table = realloc(bt->table, bt->size * 2);

                        if (temp_table == NULL) {
                                fprintf(stderr, "Failed to reallocate %lu bytes\
                                                for branch table indicies",
                                        sizeof(uint32_t) * (bt->size * 2));
                                return 1;
                        }

                        bt->table = temp_table;
                        bt->size = bt->size * 2;
                }

                if (state->commands[i] == JMP_FWRD) {
                        printf("Adding %d to array index %d\n", i, bt->index);
                        bt->table[bt->index] = i;
                        ++bt->index;
                }
                printf("%d\n", bt->table[bt->index]);
        }

        return 0;
}

static void cfbf_destroy_branch_table(cfbf_branch_table *bt)
{
        if (bt->table != NULL) {
                free(bt->table);
        }

        if (bt != NULL) {
                free(bt);
        }
}

extern int cfbf_run_commands(cfbf_state *state)
{
        cfbf_branch_table *bt = cfbf_initialize_branch_table();

        if (bt == NULL) {
                goto err;
        }

        if (cfbf_create_branch_table(state, bt) != 0) {
                goto err;
        }

        for (size_t i = 0; i < state->commands_length; ++i) {
                switch (state->commands[i]) {
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
                case ACCEPT_BYTE:
                        state->tape[state->head] = (uint8_t)getchar();
                default:
                        break;
                }
        }

err:
        cfbf_destroy_branch_table(bt);
        return 1;
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