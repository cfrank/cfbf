#include <stdio.h>
#include "lex.h"

static void cfbf_consume_file_char(FILE *file, char *pointer);
static cfbf_token cfbf_tokenize(char input);

extern int cfbf_consume_file(FILE *file)
{
        char c;
        do {
                printf("%d\n", cfbf_tokenize(c));
                cfbf_consume_file_char(file, &c);
        } while (c != EOF);

        return 1;
}

static void cfbf_consume_file_char(FILE *file, char *pointer)
{
        *pointer = (char)fgetc(file);
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