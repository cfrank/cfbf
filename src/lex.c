#include <stdio.h>
#include "lex.h"

int cfbf_consume_file(FILE *file)
{
        char c;
        do {
                cfbf_consume_file_char(file, &c);
                printf("%c", c);
                return 1;
        } while (c != EOF);
}

void cfbf_consume_file_char(FILE *file, char *pointer)
{
        *pointer = (char)fgetc(file);
}

cfbf_token tokenize(char input)
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