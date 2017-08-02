#pragma once

#include <stdint.h>
#include <stdio.h>

typedef enum cfbf_token {
        INCR_PTR,
        DECR_PTR,
        INCR_BYTE,
        DECR_BYTE,
        OUTP_BYTE,
        ACCEPT_BYTE,
        JMP_FWRD,
        JMP_BACK,
        UNKNOWN,
} cfbf_token;

typedef struct cfbf_state {
        cfbf_token *commands;
        uint32_t commands_length;
        uint8_t *tape;
        uint32_t head;
} cfbf_state;

// Branch index tuple
typedef struct cfbf_branch_value {
        uint32_t open;
        uint32_t close;
} cfbf_branch_value;

typedef struct cfbf_branch_table {
        cfbf_branch_value *table;
        int32_t index;
        int32_t size;
} cfbf_branch_table;

extern cfbf_state *cfbf_initialize_state(FILE *file, int32_t size);
extern int cfbf_run_commands(cfbf_state *state);
extern void cfbf_destroy_state(cfbf_state *state);