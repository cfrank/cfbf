#pragma once

#include <stdbool.h>
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

typedef struct cfbf_lex_state {
        uint32_t command_index;
        uint32_t jmp_index;
        uint16_t loop_index;
        cfbf_token *commands;
        bool loop_closed;
} cfbf_lex_state;

extern cfbf_lex_state *cfbf_initialize_lexer(FILE *file, int32_t size);
extern void cfbf_free_lex_state(cfbf_lex_state *state);