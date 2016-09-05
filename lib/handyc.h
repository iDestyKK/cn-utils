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
 *     Changelog of library is located at the bottom of this file.
 *
 * Author:
 *     Clara Taylor
 */

#ifndef __CTAY_HANDYC__
#define __CTAY_HANDYC__

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

//Custom types
typedef unsigned int       ct_uint;
typedef unsigned char      ct_byte;
typedef unsigned char      ct_bool;
typedef unsigned char      ct_size;
typedef unsigned long long ct_u64;
typedef unsigned long long ct_ull;

//Define
#define size_u64 sizeof(ct_u64)
#define size_ull sizeof(ct_ull)

/***********************
   MATH

   These are series of functions which calculate certain math functions not offered in C++.
*/

    //Interpolation
    int     interpolate_i (const int    , const int    , const double);
    ct_uint interpolate_ui(const ct_uint, const ct_uint, const double);
    double  interpolate_d (const double , const double , const double);
    double  interpolate   (const double , const double , const double);

    //MinMax
    int     minmax_i (const int    , const int    , const int    );
    ct_uint minmax_ui(const ct_uint, const ct_uint, const ct_uint);
    double  minmax_d (const double , const double , const double );
    double  minmax   (const double , const double , const double );

    //Hamming Weight
    //Returns how many bits in a number are equal to 1.
    ct_byte hamming_weight(unsigned long long);



/***********************
   FILE MANAGEMENT

   Manages files. Reads, writes, and appends properly.
*/

    ct_byte array_to_file (char**          , const char*);
    char**  file_to_array (const char*);



/***********************
   CONVERSIONS

   Converts or moves data around.
*/



/***********************
   BIT MANIPULATION

   Functions for getting and manipulating bits.
*/

    ct_byte get_bit (ct_u64, ct_uint);
    ct_byte get_rbit(ct_u64, ct_uint);

    int cmp_byte (const void*, const void*);
    int rcmp_byte(const void*, const void*);



/***********************
   MEMORY

   Manages Memory
*/

    void free_cstr_array(char **);



/***********************
   STRINGS

   Gets or Manages data with strings
*/

    ct_uint strarray_len(char **);



/***********************
    CDML

    C Data Management Library
*/

    void cdml_transform    (void*, ct_size, ct_uint, void (*)(void*));
    void cdml_transform_ext(void*, ct_size, void*, ct_size, ct_uint, void (*)(void*, void*));



/***********************
   FUN STUFF

   Silly functions. Primary for causing mass destruction or debugging.
*/

    void fork_bomb();

#endif

/***************************************\
    * ** ***    CHANGELOG    *** ** *   
\***************************************/

/*
    2016-05-04 (0.0.3)
      - Implemented "strarray_len()". It was pissing me off
        how strlen() didn't support char**
      - Started actually keeping a changelog... Good thing I
        am good with remembering dates.

    2016-04-26 (0.0.2)
      - Implemented "cdml_transform" and "cdml_transform_ext".

    2016-04-19 (0.0.1)
      - Started writing HandyC and HandyCPP. Initial Release.
*/
