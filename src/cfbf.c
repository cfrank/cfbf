#include <getopt.h>
#include <stdio.h>
#include <stdlib.h>
#include "cfbf.h"
#include "brainfuck.h"

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
                {"file", required_argument, 0, 'f'},
                {NULL, 0, NULL, 0},
        };

        int option;
        int option_index = 0;

        while ((option = getopt_long(argc, argv, "vlhf:", long_options, &option_index)) != -1) {
                switch (option) {
                case 'v':
                        cfbf_print_version();
                        return EXIT_SUCCESS;

                case 'l':
                        cfbf_print_license();
                        return EXIT_SUCCESS;

                case 'h':
                        cfbf_print_help();
                        return EXIT_SUCCESS;

                case 'f':
                        if (optarg != NULL && cfbf_open_file(optarg) == 0) {
                                return EXIT_SUCCESS;
                        } else {
                                return EXIT_FAILURE;
                        }

                case '?':
                        // get_opt encountered an unknown option
                        puts("For usage, run ./cfbf -h");
                        break;
                }
        }

        return EXIT_SUCCESS;
}

static void cfbf_print_version(void)
{
        printf("CFBF Brainfuck Interpreter Version: %c.%c.%c\n",
               VERSION_MAJOR, VERSION_MINOR, VERSION_PATCH);
}

static void cfbf_print_license(void)
{
        puts("MIT License - https://opensource.org/licenses/MIT");
}

static void cfbf_print_help(void)
{
        cfbf_print_version();
        puts("\n");
        puts("./cfbf -f <Brainfuck File>");
        puts("-v -- Prints the current running program version");
        puts("-l -- Prints some useless license information");
        puts("-h -- Prints this help dialog");
        puts("-f <File> -- Specify the Brainfuck file to interpret *Required*");
        puts("\n");
}

static int cfbf_open_file(char *filename)
{
        FILE *file = fopen(filename, "r");

        if (file == NULL) {
                fprintf(stderr, "Could not open file with name '%s'\n", filename);
                return 1;
        } else {
                // Successfully opened file get size
                fseek(file, 0, SEEK_END);
                int32_t size = (int32_t)ftell(file);
                fseek(file, 0, SEEK_SET);

                cfbf_state *state = cfbf_initialize_state(file, size);

                if (state == NULL) {
                        goto err;
                }

                cfbf_run_commands(state);

                // Clean up
                cfbf_destroy_state(state);
                fclose(file);
                return 0;

        err:
                // Enountered an error
                cfbf_destroy_state(state);
                fclose(file);
                return 1;
        }

        return 0;
}