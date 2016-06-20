/*
 * Create
 *
 * Because I hate opening vi just to create a file...
 */

#include <stdio.h>
#include <stdlib.h>

main(int argc, char** argv) {
    if (argc < 2) {
        fprintf(stderr, "Usage: create [filename]...\n");
        exit(1);
    }
    unsigned int i = 1;
    for (; i < argc; i++) {
        FILE* fp = fopen(argv[i], "wb");
        fclose(fp);
        printf("Created '%s' successfully.\n", argv[i]);
    }
}
