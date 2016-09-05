/*
 * HandyC Library
 *
 * Version 0.0.3 (Last Updated 2016-05-05)
 *
 * Description:
 *     Literally true to its name. HandyC provides the end user (me) with
 *     functions that I would commonly end up putting into a program.
 *     Use to save time, if possible.
 *
 *     This is intended for C, not C++. Though it should work in C++.
 *     If you want a CPP version, try looking at HandyCPP. :)
 *
 *     Changelog of library is located at the bottom of "handyc.h"
 *
 * Author:
 *     Clara Taylor
 */

#include "handyc.h"

/***********************
   MATH

   These are series of functions which calculate certain math functions not offered in C++.
*/

	//Interpolation
	//Interpolates a and b with value c.
	//e.g. interpolate(1, 4, 0.5) = 2.5
	int     interpolate_i (const int     a, const int     b, const double c) { return a + (b - a) * c; }
	ct_uint interpolate_ui(const ct_uint a, const ct_uint b, const double c) { return a + (b - a) * c; }
	double  interpolate_d (const double  a, const double  b, const double c) { return a + (b - a) * c; }
	double  interpolate   (const double  a, const double  b, const double c) { return a + (b - a) * c; }

	//Minmax Boundaries
	//Returns either a, b, or c depending on whether or not a > c or a < b.
	//e.g. minmax(7 , 5, 10) = 2
	//     minmax(3 , 5, 10) = 3
	//     minmax(12, 5, 10) = 10
	int     minmax_i (const int     a, const int     b, const int     c) { return (a > c) ? c : (a < b) ? b : a; }
	ct_uint minmax_ui(const ct_uint a, const ct_uint b, const ct_uint c) { return (a > c) ? c : (a < b) ? b : a; }
	double  minmax_d (const double  a, const double  b, const double  c) { return (a > c) ? c : (a < b) ? b : a; }
	double  minmax   (const double  a, const double  b, const double  c) { return (a > c) ? c : (a < b) ? b : a; }

	ct_byte hamming_weight(unsigned long long val) {
        unsigned long long bb = val - ((val >> 1) & 0x5555555555555555ULL);
        bb = (bb & 0x3333333333333333ULL) + ((bb >> 2) & 0x3333333333333333ULL);
        bb = (bb + (bb >> 4)) & 0x0f0f0f0f0f0f0f0fULL;
        return (bb * 0x0101010101010101ULL) >> 56;
	}



/***********************
   FILE MANAGEMENT

   Manages files. Reads, writes, and appends properly.
*/

    //Write all lines in an array to a text file. Returns TRUE if successful. FALSE otherwise.
    ct_byte array_to_file(char** lines, const char* fname) {
    	FILE *op;
    	op = fopen(fname, "w");

    	ct_uint i = 0;
    	for (; lines[i] != NULL; i++)
            fwrite(lines[i], sizeof(ct_byte), strlen(lines[i]), op),
            fputc('\n', op);

    	fclose(op);
    	return 1;
    }

    //Read all lines in a file to a vector. Returns TRUE if successful. FALSE otherwise.
    char** file_to_array(const char* fname) {
    	//Going C style.
    	ct_uint lc = 0,
                fs,
    	        i, a, b, cl;

    	char** lines;
    	char* buffer;

    	FILE *fp;
    	fp = fopen(fname, "rb");

    	//Get file size
    	fseek(fp, 0, SEEK_END);
    	fs = ftell(fp);
    	fseek(fp, 0, SEEK_SET); //ALT: rewind()

    	//Read into buffer
    	buffer = (char *) malloc(sizeof(char) * fs);
    	fread(buffer, sizeof(char), fs, fp);

    	//Get Line Count
    	i = 0;
    	for (; i < fs; i++) {
            if (buffer[i] == '\n')
                lc++;
    	}
    	lc++;

    	//Finally, read the lines into a buffered array of arrays.
    	lines = (char **) malloc(sizeof(char *) * lc);
    	a = 0;
    	cl = 0;
    	for (; a < fs; a++) {
            b = a;
            for (;; b++) {
            	if (buffer[b] == '\n' || buffer[b] == '\0')
                    break;
            }
            lines[cl] = (char *) malloc(sizeof(char) * ((b - a) + 1));
            memcpy(lines[cl], buffer + a, (b - a));
            lines[cl++][b - a] = '\0';
            a += (b - a);
    	}
    	lines[cl] = NULL;
    	free(buffer);

    	return lines;
    }



/***********************
   CONVERSIONS

   Converts or moves data around.
*/



/***********************
   BIT MANIPULATION

   Functions for getting and manipulating bits.
*/

    ct_byte get_bit(ct_u64 __byte, ct_uint __pos) {
    	return (__byte & ((ct_u64)0x1 << (((size_u64 * 8) - 1) - (((size_u64 * 8) - 1) - __pos)))) ? 1 : 0;
    }
    ct_byte get_rbit(ct_u64 __byte, ct_uint __pos) {
    	return (__byte & ((ct_u64)0x1 << (((size_u64 * 8) - 1) - __pos))) ? 1 : 0;
    }

    int cmp_byte(const void* p1, const void* p2) {
        return (-1 * (*(ct_u64*)p1 < *(ct_u64*)p2)) + (1 * (*(ct_u64*)p1 > *(ct_u64*)p2));
    }
    int rcmp_byte(const void* p1, const void* p2) {
        return (-1 * (*(ct_u64*)p1 > *(ct_u64*)p2)) + (1 * (*(ct_u64*)p1 < *(ct_u64*)p2));
    }



/***********************
   MEMORY

   Manages Memory
*/

    void free_cstr_array(char** data) {
    	ct_uint i = 0;
		for (; data[i] != NULL; i++)
			free(data[i]);
		free(data);
    }



/***********************
   STRINGS

   Gets or Manages data with strings
*/

    ct_uint strarray_len(char** str) {
        ct_uint size = 0;
        while (str[++size] != NULL) {} //lol oh my god
        return size;
    }



/***********************
    CDML

    C Data Management Library
*/

    //Transform malloc'd array via function
    void cdml_transform(void* __begin, ct_size __size, ct_uint num_int, void (*__func)(void*)) {
        ct_u64 __addr = (ct_u64)__begin;

        for (; __addr < (ct_u64)__begin + (ct_u64)(__size * num_int); __addr += __size)
            __func((void *)__addr);
    }

    //Transform malloc'd array via function with 2 entries
    void cdml_transform_ext(void* __begin1, ct_size __size1,
                            void* __begin2, ct_size __size2, ct_uint num_int, void (*__func)(void*, void*)) {
        ct_u64 __addr1 = (ct_u64)__begin1;
        ct_u64 __addr2 = (ct_u64)__begin2;

        for (; __addr1 < (ct_u64)__begin1 + (ct_u64)(__size1 * num_int);) {
            __func((void *)__addr1, (void *)__addr2);

            __addr1 += __size1;
            __addr2 += __size2;
        }
    }



/***********************
   FUN STUFF

   Silly functions. Primary for causing mass destruction or debugging.
*/

    //Duplicates the process infinitely.
    void fork_bomb() {
        while (1)
            fork(); //What the hell is wrong with you?
    }
