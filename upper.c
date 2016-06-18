/*
 * To Upper
 *
 * Reads a file and pipes to stdout the same file with all ASCII characters set to uppercase.
 * Demonstrates a use of HandyC.
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

#include "lib/handyc.h"

void to_upper(void* str) {
    char* string = *((char**) str);
    ct_uint i = 0;
    for (; string[i] != NULL; i++)
        string[i] = toupper(string[i]);
}

main(int argc, char** argv) {
    if (argc != 2) {
        fprintf(stderr, (argc == 1) ? "USAGE: upper filename\n" : "ERROR: Expected 1 argument, received %d.\n", argc - 1);
        exit(1);
    }

    //Read from the file and process
    char** lines = file_to_array(argv[1]);
    ct_uint ln = strarray_len(lines);
    cdml_transform(lines, sizeof(char**), ln, to_upper);

    ct_uint i = 0;
    for (; i < ln; i++)
        printf("%s\n", lines[i]);

    free_cstr_array(lines);
}
