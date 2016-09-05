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

#include "cn_comp.h"

//Address Comparison
CNC_COMP cn_cmp_ptr(void* arg0, void* arg1) {
	return (CN_CMP_LESS    * ((size_t) arg0 < (size_t) arg1)) +
	       (CN_CMP_GREATER * ((size_t) arg0 > (size_t) arg1));
}

//C-String Comparison
CNC_COMP cn_cmp_cstr(void* arg0, void* arg1) {
	return strcmp(*(char**)arg0, *(char**)arg1);
}

//Signed Reals
//Char Comparison
CNC_COMP cn_cmp_char(void* arg0, void* arg1) {
	char* a = (char *)arg0,
	    * b = (char *)arg1;

	return (CN_CMP_LESS    * (*a < *b)) +
	       (CN_CMP_GREATER * (*a > *b));
}


//Integer Comparison
CNC_COMP cn_cmp_int(void* arg0, void* arg1) {
	int* a = (int *)arg0,
	   * b = (int *)arg1;

	return (CN_CMP_LESS    * (*a < *b)) +
	       (CN_CMP_GREATER * (*a > *b));
}

//Short Comparison
CNC_COMP cn_cmp_short(void* arg0, void* arg1) {
	short* a = (short *)arg0,
	     * b = (short *)arg1;

	return (CN_CMP_LESS    * (*a < *b)) +
	       (CN_CMP_GREATER * (*a > *b));
}

//Long Comparison
CNC_COMP cn_cmp_long(void* arg0, void* arg1) {
	long* a = (long *)arg0,
	    * b = (long *)arg1;

	return (CN_CMP_LESS    * (*a < *b)) +
	       (CN_CMP_GREATER * (*a > *b));
}

//Long Long Comparison
CNC_COMP cn_cmp_ll(void* arg0, void* arg1) {
	long long* a = (long long *)arg0,
	         * b = (long long *)arg1;

	return (CN_CMP_LESS    * (*a < *b)) +
	       (CN_CMP_GREATER * (*a > *b));
}

//Float Comparison
CNC_COMP cn_cmp_float(void* arg0, void* arg1) {
	float* a = (float *)arg0,
         * b = (float *)arg1;

	return (CN_CMP_LESS    * (*a < *b)) +
	       (CN_CMP_GREATER * (*a > *b));
}


//Double Comparison
CNC_COMP cn_cmp_double(void* arg0, void* arg1) {
	double* a = (double *)arg0,
	      * b = (double *)arg1;

	return (CN_CMP_LESS    * (*a < *b)) +
	       (CN_CMP_GREATER * (*a > *b));
}


//Long Double Comparison
CNC_COMP cn_cmp_ldouble(void* arg0, void* arg1) {
	long double* a = (long double *)arg0,
	           * b = (long double *)arg1;

	return (CN_CMP_LESS    * (*a < *b)) +
	       (CN_CMP_GREATER * (*a > *b));
}

//Unsigned Reals
//Unsigned Char Comparison
CNC_COMP cn_cmp_uchar(void* arg0, void* arg1) {
	unsigned char* a = (unsigned char *)arg0,
	             * b = (unsigned char *)arg1;

	return (CN_CMP_LESS    * (*a < *b)) +
	       (CN_CMP_GREATER * (*a > *b));
}


//Unsigned Integer Comparison
CNC_COMP cn_cmp_uint(void* arg0, void* arg1) {
	unsigned int* a = (unsigned int *)arg0,
	            * b = (unsigned int *)arg1;

	return (CN_CMP_LESS    * (*a < *b)) +
	       (CN_CMP_GREATER * (*a > *b));
}

//Unsigned Short Comparison
CNC_COMP cn_cmp_ushort(void* arg0, void* arg1) {
	unsigned short* a = (unsigned short *)arg0,
	              * b = (unsigned short *)arg1;

	return (CN_CMP_LESS    * (*a < *b)) +
	       (CN_CMP_GREATER * (*a > *b));
}

//Unsigned Long Comparison
CNC_COMP cn_cmp_ulong(void* arg0, void* arg1) {
	unsigned long* a = (unsigned long *)arg0,
	             * b = (unsigned long *)arg1;

	return (CN_CMP_LESS    * (*a < *b)) +
	       (CN_CMP_GREATER * (*a > *b));
}

//Unsigned Long Long Comparison
CNC_COMP cn_cmp_ull(void* arg0, void* arg1) {
	unsigned long long* a = (unsigned long long *)arg0,
	                  * b = (unsigned long long *)arg1;

	return (CN_CMP_LESS    * (*a < *b)) +
	       (CN_CMP_GREATER * (*a > *b));
}
