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

typedef struct cfbf_command {
        cfbf_token token;
        int32_t jmp_ptr;
} cfbf_command;

typedef struct cfbf_state {
        cfbf_command *commands;
        uint32_t commands_length;
        uint8_t *tape;
        uint32_t head;
} cfbf_state;

cfbf_state *cfbf_initialize_state(FILE *file, int32_t size);
bool cfbf_run_commands(cfbf_state *state);
void cfbf_destroy_state(cfbf_state *state);
