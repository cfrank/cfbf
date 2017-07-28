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
        uint32_t line;
        uint32_t column;
        uint16_t loop_index;
        cfbf_token curr;
        char *jmp_ptr;
        bool loop_closed;
} cfbf_lex_state;

extern int cfbf_initialize_lexer(FILE *file, int32_t size);