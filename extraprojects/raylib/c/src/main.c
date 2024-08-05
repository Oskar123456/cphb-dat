#include "../include/includes_dot_haitch.h"
#include "../include/obh_defs.h"
#include "../include/shader_utils.h"

#ifndef STB_IMAGE_IMPLEMENTATION
#define STB_IMAGE_IMPLEMENTATION
#include "../external/stb/stb_image.h"
#endif

#ifndef STB_DS_IMPLEMENTATION
#define STB_DS_IMPLEMENTATION
#include "../external/stb/stb_ds.h"
#endif

/*
 * **********************
 * ======================
 * obh C project template
 * ======================
 * **********************
 */

#include <ctype.h>
#include <wchar.h>

#include "decl.h"
#include "lexer.c"
#include "error_handling.c"

enum PROGRAM_ARG_MODE {
    PROGRAM_ARG_MODE_SRCFILE,   
};
typedef enum PROGRAM_ARG_MODE PROGRAM_ARG_MODE;

void
signalhandler(int sig)
{
    fprintf(stderr, "\n >>> received signal %d\n", sig);
    exit(0);
}

int
main(int argc, char *argv[])
{
    int a = 000xabc;
    int exit_status = EXIT_SUCCESS;

    signal(SIGINT, signalhandler);

    /* program argument parsing */
    int src_file_num = 0;
    char *src_file_args[64];

    PROGRAM_ARG_MODE arg_mode = PROGRAM_ARG_MODE_SRCFILE;
    for (int i = 1; i < argc; ++i) {
        if (arg_mode == PROGRAM_ARG_MODE_SRCFILE) {
            src_file_args[src_file_num++] = argv[i];
        }
    }

    printf("source files:\t");
    for (int i = 0; i < src_file_num; ++i) {
        printf("%s ", src_file_args[i]);
    }
    printf("\n");

    for (int i = 0; i < src_file_num; ++i) {
        FILE *src_file = fopen(src_file_args[i], "r");
        if (src_file == NULL) {
            fprintf(stderr, "%s: ", src_file_args[i]);
            system_error();
            exit_status = errno;
            goto RETURN;
        }
        printf("%s ", src_file_args[i]);
        int src_file_orientation = fwide(src_file, 0);
        if (src_file_orientation > 0) 
            printf("is wide-oriented");
        if (src_file_orientation == 0) 
            printf("has no orientation");
        if (src_file_orientation < 0) 
            printf("is byte-oriented");
        printf("\n");

        lexInit();
        lex(src_file);

        fclose(src_file);
    }


    
    /* scanning / lexing / tokenizing */
    /* syntactic analysis */
    /* context-sensitive analysis */

RETURN:
    return exit_status;
}
