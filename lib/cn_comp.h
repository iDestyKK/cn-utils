/*
 * CN Comparison Functions
 * 
 * Description:
 *     This supplies functions to compare data types. This is used mainly for
 *     CN Data structures that require sorting such as CN_Maps. The functions
 *     all have similar return values. -1 if arg0 is less than arg1, 0 if arg0
 *     is equal to arg1, and 1 if arg0 is greater than arg1. The only
 *     exception is cn_cmp_cstr, which returns "how different" the first
 *     string is compared to the second.
 * 
 * Author:
 *     Clara Van Nguyen
 */

#ifndef __CN_COMP__
#define __CN_COMP__

#include <string.h>

//Type Definitions
typedef int CNC_COMP;

//Others
#define CN_CMP_LESS    -1
#define CN_CMP_EQUAL    0
#define CN_CMP_GREATER  1

CNC_COMP cn_cmp_ptr    (void* , void* ); //Pointer Comparison
CNC_COMP cn_cmp_cstr   (void* , void* ); //C-String Comparison

//Signed Reals
CNC_COMP cn_cmp_char   (void* , void* ); //Char Comparison
CNC_COMP cn_cmp_int    (void* , void* ); //Integer Comparison
CNC_COMP cn_cmp_short  (void* , void* ); //Short Comparison
CNC_COMP cn_cmp_long   (void* , void* ); //Long Comparison
CNC_COMP cn_cmp_ll     (void* , void* ); //Long Long Comparison
CNC_COMP cn_cmp_float  (void* , void* ); //Float Comparison
CNC_COMP cn_cmp_double (void* , void* ); //Double Comparison
CNC_COMP cn_cmp_ldouble(void* , void* ); //Long Double Comparison

//Unsigned Reals
CNC_COMP cn_cmp_uchar  (void* , void* ); //Unsigned Char Comparison
CNC_COMP cn_cmp_uint   (void* , void* ); //Unsigned Integer Comparison
CNC_COMP cn_cmp_ushort (void* , void* ); //Unsigned Short Comparison
CNC_COMP cn_cmp_ulong  (void* , void* ); //Unsigned Long Comparison
CNC_COMP cn_cmp_ull    (void* , void* ); //Unsigned Long Long Comparison

//Macros just if you want to cheat (Or rather... if you "can")
#define cn_cmp_real(type, a, b) \
	(_CN_CMP_LESS    * (*(type*)a < *(type*)b)) + \
    (_CN_CMP_GREATER * (*(type*)a > *(type*)b))

#define cn_cmp_addr(addr1, addr2) \
	cn_cmp_ptr(addr1, addr2)

#define cn_cmp_void(addr1, addr2) \
	cn_cmp_ptr(addr1, addr2)

#endif
