/*
 * Dec to Hex converter
 *
 * Because I hate Googling to convert Decimal to Hex...
 */

#include <stdio.h>
#include <stdlib.h>

main(int argc, char** argv) {
	if (argc < 2) {
		fprintf(stderr, "Usage: hex [decimal number]...\n");
		exit(1);
	}
	
	unsigned int i = 1;
	int val;
	for (; i < argc; i++) {
		val = atoi(argv[i]);
		printf("%d -> %s0x%x\n", val, (val < 0) ? "-" : "", (val < 0) ? -val : val);
	}
}
