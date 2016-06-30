/*
 * Randstr
 * 
 * Creates a random string of characters and integers based on command line specifications.
 *
 * Syntax:
 *     randstr [options]...
 *
 * Clara Van Nguyen (iDestyKK)
 * idesty@derpg.xyz
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <unistd.h>
#include <sys/types.h>

//Custom Types
typedef unsigned char BYTE;
typedef unsigned int  UINT;

typedef struct config {
	int   len,     //String Length
	      liblen;  //Library Length
	UINT  repeat,  //Number of occurances
	      seed;    //Seed for PRNG.
	BYTE* library; //Library of allowed characters in the password
} CONFIG;

void show_help() {
	fprintf(stderr, "Usage: randstr [OPTIONS]...\n\n"
	                "Random String Generator by Clara Van Nguyen.\n"
			        "Takes paramaters and generates a random string of data directed to stdout.\n\n"
			        "The following parameters are possible:\n"
			        "  -h\tShows this help prompt and exits\n"
			        "  -s\tSpecifies size (ex. -s=23)\n"
					"  -g\tSpecifies generation seed, must be an integer (ex. -s=1234)\n"
			        "  -r\tSpecifies repeats/number of strings created (ex. -r=2)\n"
			        "  -l\tAdds lower case letters to generation\n"
			        "  -u\tAdds upper case letters to generation\n"
			        "  -n\tAdds numbers to generation\n"
			        "  -a\tAdds all possible characters to generation\n\n"
			        "It should be noted that -l, -u, -n, and -a can be added multiple times. Doing\n"
			        "so will increase the chances of those values getting hit by the random number\n"
			        "generator, as it will re-add them into the library of possible characters.\n");
}

void parse_arguments(int argc, char** argv, CONFIG* CFG) {
	unsigned int i = 1,
				 a, sl, len, aa;
	BYTE* lib_add;

	for (; i < argc; i++) {
		if (argv[i][0] == '-') {
			//It is a command
			sl = strlen(argv[i]);
			for (a = 1; a < sl; a++) {
				switch (argv[i][a]) {
					default:
						//Let's just get this over with...
						fprintf(stderr, "[\e[0;31mWARNING\e[0m] Unknown parameter \"%c\" in \"%s\".\n", argv[i][a], argv[i]);
						break;
					case '-':
						//You spelled out some options
						//TODO: Add those options... I'm very lazy person as you can see.
						//if (strcmp)
						break;
					case 'h':
						//Help (and exit)
						show_help();
						exit(0);
						break;
					case 'g':
						//Specify seed
						a++;
						if (a < sl && argv[i][a] == '=') {
							CFG->seed = atoi(argv[i] + ++a); //Yup, we are totally cheating here.
							aa = CFG->seed * 0.1;
							while (aa > 0)
								a++,
								aa *= 0.1;
							break;
						}
						else {
							//You failed, but try to read anyways.
							CFG->seed = atoi(argv[i] + a);
							if (CFG->seed != 0) {
								aa = CFG->seed * 0.1;
								while (aa > 0)
									a++,
									aa *= 0.1;
							} else {
								fprintf(stderr, "[ \e[0;31mERROR\e[0m ] Expected an \"=\" after \"%c\" in \"%s\". Aborted.", argv[i][a], argv[i]);
								exit(1);
							}
						}
						break;

					case 's':
						//Specify size
						a++;
						if (a < sl && argv[i][a] == '=') {
							CFG->len = atoi(argv[i] + ++a); //Yup, we are totally cheating here.
							aa = CFG->len * 0.1;
							while (aa > 0)
								a++,
								aa *= 0.1;
							break;
						}
						else {
							//You failed, but try to read anyways.
							CFG->len = atoi(argv[i] + a);
							if (CFG->len != 0) {
								aa = CFG->len * 0.1;
								while (aa > 0)
									a++,
									aa *= 0.1;
							} else {
								fprintf(stderr, "[ \e[0;31mERROR\e[0m ] Expected an \"=\" after \"%c\" in \"%s\". Aborted.", argv[i][a], argv[i]);
								exit(1);
							}
						}
						break;
					case 'r':
						//Specify repeats
						a++;
						if (a < sl && argv[i][a] == '=') {
							CFG->repeat = atoi(argv[i] + ++a); //Yup, we are totally cheating here.
							aa = CFG->repeat * 0.1;
							while (aa > 0)
								a++,
								aa *= 0.1;
							break;
						}
						else {
							//You failed, but try to read anyways.
							CFG->repeat = atoi(argv[i] + a);
							if (CFG->repeat != 0) {
								aa = CFG->repeat * 0.1;
								while (aa > 0)
									a++,
									aa *= 0.1;
							}
							else {
								fprintf(stderr, "[ \e[0;31mERROR\e[0m ] Expected an \"=\" after \"%c\" in \"%s\". Aborted.", argv[i][a], argv[i]);
								exit(1);
							}
						}
						break;

					case 'l':
						//Add lowercase letters to dictionary.

						//Generate all lowercase letters.
						len = 0x7A - 0x61;
						aa  = 0;
						lib_add = (BYTE*) malloc(len++ + 1);
						for (; aa < len; aa++)
							lib_add[aa] = 0x61 + aa;
						lib_add[aa] = '\0';
						//printf("Appending: %s\n", lib_add);

						//Add it to the length of the library
						CFG->library = realloc(CFG->library, CFG->liblen + len);
						CFG->liblen += len;
						strcat(CFG->library, lib_add);
						free(lib_add);
						break;
					case 'u':
						//Add uppercase letters to dictionary.

						//Generate all uppercase letters.
						len = 0x5A - 0x41;
						aa  = 0;
						lib_add = (BYTE*) malloc(len++ + 1);
						for (; aa < len; aa++)
							lib_add[aa] = 0x41 + aa;
						lib_add[aa] = '\0';
						//printf("Appending: %s\n", lib_add);

						//Add it to the length of the library
						CFG->library = realloc(CFG->library, CFG->liblen + len);
						CFG->liblen += len;
						strcat(CFG->library, lib_add);
						free(lib_add);
						break;
					case 'n':
						//Add numbers to dictionary.

						//Generate all numbers.
						len = 0x39 - 0x30;
						aa  = 0;
						lib_add = (BYTE*) malloc(len++ + 1);
						for (; aa < len; aa++)
							lib_add[aa] = 0x30 + aa;
						lib_add[aa] = '\0';
						//printf("Appending: %s\n", lib_add);

						//Add it to the length of the library
						CFG->library = realloc(CFG->library, CFG->liblen + len);
						CFG->liblen += len;
						strcat(CFG->library, lib_add);
						free(lib_add);
						break;
					case 'a':
						//Add all characters to dictionary.

						//Generate all characters
						len = 0xFF - 0x01;
						aa  = 0;
						lib_add = (BYTE *) malloc(len++ + 1);
						for (; aa < len; aa++)
							lib_add[aa] = 0x01 + aa;
						lib_add[aa] = '\0';

						CFG->library = realloc(CFG->library, CFG->liblen + len);
						CFG->liblen += len;
						strcat(CFG->library, lib_add);
						free(lib_add);
						break;

				}
			}
		} else {
			//You are an idiot
			fprintf(stderr, "[\e[0;31mWARNING\e[0m] Unknown parameter \"%s\".\n", argv[i]);
		}
	}
}

main(int argc, char** argv) {
	//Make configuration object
	CONFIG CFG;
	CFG.len     = 0;
	CFG.liblen  = 0;
	CFG.repeat  = 1; //Default Value for idiot-proof-ness
	CFG.seed    = time(NULL) * getpid();
	CFG.library = (BYTE *) calloc(1, 1);
	CFG.library[0] = '\0';

	//Parse arguments
	parse_arguments(argc, argv, &CFG);

	if (argc == 1) {
		show_help();
		exit(0);
	}

	//Int Range { 0x30 - 0x39 }
	//LC  Range { 0x61 - 0x7A }
	//UC  Range { 0x41 - 0x5A }
	UINT r = 0;
	for (; r < CFG.repeat; r++) {	
		BYTE* stream = (BYTE *) malloc(CFG.len + 1);
		srand(CFG.seed + r);
		
		UINT i = 0;
		for (; i < CFG.len; i++)
			stream[i] = CFG.library[rand() % CFG.liblen];
		stream[i] = '\0';
		
		printf("%s\n", stream);
		free(stream);
	}
	free(CFG.library);
}
