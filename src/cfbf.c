#include "cfbf.h"
#include <getopt.h>
#include <stdio.h>

// Version data
#define VERSION_MAJOR '0'
#define VERSION_MINOR '1'
#define VERSION_PATCH '0'

int main(int argc, char **argv)
{
        static struct option long_options[] = {
                {"version", 0, 0, 'v'},
                {"license", 0, 0, 'l'},
                {"help", 0, 0, 'h'},
                {"file", 0, 0, 'f'},
                {NULL, 0, NULL, 0},
        };

        int option;
        int option_index = 0;

        while ((option = getopt_long(argc, argv, "vlhf:", long_options, &option_index)) != -1) {
                switch (option) {
                case 0:
                        printf("%s", "This is the 0 case");
                        break;

                case 'v':
                        /* Version */
                        cfbf_print_version();
                        break;

                case 'l':
                        printf("%s", "License");
                        break;

                case 'h':
                        printf("%s", "Help");
                        break;

                case 'f':
                        printf("%s", "File");
                        break;

                default:
                        puts("For usage, run ./cfbf -h");
                        break;
                }
        }
}

static void cfbf_print_version(void)
{
        printf("CFBF Brainfuck Interpreter Version: %c.%c.%c\n",
               VERSION_MAJOR, VERSION_MINOR, VERSION_PATCH);
}