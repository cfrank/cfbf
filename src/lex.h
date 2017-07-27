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

int cfbf_comsume_file(FILE *file);
void cfbf_consume_file_char(FILE *file, char *pointer);
cfbf_token cfbf_tokenize(char input);