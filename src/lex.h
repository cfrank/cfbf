#pragma once

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

extern int cfbf_consume_file(FILE *file);