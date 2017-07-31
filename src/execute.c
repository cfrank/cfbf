#include <stdio.h>
#include <stdlib.h>
#include "execute.h"

#define DATA_SIZE 30000

static int8_t *cfbf_initialize_tape();

static int8_t *cfbf_initialize_tape()
{
        int8_t *temp = malloc(sizeof(int8_t) * DATA_SIZE);

        if (temp == NULL) {
                fprintf(stderr, "Could not allocate %lu bytes for tape",
                        sizeof(int8_t) * DATA_SIZE);
                return NULL;
        }

        return temp;
}

extern int cfbf_execute_commands(cfbf_lex_state *state)
{
        // Initialize a DATA_SIZE length array of 8 bytes
        int8_t *tape = cfbf_initialize_tape();

        for (size_t i = 0; i < state->commands_length; ++i) {
                printf("%d\n", state->commands[i]);
        }

        free(tape);

        return 0;
}
